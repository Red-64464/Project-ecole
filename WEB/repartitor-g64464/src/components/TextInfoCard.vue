<!-- Ce fichier affiche les informations d'un texte.
     On y voit le titre, les dates, le nombre de mots et le statut.
     Il aide le repartiteur a comprendre le texte avant de l'attribuer. -->
<script setup>
defineProps({
  text: { type: Object, required: true },
});
// emit qui va permettre de signaler au parent que l'utilisateur veut voir le texte précédent
const emit = defineEmits(["go-precedent"]);

function formatDate(dateStr) {
  if (!dateStr) return "Non définie";
  return new Date(dateStr).toLocaleDateString("fr-FR", {
    weekday: "long",
    day: "numeric",
    month: "long",
    year: "numeric",
    hour: "2-digit",
    minute: "2-digit",
  });
}
</script>

<template>
  <section class="text-summary">
    <h2>Attribution</h2>
    <h3>{{ text.cote }} — {{ text.wordcount }} mots — {{ text.title }}</h3>

    <div class="deadline-box">
      <h4>Échéances</h4>
      <p><em>Reçu le</em><br />{{ formatDate(text.received) }}</p>
      <p><em>À rendre pour le</em><br />{{ formatDate(text.deadline) }}</p>
    </div>

    <button
      v-if="text.precedent"
      class="btn-link"
      @click="emit('go-precedent')"
    >
      Voir le texte précédent (ID: {{ text.precedent }})
    </button>
  </section>
</template>

<style scoped>
@import url("https://fonts.googleapis.com/css2?family=Audiowide&display=swap");

.text-summary {
  min-height: 14.3rem;
  background-color: #eaf6ff;
}

.text-summary h2 {
  margin: 0 0 1.1rem;
  padding: 0;
  border: 0;
  color: #000;
  font-family: "Audiowide", "Orbitron", "Trebuchet MS", Arial, sans-serif;
  font-size: clamp(2rem, 3.6vw, 3.1rem);
  font-weight: 400;
  line-height: 1;
  text-align: center;
}

.text-summary h3 {
  margin: 0;
  color: #000;
  font-size: 1.15rem;
  font-weight: 900;
}

.deadline-box {
  width: 9.7rem;
  margin: 1.55rem auto 0;
  padding: 0.9rem 0.8rem;
  border: 2px solid #6b72ad;
  border-radius: 4px;
  background-color: #eefafa;
  color: #000;
  box-shadow: 1px 1px 3px rgba(0, 0, 0, 0.15);
  text-align: center;
}

.deadline-box h4 {
  margin: 0 0 0.85rem;
  color: #2b2d78;
  font-size: 1.18rem;
  font-weight: 900;
}

.deadline-box p {
  margin: 0 0 0.75rem;
  font-size: 0.77rem;
  line-height: 1.05;
}
.deadline-box p:last-child {
  margin-bottom: 0;
}

.btn-link {
  display: block;
  margin: 1rem auto 0;
  border: 0;
  background: transparent;
  color: #2b2d78;
  cursor: pointer;
  font-weight: 700;
  text-decoration: underline;
}
</style>
