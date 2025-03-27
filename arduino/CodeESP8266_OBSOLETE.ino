#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuration WiFi
const char* ssid = "monwifi";
const char* password = "azertyui";

// Configuration MQTT
const char* mqtt_server = "rasp.local";  // Adresse de ton broker MQTT

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connecté !");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connexion au serveur MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Connecté !");
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);  // Pour communiquer avec l'IDE Arduino
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');  // Lire les données de l'Arduino
    data.trim();  // Supprimer les espaces ou retours à la ligne inutiles
    Serial.print("🔹 Reçu de l'Arduino : ");
    Serial.println(data);

    if (data.startsWith("Humidité :")) {
      String humidityValue = data.substring(data.indexOf(":") + 1);
      humidityValue.trim();
      Serial.print("📡 Envoi MQTT (humidité) : ");
      Serial.println(humidityValue);
      client.publish("capteur/humidity", humidityValue.c_str());  // Publier l'humidité

    } else if (data.startsWith("Température :")) {
      String temperatureValue = data.substring(data.indexOf(":") + 1);
      temperatureValue.trim();
      Serial.print("📡 Envoi MQTT (température) : ");
      Serial.println(temperatureValue);
      client.publish("capteur/temperature", temperatureValue.c_str());  // Publier la température
    } else {
      Serial.println("⚠️ Format inconnu, message ignoré.");
    }
  }
}
