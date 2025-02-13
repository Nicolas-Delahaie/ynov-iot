#include "IRremote.h"
#include "DHT.h"

#define DHTPIN 3      // Broche du capteur DHT11 (DATA sur D3)
#define DHTTYPE DHT11 // Type du capteur

// Broches du récepteur IR
int receiver = 2;  // Broche du récepteur IR
int ledBlue = 9;   // LED bleue sur D9
int ledRed = 10;   // LED rouge sur D10

// Broches de la LED RGB
#define RED_PIN 6
#define GREEN_PIN 4
#define BLUE_PIN 5

bool ledBlueState = false; // État actuel de la LED bleue
bool ledRedState = false;  // État actuel de la LED rouge

IRrecv irrecv(receiver);
uint32_t last_decodedRawData = 0;
DHT dht(DHTPIN, DHTTYPE);  // Initialisation du capteur DHT11

void translateIR() {
  if (irrecv.decodedIRData.flags) {
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT SIGNAL!");
  }

  switch (irrecv.decodedIRData.decodedRawData) {
    case 0xF30CFF00: // Code du bouton pour la LED bleue
      ledBlueState = !ledBlueState; 
      digitalWrite(ledBlue, ledBlueState ? HIGH : LOW);
      Serial.print("LED Bleue ");
      Serial.println(ledBlueState ? "ON" : "OFF");
      break;

    case 0xE718FF00: // Code du bouton pour la LED rouge
      ledRedState = !ledRedState; 
      digitalWrite(ledRed, ledRedState ? HIGH : LOW);
      Serial.print("LED Rouge ");
      Serial.println(ledRedState ? "ON" : "OFF");
      break;

    default:
      Serial.println("Autre bouton pressé");
  }

  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(300); // Petite pause pour éviter les rebonds
}

// Fonction pour lire les données du capteur DHT11
void readDHT11() {
  float temperature = dht.readTemperature(); // Lecture de la température
  float humidity = dht.readHumidity();       // Lecture de l'humidité

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur de lecture du DHT11 !");
    return;
  }

  Serial.print("Température : ");
  Serial.print(temperature);
  Serial.print(" °C | Humidité : ");
  Serial.print(humidity);
  Serial.print(" % | ");

  // Contrôle de la LED RGB en fonction de l'humidité
  if (humidity < 30) {
    // Humidité faible -> LED rouge
    setColor(255, 0, 0); // Rouge
    Serial.println("Alerte ! Humidité trop faible !");
  } else if (humidity >= 30 && humidity <= 60) {
    // Humidité normale -> LED verte
    setColor(0, 255, 0); // Vert
    Serial.println("Humidité convenable");
  } else {
    // Humidité élevée -> LED bleue
    setColor(0, 0, 255); // Bleu
    Serial.println("Humidité parfaite");
  }
}

// Fonction pour définir la couleur de la LED RGB
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receiver & DHT11 Sensor Start");

  irrecv.enableIRIn(); // Active le récepteur IR
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);

  // Configurer les broches de la LED RGB comme sorties
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  dht.begin(); // Démarre le capteur DHT11
}

void loop() {
  if (irrecv.decode()) {
    translateIR();
    irrecv.resume();
  }

  static unsigned long lastDHTRead = 0;
  if (millis() - lastDHTRead > 2000) { // Lecture du DHT11 toutes les 2 secondes
    readDHT11();
    lastDHTRead = millis();
  }
}
