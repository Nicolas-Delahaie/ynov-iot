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
unsigned long wifiConnectTime = 0;
int mqttReconnectAttempts = 0;
int mqttMessagesReceived = 0;
int humidityLow = 0, humidityNormal = 0, humidityHigh = 0;
unsigned long lastMessageTime = 0;

// Broches de la LED RGB
#define RED_PIN 16
#define GREEN_PIN 17
#define BLUE_PIN 18

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

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void callback(char* topic, byte* payload, unsigned int length) {
  unsigned long messageStartTime = millis();
  mqttMessagesReceived++;

  Serial.print("📩 Message reçu sur le topic ");
  Serial.print(topic);
  Serial.print(": ");
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Gestion de la LED en fonction de l'humidité
  if (strcmp(topic, "capteur/humidity") == 0) {
    float humidity = message.toFloat();
    if (humidity < 30) {
      setColor(255, 0, 0);  // Rouge si humidité < 30%
      humidityLow++;
      Serial.println("🚨 Humidité faible, LED rouge.");
    } else if (humidity >= 30 && humidity < 60) {
      setColor(0, 255, 0);  // Vert si humidité entre 30% et 60%
      humidityNormal++;
      Serial.println("✅ Humidité normale, LED verte.");
    } else {
      setColor(0, 0, 255);  // Bleu si humidité > 60%
      humidityHigh++;
      Serial.println("💦 Humidité élevée, LED bleue.");
    }
  }

  lastMessageTime = millis() - messageStartTime;
  Serial.print("⏳ Latence réception MQTT : ");
  Serial.print(lastMessageTime);
  Serial.println(" ms");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("🔄 Connexion MQTT...");
    if (client.connect("ESP32_Subscriber")) {
      Serial.println("✅ Connecté !");
      client.subscribe("capteur/temperature");
      client.subscribe("capteur/humidity");
    } else {
      mqttReconnectAttempts++;
      Serial.print("❌ Échec, code erreur : ");
      Serial.print(client.state());
      Serial.println(" => Nouvelle tentative dans 5s");
      client.disconnect();
      delay(5000);
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

  // Affichage des statistiques toutes les 30 secondes
  static unsigned long lastStatsTime = 0;
  if (millis() - lastStatsTime > 30000) {
    lastStatsTime = millis();
    Serial.println("📊 Statistiques ESP32:");
    Serial.print("🔄 Nombre de reconnexions MQTT : ");
    Serial.println(mqttReconnectAttempts);
    Serial.print("📥 Messages MQTT reçus : ");#include <WiFi.h>
    #include <PubSubClient.h>
    #include <ArduinoOTA.h>
    
    // Identifiants WiFi
    const char* ssid = "monwifi";
    const char* password = "azertyui";
    
    // Configuration du Broker MQTT
    const char* mqtt_server = "rasp.local";
    
    WiFiClient espClient;
    PubSubClient client(espClient);
    
    // Broches de la LED RGB
    #define RED_PIN 16
    #define GREEN_PIN 17
    #define BLUE_PIN 18
    
    // Broches des LEDs
    #define LED_HIGH_PIN 14
    #define LED_LOW_PIN 0
    
    bool mqttConnected = false;
    unsigned long wifiConnectTime = 0;
    int mqttReconnectAttempts = 0;
    int mqttMessagesReceived = 0;
    float lastTemperature = 0;  // Stocke la dernière température reçue
    
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
    
    void setColor(int red, int green, int blue) {
      analogWrite(RED_PIN, red);
      analogWrite(GREEN_PIN, green);
      analogWrite(BLUE_PIN, blue);
    }
    
    void callback(char* topic, byte* payload, unsigned int length) {
      mqttMessagesReceived++;
    
      Serial.print("📩 Message reçu sur le topic ");
      Serial.print(topic);
      Serial.print(": ");
      String message;
      for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
      }
      Serial.println(message);
    
      // Gestion de la LED et du moteur
      if (strcmp(topic, "capteur/humidity") == 0) {
        float humidity = message.toFloat();
        if (humidity < 30) {
          setColor(255, 0, 0);  // Rouge
          Serial.println("🚨 Humidité faible, LED rouge.");
        } else if (humidity >= 30 && humidity < 60) {
          setColor(0, 255, 0);  // Vert
          Serial.println("✅ Humidité normale, LED verte.");
        } else {
          setColor(0, 0, 255);  // Bleu
          Serial.println("💦 Humidité élevée, LED bleue.");
        }
      } else if (strcmp(topic, "capteur/temperature") == 0) {
        lastTemperature = message.toFloat();
      } else if (strcmp(topic, "capteur/distance") == 0) {
        long distance = message.toFloat();
        if (distance > 200) {
          digitalWrite(LED_HIGH_PIN, HIGH);
          digitalWrite(LED_LOW_PIN, LOW);
          Serial.println("🔆 Distance élevée, LED Rouge allumée.");
        } else {
          digitalWrite(LED_HIGH_PIN, LOW);
          digitalWrite(LED_LOW_PIN, HIGH);
          Serial.println("🔴 Distance faible, LED Bleu allumée.");
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
          client.subscribe("capteur/distance");
        } else {
          mqttReconnectAttempts++;
          Serial.print("❌ Échec, code erreur : ");
          Serial.print(client.state());
          Serial.println(" => Nouvelle tentative dans 5s");
          client.disconnect();
          delay(5000);
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
    
      // Configuration des broches
      pinMode(LED_HIGH_PIN, OUTPUT);
      pinMode(LED_LOW_PIN, OUTPUT);
    
      // Connexion OTA
      ArduinoOTA.setHostname("ESP32_SUBSCRIBER_OTA");
      ArduinoOTA.begin();
    }
    
    void loop() {
      if (!client.connected()) {
        reconnect();
      }
    
      // Lancer la boucle MQTT
      client.loop();
      ArduinoOTA.handle();
    
      // Ajout d'une petite pause pour éviter un blocage continu
      delay(100);
    }
    
    Serial.println(mqttMessagesReceived);
    Serial.print("💧 Humidité faible (<30%) : ");
    Serial.println(humidityLow);
    Serial.print("✅ Humidité normale (30-60%) : ");
    Serial.println(humidityNormal);
    Serial.print("💦 Humidité élevée (>60%) : ");
    Serial.println(humidityHigh);
    Serial.print("⏳ Dernière latence MQTT : ");
    Serial.print(lastMessageTime);
    Serial.println(" ms");
  }
}
