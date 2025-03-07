import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    # Débogage lors de la connexion
    print(f"Connexion au broker, code de retour : {rc}")
    if rc == 0:
        print("Connecté avec succès au broker MQTT.")
        client.subscribe("capteur/humidity")
    else:
        print(f"Échec de la connexion, code de retour : {rc}. Cela peut être dû à un problème de réseau ou de configuration du broker.")

def on_message(client, userdata, msg):
    # Débogage lors de la réception de message
    print(f"Message reçu sur le sujet {msg.topic}.")
    try:
        payload = msg.payload.decode()
        print(f"Contenu du message : {payload}")
    except Exception as e:
        print(f"Erreur lors du décodage du message : {e}")

# Fonction de gestion des erreurs lors de la tentative de connexion
def on_disconnect(client, userdata, rc):
    if rc != 0:
        print(f"Déconnexion inattendue du broker, code de retour : {rc}")
    else:
        print("Déconnexion propre du broker.")

# Configuration du client MQTT
BROKER = "192.168.137.56"  # Remplace par l'adresse de ton broker
PORT = 1883

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

# Débogage avant de se connecter
print(f"Connexion au broker {BROKER} sur le port {PORT}...")

try:
    client.connect(BROKER, PORT, 60)
except Exception as e:
    print(f"Erreur lors de la tentative de connexion au broker : {e}")
    exit(1)  # Arrêter le programme si la connexion échoue

# Boucle pour écouter les messages
print("Lancement de la boucle d'écoute des messages...")
client.loop_forever()
