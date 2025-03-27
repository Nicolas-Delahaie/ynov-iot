#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

// Identifiants WiFi
const char* ssid = "monwifi";
const char* password = "azertyui";

// Configuration du Broker MQTT
const char* mqtt_server = "rasp.local";

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

bool mqttConnected = false;
unsigned long wifiConnectTime = 0;
unsigned long mqttPublishTime = 0;
int mqttReconnectAttempts = 0;
int dhtReadSuccess = 0;
int dhtReadFailures = 0;
unsigned long lastStatsTime = 0;

void setup_wifi() {
  Serial.print("Connexion au WiFi...");
  wifiConnectTime = millis();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  wifiConnectTime = millis() - wifiConnectTime;
  Serial.println("\n✅ Connecté au WiFi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.print("⏳ Temps de connexion WiFi : ");
  Serial.print(wifiConnectTime);
  Serial.println(" ms");
}

void reconnect() {
  if (!client.connected()) {
    if (!mqttConnected) {
      Serial.print("🔄 Connexion MQTT...");
    }
    if (client.connect("ESP32_Receiver")) {
      if (!mqttConnected) {
        Serial.println("✅ Connecté !");
        mqttConnected = true;
      }
    } else {
      if (!mqttConnected) {
        Serial.print("❌ Échec, code erreur : ");
        Serial.print(client.state());
        Serial.println(" => Nouvelle tentative dans 5s");
      }
      mqttReconnectAttempts++;
      mqttConnected = false;
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();

  // Connexion au broker MQTT
  client.setServer(mqtt_server, 1883);
  reconnect();

  // Connexion OTA
  ArduinoOTA.setHostname("ESP32_SENDER_OTA");
  ArduinoOTA.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    dhtReadSuccess++;

    // Mesurer le temps de publication MQTT
    mqttPublishTime = millis();
    
    // Envoi des données sur MQTT
    char tempString[8], humString[8];
    dtostrf(temperature, 6, 2, tempString);
    dtostrf(humidity, 6, 2, humString);

    Serial.print("📡 Envoi MQTT (température) : ");
    Serial.println(tempString);
    client.publish("capteur/temperature", tempString);
    
    Serial.print("📡 Envoi MQTT (humidité) : ");
    Serial.println(humString);
    client.publish("capteur/humidity", humString);

    mqttPublishTime = millis() - mqttPublishTime;
    Serial.print("⏳ Latence MQTT : ");
    Serial.print(mqttPublishTime);
    Serial.println(" ms");

  } else {
    Serial.println("⚠️ Erreur de lecture du DHT11 !");
    dhtReadFailures++;
  }

  // Affichage des statistiques toutes les 30 secondes
  if (millis() - lastStatsTime > 30000) {
    lastStatsTime = millis();
    Serial.println("📊 Statistiques ESP32:");
    Serial.print("🔄 Nombre de reconnexions MQTT : ");
    Serial.println(mqttReconnectAttempts);
    Serial.print("📈 Lectures DHT réussies : ");
    Serial.println(dhtReadSuccess);
    Serial.print("⚠️ Lectures DHT échouées : ");
    Serial.println(dhtReadFailures);
  }

  delay(5000);
}
