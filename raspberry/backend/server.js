const WebSocket = require("ws");
const server = new WebSocket.Server({ host: "0.0.0.0", port: 8080 });

server.on("connection", (socket) => {
  console.log("Un client est connecté");

  socket.on("message", (message) => {
    console.log(`Message reçu : ${message}`);
    
    let parsedMessage;
    try {
        parsedMessage = JSON.parse(message);
    } catch (error) {
        console.error("Erreur de parsing du message reçu :", error);
        return;
    }
    
    const formattedMessage = JSON.stringify({
        topic: parsedMessage.topic,
        message: parsedMessage.message
    });

    server.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(formattedMessage);
        }
    });
  });

  socket.on("close", () => {
    console.log("Un client s'est déconnecté");
  });
});

console.log("Serveur WebSocket démarré sur ws://localhost:8080");
