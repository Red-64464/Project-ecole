<!-- Statistiques d'un traducteur (vue partagée par le traducteur lui-même
     et par le répartiteur consultant les stats d'autrui).
     L'identité est lue dans l'URL (/stats/translator/:id). -->
<script setup>
import { ref, onMounted, watch } from "vue";
import { useRoute } from "vue-router";
import {
  getTranslatorStats,
  getTranslatorCurrent,
} from "@/services/statsService";
import { getTranslatorById } from "@/services/translatorService";
import { useNotifications } from "@/composables/useNotifications";

const route = useRoute();
const { notify } = useNotifications();

const translator = ref(null);
const current = ref({ texts: 0, words: 0 });
const history = ref({ attributed: 0, finished: 0, late: 0, words: 0 });
const loading = ref(true);
const from = ref(defaultFrom());
const to = ref(defaultTo());

function defaultFrom() {
  const date = new Date();
  date.setMonth(date.getMonth() - 1);
  return date.toISOString().slice(0, 10);
}

function defaultTo() {
  return new Date().toISOString().slice(0, 10);
}

async function load() {
  loading.value = true;
  const id = route.params.id;

  const person = await getTranslatorById(id);
  if (!person) {
    notify(`Traducteur ${id} introuvable`, "error");
    loading.value = false;
    return;
  }
  translator.value = person;

  current.value = await getTranslatorCurrent(id);
  history.value = await getTranslatorStats(
    id,
    `${from.value}T00:00:00`,
    `${to.value}T23:59:59`,
  );
  loading.value = false;
}

function refresh() {
  load();
}

onMounted(load);
watch(() => route.params.id, load);
</script>

<template>
  <div v-if="loading" class="loading">Chargement...</div>
  <div v-else-if="translator" class="translator-stats">
    <h3>
      Pour le traducteur :
      {{ translator.firstname }} {{ translator.lastname }} ({{ translator.id }})
    </h3>

    <section class="block">
      <h4>En cours</h4>
      <table class="stats-table">
        <thead>
          <tr><th>Textes en cours</th><th>Mots en main</th></tr>
        </thead>
        <tbody>
          <tr><td>{{ current.texts }}</td><td>{{ current.words }}</td></tr>
        </tbody>
      </table>
    </section>

    <section class="block">
      <h4>Passés</h4>
      <div class="interval">
        <label>Entre <input v-model="from" type="date" /></label>
        <label>et <input v-model="to" type="date" /></label>
        <button class="btn-refresh" @click="refresh">Actualiser</button>
      </div>
      <table class="stats-table">
        <thead>
          <tr><th>Attribués</th><th>Traduits</th><th>Dont rendus en retard</th><th>Mots</th></tr>
        </thead>
        <tbody>
          <tr>
            <td>{{ history.attributed }}</td>
            <td>{{ history.finished }}</td>
            <td>{{ history.late }}</td>
            <td>{{ history.words }}</td>
          </tr>
        </tbody>
      </table>
    </section>
  </div>
</template>

<style scoped>
.translator-stats {
  background: #fff;
  border-radius: 6px;
  padding: 1.25rem 1.5rem;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.12);
}

.translator-stats h3 {
  margin: 0 0 1rem;
  color: #2b2d78;
  font-size: 1.1rem;
  font-weight: 800;
}

.block { margin-bottom: 1.5rem; }
.block h4 {
  margin: 0 0 0.7rem;
  color: #2b2d78;
  font-size: 1rem;
  font-weight: 800;
}

.stats-table {
  width: 100%;
  border-collapse: collapse;
  font-size: 0.9rem;
}
.stats-table th, .stats-table td {
  padding: 0.4rem 0.7rem;
  border: 1px solid #6b72ad;
  text-align: center;
}
.stats-table thead th { background-color: #2b2d78; color: #fff; }

.interval {
  display: flex;
  align-items: center;
  gap: 0.7rem;
  margin-bottom: 0.7rem;
  flex-wrap: wrap;
}
.interval input {
  border: 1px solid #99a6b5;
  padding: 0.25rem 0.4rem;
}
.btn-refresh {
  border: 1px solid #8a4f00;
  border-radius: 3px;
  background-color: #ffa500;
  color: #000;
  padding: 0.35rem 0.7rem;
  cursor: pointer;
  font-weight: 700;
}
.loading { padding: 1rem; }
</style>
