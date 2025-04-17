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
const mappings = {
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
backend.onmessage = (event) => {
  try {
    const data = JSON.parse(event.data);
    updateOrCreateCard(data.topic, data.value);
  } catch (error) {
    console.error("Erreur de parsing JSON :", error);
  }
};
backend.onclose = () => console.log("Déconnecté du serveur");

function updateOrCreateCard(topic, value) {
  let card = document.getElementById(`card-${topic}`);

  const lower = topic.toLowerCase();
  const roundedValue = Math.round(value * 10) / 10;
  const symbol = mappings[lower]?.symbol;

  if (card) {
    card.querySelector(".value-text").textContent = `${roundedValue} ${symbol}`;
  } else {
    createCard(topic, roundedValue);
  }
}

function createCard(topic, value) {
  const card = Object.assign(document.createElement("div"), {
    className: "card",
    id: `card-${topic}`,
  });

  const createElement = (tag, className, text) => {
    const el = document.createElement(tag);
    if (className) el.className = className;
    if (text) el.textContent = text;
    return el;
  };

  const lower = topic.toLowerCase();

  const { label, symbol, img } = mappings[lower] ?? {
    img: "img/error.png",
    symbol: "",
    label: "Erreur",
  };

  card.appendChild(Object.assign(document.createElement("img"), { src: img }));
  card.appendChild(createElement("div", "topic-text", label));
  card.appendChild(
    createElement(
      "div",
      "value-text",
      `${Math.round(value * 10) / 10} ${symbol}`
    )
  );

  document.getElementById("output").appendChild(card);
}
