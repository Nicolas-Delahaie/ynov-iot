# Projet Arduino : Capteur DHT22 + LCD 16x2 + LED RGB

## Description du projet
Ce projet utilise un **Arduino Uno** pour mesurer la **température et l'humidité** via un **capteur DHT22**, afficher les valeurs sur un **écran LCD 16x2 en mode parallèle**, et changer la couleur d'une **LED RGB** en fonction du taux d'humidité.

🔹 **Capteur DHT22** → Mesure la température et l'humidité.  
🔹 **LCD 16x2 (mode parallèle)** → Affiche les données.  
🔹 **LED RGB** → Change de couleur en fonction de l'humidité :  
  - 🔴 **Rouge** : Air sec (< 30%)  
  - 🟢 **Vert** : Humidité normale (30% - 60%)  
  - 🔵 **Bleu** : Air humide (> 60%)  

## 🔗 Lien vers le code et le prototype (Schéma du montage) Wokwi
🔗 **Code source + Simulation Wokwi** : [Wokwi Projet](https://wokwi.com/projects/422783187973623809)

## 📌 Matériel nécessaire
- **Arduino Uno**
- **Capteur DHT22**
- **Écran LCD 16x2 (mode parallèle)**
- **Potentiomètre 10kΩ** (pour le contraste du LCD)
- **LED RGB (anode ou cathode commune)**
- **Résistances 220Ω** (pour la LED RGB)
- **Câbles et breadboard**
- **Raspberry**
  - écran, souris, clavier, antenne LoRa

## ⚠️ Problèmes possibles et solutions
### 1️⃣ **Le projet ne démarre pas immédiatement sur Wokwi**
- Wokwi peut afficher l’erreur **"Server Failed: Timeout"**.
- **Solution** : Redémarrer plusieurs fois la simulation.
- **Laisser tourner quelques secondes** pour que la plateforme initialise tout correctement.

