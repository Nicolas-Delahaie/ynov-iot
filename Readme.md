# Projet IoT - Surveillance d’Humidité avec MQTT

> **Équipe Hanse Mance Mans**  
> Nicolas DELAHAIE  
> Émile RICORDEL  
> Anas DAOUI  
> Clément ANDRIEU

## Présentation du projet

[Document de présentation du projet (Word)](https://auvencecom-my.sharepoint.com/:w:/g/personal/nicolas_delahaie_ynov_com/EWxNXPk6Hf5GhAFUDsCuzskBGGFIuqWOAZh5HHxPKcpJHA?e=mTtp2u)

Ce projet IoT utilise **un capteur DHT11** pour mesurer l’humidité et la transmet via **un ESP32** à un **broker MQTT**. Un autre **ESP32** reçoit ensuite ces données et ajuste une **LED RGB** en fonction du niveau d’humidité :

- 🔴 **Rouge** : Air sec (< 30%)
- 🟢 **Vert** : Humidité normale (30% - 60%)
- 🔵 **Bleu** : Air humide (> 60%)

On utilise aussi **un capteur d'ultrasons** qui permet de mesurer une distance inférieure à 400 cm et la transmet via **un ESP32** à un **broker MQTT**. Un autre **ESP32** reçoit ensuite cette distance et allume soit une LED Rouge, soit une LED Bleue en fonction de la distance :

- 🔴 **LED Rouge** : Distance > 200 cm
- 🔵 **LED Bleue** : Distance < 200 cm

### Documentation

Les diagrammes sont trouvables dans le dossier `docs/`. Les fichiers avec l'extension `.drawio` peuvent être ouverts via l'extension `hediet.vscode-drawio` sur VSCode.

## Matériel utilisé

[Schema de l'installation](Branchement.png)

- **Câbles et breadboard**
- **2 ESP32**
  - Capteurs :
    - DHT11 (humidité et température)
    - Capteur d'ultrasons
  - Éclairage :
    - 1 LED RGB (anode ou cathode commune)
    - 1 LED rouge
    - 1 LED bleue
    - 5 résistances 220Ω
  - Alimentation :
    - Pile 9V
    - Module ...
- **Raspberry**

## Fonctionnement

### Communication entre les composants

1. Une ESP32 mesure **l’humidité** et **la température** via le capteur **DHT11**.
2. Il envoie les valeurs au **broker MQTT** présent sur le Raspberry via WiFi.
3. Une autre ESP32 **s’abonne** aux données et active les différentes LEDS selon les valeurs.

## Installation logicielle ESP32 (capteurs)

- Le code CodeRecepteur.ino est à téléverser sur l'ESP32 relié aux capteurs.
- Le code CodeActionneur.ino est à téléverser sur l'ESP32 relié aux LEDS.

## Installation Raspberry Pi (serveur public)

Tout le code de la Raspberry se trouve dans le dossier `/raspberry`.

### Allumage

- Lors de l’allumage, la LED **verte** doit clignoter de manière **irrégulière** pendant le boot : cela indique que le système fonctionne normalement.
- Si la LED **rouge** est allumée ou reste fixe, cela signifie que l’alimentation est correcte.
- Si la LED **verte reste figée**, cela peut indiquer un problème avec la carte SD. Vérifiez qu’elle est bien flashée.

### Configuration réseau

1. Allumez la Raspberry Pi.
2. Connectez-la au réseau souhaité.
3. Attribuez un nom à la Raspberry pour éviter d’utiliser son adresse IP :

   ```bash
   sudo nmtui
   ```

   → Allez dans **Set system hostname**, puis définissez un nom comme `rasp`.

### Connexion SSH

Assurez-vous que votre ordinateur est connecté au **même réseau local** que la Raspberry puis se connecter via :

```bash
ssh rasp@rasp.local
```

Il faudra ensuite saisir le mot de passe. Pour ne pas avoir à ressaisir ce dernier à chaque connexion SSH, on peut s'enregistrer une seule fois via :

```bash
ssh-copy-id rasp@rasp.local
```

Cela enregistre votre clé publique dans la Raspberry et active la connexion SSH sans mot de passe.

### Déploiement du projet

Pour récupérer le code dans la Raspberry, il faut installer Git dessus via : `sudo apt install git-all`.

Ensuite on doit cloner le projet avec : `git clone https://github.com/Nicolas-Delahaie/ynov-iot`.

### Lancer le serveur public

Une fois connecté à la Raspberry, placez-vous dans le dossier du projet, puis lancez :

```bash
sudo docker compose up -d --build
```

Cela démarre le serveur en arrière-plan via Docker.

### Configuration services

#### Restauration backup Influxdb

Une backup est disponible pour avoir accès aux données récupérées via MQTT.

Pour l'exécuter, se déplacer dans le dossier `raspberry/`, copier le dossier de backup à la racine puis l'exécuter via :

```bash

sudo docker compose cp influxdb/backup influxdb:/
sudo docker compose exec influxdb influx restore /backup

```

## Problèmes rencontrés

**Si un problème survient, vérifier en premier :**

- Le WiFi fonctionne ?
- Le Broker MQTT reçoit bien les messages ?
- Les ESP sont bien abonnés et publient les bonnes valeurs ?
