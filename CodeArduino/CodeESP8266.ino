#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuration WiFi
const char* ssid = "monwifi";
const char* password = "azertyui";

// Configuration MQTT
const char* mqtt_server = "192.168.137.56";  // Adresse de ton broker MQTT

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
    String data = Serial.readStringUntil('\n');
    Serial.print("Reçu de l'Arduino : ");
    Serial.println(data);  // Affiche les données reçues de l'Arduino
    client.publish("capteur/humidity", data.c_str());  // Publier sur le broker MQTT
  }
}
