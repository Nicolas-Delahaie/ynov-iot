#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

// Identifiants WiFi
const char* ssid = "monwifi";
const char* password = "azertyui";

// Configuration du Broker MQTT
const char* mqtt_server = "rasp.local";  // Adresse IP du serveur MQTT

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 4       // Broche du capteur DHT11
#define DHTTYPE DHT11  // Type de capteur
DHT dht(DHTPIN, DHTTYPE);

bool mqttConnected = false;

void setup_wifi() {
  Serial.print("Connexion au WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connecté au WiFi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
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
  } else {
    Serial.println("⚠️ Erreur de lecture du DHT11 !");
  }

  delay(5000);  // Attendre 5 secondes avant de refaire une lecture
}
