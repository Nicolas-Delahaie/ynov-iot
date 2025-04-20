const urlParams = new URLSearchParams(window.location.search);
const dashboardId = urlParams.get("dashboard_id");

if (!dashboardId) {
  console.error("Erreur : aucun dashboard spécifié dans l'URL");
} else {
  const iframe = document.getElementById("grafana-iframe");
  const dashboardUrl = `http://${window.location.hostname}:3000/public-dashboards/${dashboardId}`;
  iframe.src = dashboardUrl;
}

const backend = new WebSocket(`ws://${window.location.hostname}:8080`);
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
  const roundedValue = Math.round(value * 10) / 10;
  valueText.className = "value-text";
  valueText.textContent = `${roundedValue} ${symbol}`;

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
      const symbol = topicInformations.symbol;

      card.querySelector(
        ".value-text"
      ).textContent = `${roundedValue} ${symbol}`;
    } else {
      const newCard = createCard(topic, value, topicInformations);
      document.getElementById("output").appendChild(newCard);
    }
  } catch (error) {
    console.error("Erreur :", error);
  }
};
