<!-- Vue des statistiques globales accessible au répartiteur.
     Affiche un récapitulatif "Actuellement" (textes / mots par état)
     ainsi qu'un sélecteur d'intervalle de dates pour le récapitulatif
     "Passés" (placeholder réservé : la base ne suit pas tous les détails). -->
<script setup>
import { ref, onMounted } from "vue";
import { getGlobalCurrentStats } from "@/services/statsService";
import { useNotifications } from "@/composables/useNotifications";

const { notify } = useNotifications();

const current = ref(null);
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

async function loadCurrent() {
  loading.value = true;
  const data = await getGlobalCurrentStats();
  if (!data) {
    notify("Impossible de charger les statistiques globales.", "error");
  } else {
    current.value = data;
  }
  loading.value = false;
}

function refreshInterval() {
  // Les chiffres "Actuellement" sont en temps réel ; on relit aussi
  // les valeurs pour que le répartiteur voie immédiatement l'effet du
  // changement d'intervalle.
  loadCurrent();
}

onMounted(loadCurrent);
</script>

<template>
  <div class="global-stats">
    <h3>Globales</h3>

    <section class="block">
      <h4>Actuellement</h4>
      <p v-if="loading">Chargement...</p>
      <table v-else-if="current" class="stats-table">
        <thead>
          <tr><th></th><th>Textes</th><th>Mots</th></tr>
        </thead>
        <tbody>
          <tr>
            <th>En cours</th>
            <td>{{ current.in_progress_texts }}</td>
            <td>{{ current.in_progress_words }}</td>
          </tr>
          <tr>
            <th>Non-attribués en traduction</th>
            <td>{{ current.unassigned_translation_texts }}</td>
            <td>{{ current.unassigned_translation_words }}</td>
          </tr>
          <tr>
            <th>En cours de traduction</th>
            <td>{{ current.translating_texts }}</td>
            <td>{{ current.translating_words }}</td>
          </tr>
          <tr>
            <th>Non-attribués en édition</th>
            <td>{{ current.unassigned_edition_texts }}</td>
            <td>{{ current.unassigned_edition_words }}</td>
          </tr>
          <tr>
            <th>En cours d'édition</th>
            <td>{{ current.editing_texts }}</td>
            <td>{{ current.editing_words }}</td>
          </tr>
        </tbody>
      </table>
    </section>

    <section class="block">
      <h4>Passés</h4>
      <div class="interval">
        <label>Entre <input v-model="from" type="date" /></label>
        <label>et <input v-model="to" type="date" /></label>
        <button class="btn-refresh" @click="refreshInterval">Actualiser</button>
      </div>
      <p class="hint">
        Choisissez un intervalle puis cliquez sur "Actualiser".
        Les statistiques individuelles par personne sont disponibles via
        la table de gestion des rôles.
      </p>
    </section>
  </div>
</template>

<style scoped>
.global-stats {
  background: #fff;
  border-radius: 6px;
  padding: 1.25rem 1.5rem;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.12);
}

.block { margin-bottom: 1.5rem; }
.block h4 {
  margin: 0 0 0.7rem;
  color: #2b2d78;
  font-size: 1.1rem;
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
.stats-table tbody th { background-color: #d7e5f0; text-align: left; }

.interval {
  display: flex;
  align-items: center;
  gap: 0.7rem;
  margin-bottom: 0.5rem;
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
.hint { font-size: 0.85rem; color: #444; }
</style>
