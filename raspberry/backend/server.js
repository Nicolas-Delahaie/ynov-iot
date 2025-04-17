const WebSocket = require("ws");
const mqtt = require("mqtt");
const { InfluxDB, Point } = require("@influxdata/influxdb-client");

// =========================
// Configuration InfluxDB
// =========================
const { INFLUX_URL, INFLUX_TOKEN, INFLUX_ORG, INFLUX_BUCKET } = process.env;

if (!INFLUX_URL || !INFLUX_TOKEN || !INFLUX_ORG || !INFLUX_BUCKET) {
  throw new Error(
    "Error : Please set INFLUX_URL, INFLUX_TOKEN, INFLUX_ORG and INFLUX_BUCKET environment variables"
  );
}
const influxDB = new InfluxDB({ url: INFLUX_URL, token: INFLUX_TOKEN });
const writeApi = influxDB.getWriteApi(INFLUX_ORG, INFLUX_BUCKET, "ns");

// =========================
// Configuration WebSocket
// =========================
const wss = new WebSocket.Server({ host: "0.0.0.0", port: 8080 });
console.log("✅ Serveur WebSocket démarré sur ws://localhost:8080");

wss.on("connection", (socket) => {
  console.log("🔌 Client WebSocket connecté");

  socket.on("close", () => {
    console.log("❌ Client WebSocket déconnecté");
  });
});

// =========================
// Configuration MQTT
// =========================
const MQTT_BROKER = "mqtt://mosquitto";
const MQTT_TOPICS = [
  "capteur/humidity",
  "capteur/temperature",
  "capteur/distance",
];

const mqttClient = mqtt.connect(MQTT_BROKER);

// =========================
// Unités par topic
// =========================
const UNITS = {
  "capteur/temperature": "°C",
  "capteur/distance": "cm",
  "capteur/humidity": "%",
};

// =========================
// Connexion au MQTT Broker
// =========================
mqttClient.on("connect", () => {
  console.log("✅ Connecté au broker MQTT");

  MQTT_TOPICS.forEach((topic) => {
    mqttClient.subscribe(topic, (err) => {
      if (err) {
        console.error(`❗ Erreur abonnement ${topic} :`, err);
      } else {
        console.log(`📡 Abonné à ${topic}`);
      }
    });
  });
});

// =========================
// Réception des messages MQTT
// =========================
mqttClient.on("message", (topic, message) => {
  const value = parseFloat(message.toString());
  const unit = UNITS[topic] || "";

  const payload = {
    topic,
    value,
    unit,
  };

  console.log(`📥 Message MQTT reçu : ${JSON.stringify(payload)}`);

  // Envoi aux clients WebSocket
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(payload));
    }
  });

  // Insertion dans InfluxDB
  const point = new Point(topic.replace("capteur/", ""))
    .floatField("value", value)
    .tag("unit", unit)
    .timestamp(new Date());

  writeApi.writePoint(point);
});

// =========================
// Gestion erreurs MQTT
// =========================
mqttClient.on("error", (err) => {
  console.error("❗ Erreur MQTT :", err);
});

mqttClient.on("close", () => {
  console.log("🔌 Déconnecté du broker MQTT");
});

// =========================
// Fermeture propre
// =========================
process.on("SIGINT", () => {
  console.log("⏳ Fermeture du serveur...");
  writeApi
    .close()
    .then(() => {
      console.log("✅ Données InfluxDB flushées avec succès.");
      process.exit(0);
    })
    .catch((err) => {
      console.error("❗ Erreur fermeture InfluxDB :", err);
      process.exit(1);
    });
});
