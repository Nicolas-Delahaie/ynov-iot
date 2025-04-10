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

  if (card) {
    card.querySelector(".value-text").textContent = `Valeur : ${value}`;
  } else {
    createCard(topic, value);
  }
}

function createCard(topic, value) {
  const card = document.createElement("div");
  card.classList.add("card");
  card.id = `card-${topic}`;

  const img = document.createElement("img");
  img.src = "https://via.placeholder.com/100";

  const topicText = document.createElement("div");
  topicText.classList.add("topic-text");
  topicText.textContent = `Topic : ${topic}`;

  const valueText = document.createElement("div");
  valueText.classList.add("value-text");
  valueText.textContent = `Valeur : ${value}`;

  card.appendChild(img);
  card.appendChild(topicText);
  card.appendChild(valueText);

  document.getElementById("output").appendChild(card);
}
