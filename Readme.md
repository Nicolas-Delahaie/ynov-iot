# Projet Arduino : agriculture autonome

> **Équipe Hanse Mance Mans**  
> Nicolas DELAHAIE  
> Émile RICORDEL  
> Anas DAOUI  
> Clément ANDRIEU

## Description du projet

[Document de présentation du projet (Word)](https://auvencecom-my.sharepoint.com/:w:/g/personal/nicolas_delahaie_ynov_com/EWxNXPk6Hf5GhAFUDsCuzskBGGFIuqWOAZh5HHxPKcpJHA?e=mTtp2u)

Ce projet utilise un **Arduino Uno** pour mesurer la **température et l'humidité** via un **capteur DHT11**, afficher les valeurs sur un **écran LCD 16x2 en mode parallèle**, et changer la couleur d'une **LED RGB** en fonction du taux d'humidité :

- 🔴 **Rouge** : Air sec (< 30%)
- 🟢 **Vert** : Humidité normale (30% - 60%)
- 🔵 **Bleu** : Air humide (> 60%)

Pour l'instant, les LEDs peuvent être allumées ou éteintes manuellement via les boutons correspondants sur la télécommande, comme on peut le voir sur cette [vidéo de démonstration](demonstration.mp4).

### Documentation

Les diagrammes sont trouvables dans le dossier `docs/`. Les fichiers avec l'extension `.drawio` peuvent être ouverts via l'extension `hediet.vscode-drawio` sur VSCode.

## Matériel utilisé

[Schema de l'installation](schema_arduino.png)

- **Câbles et breadboard**
- **Arduino Uno** avec ses équipements :
  - Capteurs :
    - DHT11 (humidité et température)
  - Pilotage :
    - Recepteur infrarouge (IRReceiver)
    - Télécommande
  - Affichage :
    - Écran LCD 16x2 (mode parallèle)
    - Potentiomètre 10kΩ (pour le contraste du LCD)
  - Éclairage :
    - 1 LED RGB (anode ou cathode commune)
    - 1 LED rouge
    - 1 LED bleue
    - 2 résistances 220Ω
- **Raspberry** avec ses équipements :
  - Écran HDMI
  - Souris
  - Clavier
  - Antenne LoRa

## Installation

Le code source est présent sur la plateforme en ligne Wokwi, qui permet de simuler le circuit electronique et de coder directement dedans. [Projet Wokwi](https://wokwi.com/projects/422783187973623809)

Lorsque le code est bon, il faut ensuite le copier sur la carte physique.

## Problèmes rencontrés et solutions

### Le projet ne démarre pas immédiatement sur Wokwi

- **Erreur :** Wokwi peut afficher l’erreur `Server Failed: Timeout`.
- **Solutions :**
  - Redémarrer plusieurs fois la simulation.
  - Laisser tourner quelques secondes pour que la plateforme initialise tout correctement.
