<!-- Ce fichier affiche tous les textes pour le repartiteur.
     Il permet de filtrer et trier la liste.
     Un clic sur un texte ouvre la page pour l'attribuer. -->
<script setup>
import { toRef } from "vue";
import { useTextTableFilters } from "@/composables/useTextTableFilters";

const props = defineProps({
  texts: { type: Array, required: true },
});
const emit = defineEmits(["select"]);
// On mets toRef pour créer une connexion avec le prop "texts" pour que si il change dans le parent , le prop enfant change aussi et on ne passe pas une copie qui reste fixe 
const textsRef = toRef(props, "texts");
const {
  filters, // objet qui contient les valeurs des filtres appliqués ( par exemple filters.cote pour le filtre de la cote )
  filteredTexts, // la liste des textes après application des filtres et du tri
  toggleSort, // fonction pour trier la colonne quand tu clique sur l'entête 
  clearFilter, // fonction qui efface un filtre spécifique
  setStatusFilter, // fonction pour filtrer par statut
  getTranslator, // fonction pour afficher le nom d'un taducteur d'un texte 
  isTranslated, // fonction pour savoir si un texte est traduit 
  getEditor, // fonction pour afficher le nom d'un éditeur d'un texte 
  isEdited, // fonction pour savoir si un texte est édité 
} = useTextTableFilters(textsRef);

// la fonction prend une date au format string et la formate pour l'affichage dans le tableau
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
</script>

<template>
  <div class="table-wrapper">
    <table class="texts-table">
      <thead>
        <tr>
          <th class="col-cote sortable" @click="toggleSort('cote')">Cote</th>
          <th class="col-title sortable" @click="toggleSort('title')">Titre</th>
          <th class="col-prec sortable" @click="toggleSort('precedent')">
            Précédent
          </th>
          <th class="col-date sortable" @click="toggleSort('received')">
            Reçu<br />le
          </th>
          <th class="col-date sortable" @click="toggleSort('deadline')">
            Limite
          </th>
          <th class="col-person sortable" @click="toggleSort('translator')">
            Traducteur
          </th>
          <th class="col-status sortable" @click="toggleSort('translated')">
            Traduit
          </th>
          <th class="col-person sortable" @click="toggleSort('editor')">
            Éditeur
          </th>
          <th class="col-status sortable" @click="toggleSort('edited')">
            Terminé
          </th>
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
          <th></th>
          <th></th>
          <th></th>
          <th>
            <div class="filter-cell">
              <input
                v-model="filters.translator"
                aria-label="Filtrer le traducteur"
              />
              <button @click="clearFilter('translator')">x</button>
            </div>
          </th>
          <th>
            <div class="status-filter">
              <button
                :class="{ active: filters.translated === 'yes' }"
                @click="setStatusFilter('translated', 'yes')"
              >
                ✓
              </button>
              <span>/</span>
              <button
                :class="{ active: filters.translated === 'no' }"
                @click="setStatusFilter('translated', 'no')"
              >
                ✖
              </button>
              <button @click="clearFilter('translated')">x</button>
            </div>
          </th>
          <th>
            <div class="filter-cell">
              <input v-model="filters.editor" aria-label="Filtrer l'éditeur" />
              <button @click="clearFilter('editor')">x</button>
            </div>
          </th>
          <th>
            <div class="status-filter">
              <button
                :class="{ active: filters.edited === 'yes' }"
                @click="setStatusFilter('edited', 'yes')"
              >
                ✓
              </button>
              <span>/</span>
              <button
                :class="{ active: filters.edited === 'no' }"
                @click="setStatusFilter('edited', 'no')"
              >
                ✖
              </button>
              <button @click="clearFilter('edited')">x</button>
            </div>
          </th>
        </tr>
      </thead>

      <tbody>
        <tr
          v-for="text in filteredTexts"
          :key="text.id"
          class="clickable-row"
          @click="emit('select', text.id)"
        >
          <td>{{ text.cote }}</td>
          <td class="title-cell">{{ text.title }}</td>
          <td>{{ text.precedent || "" }}</td>
          <td>{{ formatDate(text.received) }}</td>
          <td>{{ formatDate(text.deadline) }}</td>
          <td>{{ getTranslator(text) }}</td>
          <td class="status-cell">{{ isTranslated(text) ? "✓" : "✖" }}</td>
          <td>{{ getEditor(text) }}</td>
          <td class="status-cell">{{ isEdited(text) ? "✓" : "✖" }}</td>
        </tr>

        <tr v-if="filteredTexts.length === 0">
          <td colspan="9" class="no-data">Aucun texte trouvé</td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<style scoped>
@import "@/assets/texts-table.css";
</style>
