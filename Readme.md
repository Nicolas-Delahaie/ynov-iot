# Projet Arduino : Capteur DHT11 + LCD 16x2 + LED RGB + LED MONOCOULEUR

## Description du projet
Ce projet utilise un **Arduino Uno** pour mesurer la **température et l'humidité** via un **capteur DHT11**, afficher les valeurs sur un **écran LCD 16x2 en mode parallèle**, et changer la couleur d'une **LED RGB** en fonction du taux d'humidité.

🔹 **Capteur DHT11** → Mesure la température et l'humidité.  
🔹 **LCD 16x2 (mode parallèle)** → Affiche les données.  
🔹 **LED RGB** → Change de couleur en fonction de l'humidité :  
  - 🔴 **Rouge** : Air sec (< 30%)  
  - 🟢 **Vert** : Humidité normale (30% - 60%)  
  - 🔵 **Bleu** : Air humide (> 60%)
  - **LED Bleu**
  - **LED Rouge**

## 🔗 Lien vers le code et le prototype (Schéma du montage) Wokwi
🔗 **Code source + Simulation Wokwi** : [Wokwi Projet](https://wokwi.com/projects/422783187973623809)

## 📌 Matériel nécessaire
- **Arduino Uno**
- **Capteur DHT11**
- **Écran LCD 16x2 (mode parallèle)**
- **Potentiomètre 10kΩ** (pour le contraste du LCD)
- **LED RGB (anode ou cathode commune)**
- **2 LED Rouge et Bleu**
- **IRReceiver et sa télécommande**
- **Résistances 220Ω** (pour la LED RGB)
- **Câbles et breadboard**
- **Raspberry**
  - écran, souris, clavier, antenne LoRa
 
## Vidéo de présentation des branchements réalisés
- **LED RGB à gauche qui est lié au taux d'humidité (Rouge, Vert et Bleu)**
- **2 LED de couleurs qui réagissent lorsqu'on appuie sur le boutton 1 ou 2 de la télécommande**

## ⚠️ Problèmes possibles et solutions
### 1️⃣ **Le projet ne démarre pas immédiatement sur Wokwi**
- Wokwi peut afficher l’erreur **"Server Failed: Timeout"**.
- **Solution** : Redémarrer plusieurs fois la simulation.
- **Laisser tourner quelques secondes** pour que la plateforme initialise tout correctement.

