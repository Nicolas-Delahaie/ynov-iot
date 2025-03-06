#include <DHT.h>

#define DHTPIN 3       // Broche du capteur DHT11
#define DHTTYPE DHT11  // Type de capteur
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // Communication série avec l'ESP8266
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    // Envoi des données sur le port série
    // Serial.print("Température : ");
    // Serial.print(temperature);
    Serial.print("Humidité : ");
    Serial.println(humidity);
  } else {
    Serial.println("Erreur de lecture du DHT11 !");
  }

  delay(5000);  // Attendre 5 secondes avant de refaire une lecture
}
