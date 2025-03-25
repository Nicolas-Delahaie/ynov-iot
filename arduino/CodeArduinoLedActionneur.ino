#include <WiFi.h>
#include <PubSubClient.h>

// Identifiants WiFi
const char* ssid = "monwifi";
const char* password = "azertyui";

// Configuration du Broker MQTT
const char* mqtt_server = "192.168.137.56"; // Adresse IP du serveur MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// Broches de la LED RGB
#define RED_PIN 16
#define GREEN_PIN 17
#define BLUE_PIN 18

void setup() {
  Serial.begin(115200);  // Initialisation de la communication série avec l'ESP32
  delay(5000);

  // Connexion au Wi-Fi
  Serial.print("Connexion au WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connecté au WiFi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Connexion au broker MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Définir les broches des LEDs RGB comme sorties
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Éteindre la LED au démarrage
  setColor(0, 0, 0);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// Fonction de reconnexion MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("🔄 Connexion MQTT...");
    if (client.connect("ESP32_Receiver")) {
      Serial.println("✅ Connecté !");
      client.subscribe("capteur/humidity"); // S'abonne au topic "capteur/humidite"
    } else {
      Serial.print("❌ Échec, code erreur : ");
      Serial.print(client.state());
      Serial.println(" => Nouvelle tentative dans 5s");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("📥 Message reçu : ");

  // Convertir le payload en String proprement
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println(message);  // Affichage du message brut

  // Extraire uniquement la valeur numérique après "Humidité : "
  int index = message.indexOf(":");
  if (index != -1) {
    message = message.substring(index + 1); // Garde uniquement ce qui est après ":"
    message.trim(); // Supprime les espaces inutiles
  }

  // Convertir en float
  float humidity = message.toFloat();

  // Vérifier si la conversion a réussi
  if (humidity == 0 && message != "0") {
    Serial.println("⚠️ Erreur de conversion de l'humidité !");
    return;
  }

  Serial.print("💧 Humidité : ");
  Serial.println(humidity);

  // Modifier la couleur de la LED en fonction de l'humidité
  if (humidity < 30) {
    setColor(255, 0, 0); // Rouge si humidité < 30%
    Serial.println("🚨 Humidité trop faible, LED rouge.");
  } else if (humidity >= 30 && humidity < 60) {
    setColor(0, 255, 0); // Vert si humidité entre 30% et 60%
    Serial.println("✅ Humidité normale, LED verte.");
  } else {
    setColor(0, 0, 255); // Bleu si humidité > 60%
    Serial.println("💦 Humidité élevée, LED bleue.");
  }
}


// Fonction pour changer la couleur de la LED RGB
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}
