<!-- Ce fichier affiche les textes donnes a un traducteur ou editeur.
     Il permet de filtrer, trier et voir l'etat de chaque texte.
     Il contient le bouton pour marquer le travail comme termine. -->
<script setup>
import { toRef } from "vue";
import { useWorkerTableFilters } from "@/composables/useWorkerTableFilters";

const props = defineProps({
  items: { type: Array, required: true }, // Liste des textes avec infos de traduction/edition
  mode: { type: String, required: true }, // 'traduction' | 'edition'
  translationStatuses: { type: Object, default: () => ({}) },
  // object qui indique pour chaque texte si la traduction est disponible (pour edition) ou si le travail est termine (pour traduction)
});
const emit = defineEmits(["finish"]); // emit pour signaler que le travail sur un texte est termine (traduction ou edition)

const itemsRef = toRef(props, "items"); // ref reactive pour la liste des textes, utilise dans le composable de filtrage
const statusesRef = toRef(props, "translationStatuses"); // ref reactive pour les statuts de traduction, utilise dans le composable de filtrage

const { filters, filteredItems, toggleSort, clearFilter, setStatusFilter } =
  useWorkerTableFilters(itemsRef, props.mode, statusesRef);

function formatDate(dateStr) {
  if (!dateStr) return "";
  return new Date(dateStr).toLocaleDateString("fr-FR", {
    weekday: "long",
    day: "numeric",
    month: "long",
    year: "numeric",
    hour: "2-digit",
    minute: "2-digit",
  });
}
// fonction qui va retourner la deadline a afficher dans la colonne "Limite" 
function getDeadline(item) {
  return props.mode === "traduction" ? item.deadline : item.text?.deadline;
}

function isAvailable(item) {
  return props.mode !== "edition" || Boolean(props.translationStatuses[item.text?.id]);
}
</script>

<template>
  <div class="worker-texts-view">
    <p v-if="filteredItems.length === 0" class="no-data">
      Aucun texte ne vous est attribué.
    </p>

    <div v-else class="table-wrapper">
      <table class="worker-table">
        <thead>
          <tr>
            <th class="col-cote sortable" @click="toggleSort('cote')">Cote</th>
            <th class="col-title sortable" @click="toggleSort('title')">
              Titre
            </th>
            <th
              v-if="mode === 'edition'"
              class="col-status sortable"
              @click="toggleSort('available')"
            >
              Disponible
            </th>
            <th
              v-if="mode === 'traduction'"
              class="col-date sortable"
              @click="toggleSort('deadline')"
            >
              Limite
            </th>
            <th
              v-if="mode === 'traduction'"
              class="col-prec sortable"
              @click="toggleSort('precedent')"
            >
              Précédent
            </th>
            <th class="col-status sortable" @click="toggleSort('finished')">
              Terminé
            </th>
            <th
              v-if="mode === 'edition'"
              class="col-date sortable"
              @click="toggleSort('deadline')"
            >
              Limite
            </th>
            <th class="col-action"></th>
          </tr>

          <tr class="filter-row">
            <th>
              <div class="filter-cell">
                <input v-model="filters.cote" aria-label="Filtrer la cote" />
                <button @click="clearFilter('cote')">x</button>
              </div>
            </th>
            <th>
              <div class="filter-cell">
                <input v-model="filters.title" aria-label="Filtrer le titre" />
                <button @click="clearFilter('title')">x</button>
              </div>
            </th>
            <th v-if="mode === 'edition'">
              <div class="status-filter">
                <button
                  :class="{ active: filters.available === 'yes' }"
                  @click="setStatusFilter('available', 'yes')"
                >
                  ✓
                </button>
                <span>/</span>
                <button
                  :class="{ active: filters.available === 'no' }"
                  @click="setStatusFilter('available', 'no')"
                >
                  ✖
                </button>
                <button @click="clearFilter('available')">x</button>
              </div>
            </th>
            <th v-if="mode === 'traduction'"></th>
            <th v-if="mode === 'traduction'"></th>
            <th>
              <div class="status-filter">
                <button
                  :class="{ active: filters.finished === 'yes' }"
                  @click="setStatusFilter('finished', 'yes')"
                >
                  ✓
                </button>
                <span>/</span>
                <button
                  :class="{ active: filters.finished === 'no' }"
                  @click="setStatusFilter('finished', 'no')"
                >
                  ✖
                </button>
                <button @click="clearFilter('finished')">x</button>
              </div>
            </th>
            <th v-if="mode === 'edition'"></th>
            <th></th>
          </tr>
        </thead>

        <tbody>
          <tr v-for="item in filteredItems" :key="item.text?.id">
            <td>{{ item.text?.cote }}</td>
            <td class="title-cell">{{ item.text?.title }}</td>
            <td v-if="mode === 'edition'" class="status-cell">
              {{ isAvailable(item) ? "✓" : "✖" }}
            </td>
            <td v-if="mode === 'traduction'">
              {{ formatDate(item.deadline) }}
            </td>
            <td v-if="mode === 'traduction'">
              {{ item.text?.precedent || "" }}
            </td>
            <td class="status-cell">{{ item.finished ? "✓" : "✖" }}</td>
            <td v-if="mode === 'edition'">
              {{ formatDate(getDeadline(item)) }}
            </td>
            <td>
              <button
                v-if="!item.finished"
                class="btn-finish"
                :disabled="!isAvailable(item)"
                @click="emit('finish', item)"
              >
                <template v-if="isAvailable(item)">
                  Marquer<br />comme
                  {{ mode === "traduction" ? "traduit" : "terminé" }}
                </template>
                <template v-else>En attente</template>
              </button>
              <span v-else class="done-label">Terminé</span>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>

<style scoped>
@import "@/assets/worker-texts-table.css";
</style>
