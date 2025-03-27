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

bool mqttConnected = false;

// Broches de la LED RGB
#define RED_PIN 16
#define GREEN_PIN 17
#define BLUE_PIN 18

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

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("📩 Message reçu sur le topic ");
  Serial.print(topic);
  Serial.print(": ");
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Gestion de la LED
  if (strcmp(topic, "capteur/humidity") == 0) {
    float humidity = message.toFloat();
    
    // Modifier la couleur de la LED en fonction de l'humidité
    if (humidity < 30) {
      setColor(255, 0, 0);  // Rouge si humidité < 30%
      Serial.println("🚨 Humidité trop faible, LED rouge.");
    } else if (humidity >= 30 && humidity < 60) {
      setColor(0, 255, 0);  // Vert si humidité entre 30% et 60%
      Serial.println("✅ Humidité normale, LED verte.");
    } else {
      setColor(0, 0, 255);  // Bleu si humidité > 60%
      Serial.println("💦 Humidité élevée, LED bleue.");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("🔄 Connexion MQTT...");
    if (client.connect("ESP32_Subscriber")) {
      Serial.println("✅ Connecté !");
      client.subscribe("capteur/temperature");
      client.subscribe("capteur/humidity");
    } else {
      Serial.print("❌ Échec, code erreur : ");
      Serial.print(client.state());
      Serial.println(" => Nouvelle tentative dans 5s");
      client.disconnect();  // Déconnexion propre
      delay(5000);  // Pause pour éviter un spam de tentatives
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  // Connexion au broker MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();

  // Connexion OTA
  ArduinoOTA.setHostname("ESP32_SUBSCRIBER_OTA");
  ArduinoOTA.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();
}
