const WebSocket = require("ws");
const server = new WebSocket.Server({ host: "0.0.0.0", port: 80 });

server.on("connection", (socket) => {
  console.log("Un client est connecté");

  socket.on("message", (message) => {
    console.log(`Message reçu : ${message}`);
    // Répondre au client
    socket.send(`Serveur: ${message}`);
  });

  socket.on("close", () => {
    console.log("Un client s'est déconnecté");
  });
});

console.log("Serveur WebSocket démarré sur ws://localhost:80");
