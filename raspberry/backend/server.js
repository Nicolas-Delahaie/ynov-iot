const WebSocket = require("ws");
const mqtt = require("mqtt");

// Configuration du serveur WebSocket
const wss = new WebSocket.Server({ host: "0.0.0.0", port: 8080 });
console.log("Serveur WebSocket démarré sur ws://localhost:8080");

// Configuration du client MQTT
const MQTT_BROKER = "mosquitto";
const mqttClient = mqtt.connect(MQTT_BROKER);

// Liste des topics à écouter
const MQTT_TOPICS = ["capteur/humidity", "capteur/temperature", "capteur/distance"];

wss.on("connection", (socket) => {
    console.log("Un client WebSocket est connecté");

    socket.on("close", () => {
        console.log("Un client WebSocket s'est déconnecté");
    });
});

// Connexion au broker MQTT
mqttClient.on("connect", () => {
    console.log("Connecté au broker MQTT");

    // S'abonner aux topics
    MQTT_TOPICS.forEach(topic => mqttClient.subscribe(topic, (err) => {
        if (err) {
            console.error(`Erreur lors de l'abonnement à ${topic} :`, err);
        } else {
            console.log(`Abonné à ${topic}`);
        }
    }));
});

// Réception des messages MQTT et envoi aux clients WebSocket
mqttClient.on("message", (topic, message) => {
    const payload = {
        topic: topic,
        message: message.toString()
    };

    console.log(`Message MQTT reçu : ${JSON.stringify(payload)}`);

    // Envoi du message à tous les clients WebSocket connectés
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(payload));
        }
    });
});

// Gestion des erreurs MQTT
mqttClient.on("error", (err) => {
    console.error("Erreur MQTT :", err);
});

mqttClient.on("close", () => {
    console.log("Déconnecté du broker MQTT");
});
