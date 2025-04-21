const host = window.location.hostname;
const iframe = document.getElementById("grafana-iframe");

// Fonction pour charger un dashboard directement dans l'iframe
function loadDashboard(uid) {
  iframe.src = `http://${host}:3000/public-dashboards/${uid}`;
}

const urlParams = new URLSearchParams(window.location.search);
const dashboardId = urlParams.get("dashboard_id");

if (dashboardId) {
  loadDashboard(dashboardId);
} else {
  fetch(`http://${host}:3001/api/dashboards`)
    .then((response) => {
      if (!response.ok)
        throw new Error(`Erreur API Backend : ${response.status}`);
      return response.json();
    })
    .then((dashboards) => {
      if (dashboards.length > 0) {
        loadDashboard(dashboards[0].uid);
      } else {
        console.error("Aucun dashboard disponible.");
      }
    })
    .catch(console.error);
}

const backend = new WebSocket(`ws://${host}:8080`);

const createCard = (topic, value, topicInformations) => {
  const { label, symbol, img } = topicInformations;

  const card = document.createElement("div");
  card.className = "card";
  card.id = `card-${topic}`;

  const image = document.createElement("img");
  image.src = img;

  const topicText = document.createElement("div");
  topicText.className = "topic-text";
  topicText.textContent = label;

  const valueText = document.createElement("div");
  valueText.className = "value-text";
  valueText.textContent = `${Math.round(value * 10) / 10} ${symbol}`;

  card.appendChild(image);
  card.appendChild(topicText);
  card.appendChild(valueText);

  return card;
};

const topicsInformations = {
  "capteur/humidity": {
    label: "Humidité",
    symbol: "%",
    img: "img/humidity.png",
  },
  "capteur/temperature": {
    label: "Température",
    symbol: "°C",
    img: "img/temperature.png",
  },
  "capteur/distance": {
    label: "Distance",
    symbol: "cm",
    img: "img/distance.png",
  },
};

backend.onopen = () => console.log("Connecté au serveur WebSocket");
backend.onclose = () => console.log("Déconnecté du serveur");
backend.onmessage = (event) => {
  try {
    const { topic, value } = JSON.parse(event.data);
    let card = document.getElementById(`card-${topic}`);
    const topicInformations = topicsInformations[topic.toLowerCase()];

    if (!topicInformations) {
      console.error(`Erreur : aucun mapping trouvé pour le topic ${topic}`);
      return;
    }

    if (card) {
      const roundedValue = Math.round(value * 10) / 10;
      card.querySelector(
        ".value-text"
      ).textContent = `${roundedValue} ${topicInformations.symbol}`;
    } else {
      const newCard = createCard(topic, value, topicInformations);
      document.getElementById("output").appendChild(newCard);
    }
  } catch (error) {
    console.error("Erreur WebSocket :", error);
  }
};
