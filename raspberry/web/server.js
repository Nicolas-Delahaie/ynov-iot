const WebSocket = require('ws');
const server = new WebSocket.Server({ port: 8080 });

server.on('connection', socket => {
    console.log('Un client est connecté');

    socket.on('message', message => {
        console.log(`Message reçu : ${message}`);
        // Répondre au client
        socket.send(`Serveur: ${message}`);
    });

    socket.on('close', () => {
        console.log('Un client s\'est déconnecté');
    });
});

// Fonction pour envoyer des messages à tous les clients connectés
function broadcastMessage() {
    const message = `Message automatique : ok.`;
    server.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message);
        }
    });
}

// Envoie un message toutes les 5 secondes
setInterval(broadcastMessage, 5000);
setInterval(test, 5000);

console.log('Serveur WebSocket démarré sur ws://localhost:8080');
