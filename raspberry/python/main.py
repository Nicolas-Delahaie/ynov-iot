import paho.mqtt.client as mqtt
import websocket
import json
import time

# Configuration du broker MQTT
BROKER = "mosquitto"
PORT = 1883

# Configuration du serveur WebSocket
WS_SERVER = "ws://backend:8080"

def connect_websocket():
    while True:
        try:
            ws = websocket.create_connection(WS_SERVER)
            print("Connecté au serveur WebSocket.")
            return ws
        except Exception as e:
            print(f"Erreur lors de la connexion au serveur WebSocket : {e}. Nouvelle tentative dans 2 secondes...")
            time.sleep(2)

ws_client = connect_websocket()

def on_connect(client, userdata, flags, rc):
    print(f"Connexion au broker, code de retour : {rc}")
    if rc == 0:
        print("Connecté avec succès au broker MQTT.")
        client.subscribe("capteur/humidity")
        client.subscribe("capteur/temperature")
    else:
        print(f"Échec de la connexion, code de retour : {rc}")

def on_message(client, userdata, msg):
    print(f"Message reçu sur {msg.topic} : {msg.payload.decode()}")
    data = {
        "topic": msg.topic,
        "message": msg.payload.decode()
    }
    global ws_client
    if ws_client:
        try:
            ws_client.send(json.dumps(data))
            print("Message envoyé au serveur WebSocket.")
        except Exception as e:
            print(f"Erreur lors de l'envoi du message WebSocket : {e}. Tentative de reconnexion...")
            ws_client = connect_websocket()

def on_disconnect(client, userdata, rc):
    if rc != 0:
        print(f"Déconnexion inattendue du broker, code de retour : {rc}")
    else:
        print("Déconnexion propre du broker.")

# Configuration du client MQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

print(f"Connexion au broker {BROKER} sur le port {PORT}...")
try:
    client.connect(BROKER, PORT, 60)
except Exception as e:
    print(f"Erreur lors de la connexion au broker : {e}")
    exit(1)

print("Lancement de la boucle d'écoute des messages...")
client.loop_forever()
