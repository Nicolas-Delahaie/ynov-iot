const backend = new WebSocket(`ws://${window.location.hostname}:8080`);

backend.onopen = () => console.log("Connecté au serveur WebSocket");
backend.onmessage = (event) => {
  try {
    const data = JSON.parse(event.data);
    updateOrCreateCard(data.topic, data.message);
  } catch (error) {
    console.error("Erreur de parsing JSON :", error);
  }
};
backend.onclose = () => console.log("Déconnecté du serveur");

function updateOrCreateCard(topic, value) {
  let card = document.getElementById(`card-${topic}`);

  const lower = topic.toLowerCase();
  const mappings = {
    "capteur/humidity": { symbol: "%" },
    "capteur/temperature": { symbol: "°C" },
    "capteur/distance": { symbol: "cm" }
  };
  const { symbol = "" } = mappings[lower] || {};

  if (card) {
    card.querySelector(".value-text").textContent = `${Math.round(value * 10) / 10} ${symbol}`;
  } else {
    createCard(topic, value);
  }
}

function createCard(topic, value) {
  const card = Object.assign(document.createElement("div"), {
    className: "card",
    id: `card-${topic}`
  });

  const createElement = (tag, className, text) => {
    const el = document.createElement(tag);
    if (className) el.className = className;
    if (text) el.textContent = text;
    return el;
  };

  const lower = topic.toLowerCase();
  const mappings = {
    "capteur/humidity": { label: "Humidité", symbol: "%", img: "img/humidity.png" },
    "capteur/temperature": { label: "Température", symbol: "°C", img: "img/temperature.png" },
    "capteur/distance": { label: "Distance", symbol: "cm", img: "img/distance.png" }
  };

  const { label = "jsp", symbol = "", img = "img/error.png" } = mappings[lower] || {};

  card.appendChild(Object.assign(document.createElement("img"), { src: img }));
  card.appendChild(createElement("div", "topic-text", label));
  card.appendChild(createElement("div", "value-text", `${Math.round(value * 10) / 10} ${symbol}`)); // valeur suivie du symbole

  document.getElementById("output").appendChild(card);
}
