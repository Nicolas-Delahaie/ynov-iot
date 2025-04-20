#!/bin/bash

# Configuration
HOST=mosquitto
PORT=1883

# Boucle infinie
while true; do
  HUMIDITY=$(awk -v r=$RANDOM 'BEGIN { printf "%.1f", (r % 1000) / 10 }')
  DISTANCE=$(awk -v r=$RANDOM 'BEGIN { printf "%.1f", (r % 2000) / 10 }')
  TEMPERATURE=$(awk -v r=$RANDOM 'BEGIN { printf "%.1f", (r % 350) / 10 }')
  
  mosquitto_pub -h $HOST -p $PORT -t "capteur/humidity" -m "$HUMIDITY"
  mosquitto_pub -h $HOST -p $PORT -t "capteur/distance" -m "$DISTANCE"
  mosquitto_pub -h $HOST -p $PORT -t "capteur/temperature" -m "$TEMPERATURE"
  
  echo "Publié : humidité=$HUMIDITY%, distance=$DISTANCE cm, température=$TEMPERATURE°C"

  sleep 1
done