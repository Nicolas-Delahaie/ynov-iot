#include "IRremote.h"
#include "DHT.h"

#define DHTPIN 3      // Broche du capteur DHT11 (DATA sur D3)
#define DHTTYPE DHT11 // Type du capteur
#define ANALOG_PIN A0 // Broche pour mesurer la tension
#define RAM_MAX 2048  // Estimation de la RAM maximale disponible

// Broches du récepteur IR
int receiver = 2;
int ledBlue = 9;
int ledRed = 10;

// Broches de la LED RGB
#define RED_PIN 6
#define GREEN_PIN 4
#define BLUE_PIN 5

bool ledBlueState = false;
bool ledRedState = false;

IRrecv irrecv(receiver);
uint32_t last_decodedRawData = 0;
DHT dht(DHTPIN, DHTTYPE);

extern int __heap_start, *__brkval;
int freeMemory() {
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void translateIR() {
  if (irrecv.decodedIRData.flags) {
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT SIGNAL!");
  }

  switch (irrecv.decodedIRData.decodedRawData) {
    case 0xF30CFF00:
      ledBlueState = !ledBlueState;
      digitalWrite(ledBlue, ledBlueState ? HIGH : LOW);
      Serial.println("--------------");
      Serial.print("LED Bleue ");
      Serial.println(ledBlueState ? "ON" : "OFF");
      Serial.println("--------------");
      break;

    case 0xE718FF00:
      ledRedState = !ledRedState;
      digitalWrite(ledRed, ledRedState ? HIGH : LOW);
      Serial.println("--------------");
      Serial.print("LED Rouge ");
      Serial.println(ledRedState ? "ON" : "OFF");
      Serial.println("--------------");
      break;

    default:
      Serial.println("Autre bouton pressé");
  }

  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(300);
}

void readDHT11() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur de lecture du DHT11 !");
    return;
  }

  Serial.println();
  Serial.print("Température : ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidité : ");
  Serial.print(humidity);
  Serial.print(" % | ");

  if (humidity < 30) {
    setColor(255, 0, 0);
    Serial.println("Alerte ! Humidité trop faible !");
    Serial.println("--------------");
  } else if (humidity >= 30 && humidity <= 60) {
    setColor(0, 255, 0);
    Serial.println("Humidité convenable");
    Serial.println("--------------");
  } else {
    setColor(0, 0, 255);
    Serial.println("Humidité parfaite");
    Serial.println("--------------");
  }
}

void displayMonitoring() {
  int rawValue = analogRead(ANALOG_PIN);
  float voltage = rawValue * (5.0 / 1023.0);
  Serial.print("Tension mesurée : ");
  Serial.print(voltage);
  Serial.println(" V");
  
  int freeMem = freeMemory();
  float freeMemPercentage = (freeMem / (float)RAM_MAX) * 100.0;
  Serial.print("RAM disponible : ");
  Serial.print(freeMemPercentage);
  Serial.print(" % ( ");
  Serial.print(freeMem);
  Serial.println(" octets )");
  Serial.println("--------------");
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receiver & DHT11 Sensor Start");

  irrecv.enableIRIn();
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  dht.begin();
}

void loop() {
  if (irrecv.decode()) {
    translateIR();
    irrecv.resume();
  }

  static unsigned long lastRead = 0;
  if (millis() - lastRead > 10000) {
    readDHT11();
    displayMonitoring();
    lastRead = millis();
  }
}
