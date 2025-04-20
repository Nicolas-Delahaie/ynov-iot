# Projet IoT - Surveillance de l'humidité et du niveau d'eau pour une irrigation connectée

> **Équipe Hanse Mance Mans**  
> Nicolas DELAHAIE  
> Émile RICORDEL  
> Anas DAOUI  
> Clément ANDRIEU

## Présentation du projet

La présentation complète est disponible dans le document Word.

Ce projet IoT utilise **un capteur DHT11** pour mesurer l’humidité et la transmet via **un ESP32** à un **broker MQTT**. Un autre **ESP32** reçoit ensuite ces données et ajuste une **LED RGB** en fonction du niveau d’humidité :

- 🔴 **Rouge** : Air sec (< 30%)
- 🟢 **Vert** : Humidité normale (30% - 60%)
- 🔵 **Bleu** : Air humide (> 60%)

On utilise aussi **un capteur d'ultrasons** qui permet de mesurer une distance inférieure à 400 cm et la transmet via **un ESP32** à un **broker MQTT**. Un autre **ESP32** reçoit ensuite cette distance et allume soit une LED Rouge, soit une LED Bleue en fonction de la distance :

- 🔴 **LED Rouge** : Distance > 200 cm
- 🔵 **LED Bleue** : Distance < 200 cm

## Documentation

Les diagrammes sont trouvables dans le dossier `docs/`. Les fichiers avec l'extension `.drawio` peuvent être ouverts via l'extension `hediet.vscode-drawio` sur VSCode.

## Installation matérielle

Les différents éléments électroniques doivent être installés de la manière suivante : [schéma du montage électronique](Branchement.png)

Les différents éléments sont les suivants :

- 2 ESP32
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
  - Module d'alimentation

## Installation ESP32

Les codes `esp32/capteur.ino` et `esp32/actionneur.ino` sont à téléverser et exécuter respectivement sur l'ESP32 lié aux capteurs et sur l'ESP32 lié aux actionneurs.

Il est possible de les téléverser manuellement avec un câble micro USB, mais également via wifi (il faut être sur le même réseau).

En wifi, il faudra sélectionner :

- Capteur : `ESP32_SENDER_OTA`
- Actionneur : `ESP32_SUBSCRIBER_OTA`.

## Installation Raspberry Pi

Tout le code de la Raspberry se trouve dans le dossier `raspberry/`.

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

Pour récupérer le code dans la Raspberry, il faut installer Git dessus via :

```bash
sudo apt install git-all
```

Ensuite on doit cloner le projet avec :

```bash
git clone https://github.com/Nicolas-Delahaie/ynov-iot
```

### Lancer le serveur public

Une fois connecté à la Raspberry, placez-vous dans le dossier du projet, puis lancez :

```bash
sudo docker compose up -d --build
```

Cela démarre le serveur en arrière-plan via Docker.

### Restauration base de données

Une backup est disponible pour avoir accès aux données récupérées via MQTT.

Pour l'exécuter, se déplacer dans le dossier `raspberry/`, copier le dossier de backup à la racine puis l'exécuter via :

```bash
sudo docker compose cp influxdb/backup influxdb:/

sudo docker compose exec influxdb influx restore --full /backup
```

### Émulation

S'il n'y a pas d'ESP32 connecté, on peut utiliser un émulateur, qui va simuler des valeurs aléatoires. Pour l'activer, ajouter le flag `--profile emulation` avant `up`. Cela va activer le service esp32-emulator. Celui ci va envoyer des valeurs à l'infini toutes les secondes.

Sinon, simplement lancer le service via :

```bash
docker compose up -d --build esp32-emulator
```

## Utilisation

Les données sont alors disponibles sur Grafana, InfluxDB et le site web.

L'affichage des graphiques Grafana n'a pas pu être intégré de manière automatique pour l'instant. Ce n'est qu'un POC pour l'instant. Pour activer son affichage, il faut aller sur Grafana > Dashboard souhaité > Share ... > Share externally > Copy external link puis copier L'ID de ce lien dans le parametre d'url dashboard_id. Par exemple :

1. Récupérer l'url : `http://localhost:3000/public-dashboards/1b318f9f630a4fd2b5d9b9c5aa425db1`
2. Extraire l'id `1b318f9f630a4fd2b5d9b9c5aa425db1`
3. Intégrer l'id dans `dashboard_id` : `https://localhost/?dashboard_id=1b318f9f630a4fd2b5d9b9c5aa425db1`.

## Problèmes rencontrés

**Si un problème survient, vérifier en premier :**

- Le WiFi fonctionne ?
- Le Broker MQTT reçoit bien les messages ?
- Les ESP sont bien abonnés et publient les bonnes valeurs ?
