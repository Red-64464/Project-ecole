<script setup>
/**
 * Composant de tableau générique pour afficher des listes de personnes.
 *
 * Ce composant permet d'afficher, filtrer, trier et sélectionner des individus
 * (traducteurs ou éditeurs) à partir d'un ensemble de colonnes configurables.
 *
 * @component
 */
import { ref, computed } from "vue";

const props = defineProps({
  columns: {
    // Le nom et la liste des colonnes
    type: Array, // Il recoit un tableau d'objets []
    required: true, // obligatoire
  },
  data: {
    // Liste de personnes a afficher
    type: Array,
    required: true,
  },
  selectLabel: {
    // Le texte affiché sur chauqe bouton
    type: String,
    default: "Choisir",
  },
  showSelect: {
    // Afficher ou cacher le bouton " choisir "
    type: Boolean,
    default: true,
  },
});

const emit = defineEmits(["select"]);
// On déifnis un emit qui s'apelle select , afin que le composant puisse communiquer avec le parent  et lui envoyer la personne qui a été sélectioné

// ref est ce qu'on apelle une variable réactive , c'est a dire que quand sa valeur change , le composant se met a jour automatiquement pour refléter ce changement
const filters = ref({}); // Va contenir le filtre de chaque colonne , c'est un objet ou la clé est le nom de la colonne et la valeur est le texte de filtrage saisi par l'utilisateur
const sortKey = ref(""); // La colonne actuellement utilisée pour le tri , c'est une chaîne de caractères qui correspond à la clé de la colonne
const sortAsc = ref(true); // On tre dans l'ordre croissant ou décroissant

const filteredData = computed(() => {
  // C'est une variable computed ce qui veut dire que elle est recalculée automatiquement a chaque fois que
  let result = [...props.data]; // on fait une copie du tableaux de personnes recu ne prop

  for (const col of props.columns) {
    // Je boucle sur chaque colonne
    const filterValue = (filters.value[col.key] || "").toLowerCase(); // Je récupère le texte de ce qui a été écris dan sl'input de recherche de ma colonne depui sla liste de filters
    if (filterValue) {
      // Si il y a un texte de filtrage
      result = result.filter((row) => {
        // Dans le tableau qui contient toutes les personnes , je ne garde que celles qui correspondent au filtre de la colonne en cours
        // row = on examine chaque ligne du tableau , une par un e
        const cellValue = String(row[col.key] || "").toLowerCase(); // On regarde a valeur qui est contenue dans notre ligne pour la colonne en cours
        // exemple : row = { name: "Alice", age: 30 } et col.key = "name" => cellValue = "alice"
        return cellValue.includes(filterValue); // Si la valeur qu'on a récupéré  contient  le filtre alors on garde cette ligne , sinon on la supprime du résultat
      });
    }
  }

  // La partie du code qiu s'occupe de trier  les lignes d'une colonne  dans l'ordre croissant ou décroissant
  if (sortKey.value) {
    // Si on a cliqué sur une colonne  donc si sortKey a une valeur , alors on lance le tri sinon on ne fait rien
    result.sort((a, b) => {
      // Le programme prends deux lignes au hasard et regarde leur contenu
      const valA = a[sortKey.value] ?? ""; // ?? " " , veut dire que si a[sortKey.value] est undefined ou null , alors on lui donne la valeur de " " pour éviter les erreurs de comparaison
      const valB = b[sortKey.value] ?? "";

      // Cas numéro 1 : se sont des nombres
      if (typeof valA === "number" && typeof valB === "number") {
        return sortAsc.value ? valA - valB : valB - valA; // Si on est dans l'ordre croissant , alots on soustrait pour savoir lequel est le plus grand
        // ordre croissant = 1 - 2 = -1 => il mettras la ligne a avant b car on retourne un nombre négatif
        // ordre décroissant = 2 - 1 = 1 => il mettras la ligne b avant a car on retourne un nombre positif
      }

      // Cas 2 ce sont des string
      // on va utiliser l'outil localCompare qui compare deux chaines de caractères et retourne un nombre négatif si la première chaine est avant la deuxième dans l'ordre alphabétique , un nombre positif si elle est après et 0 si elles sont égales
      const cmp = String(valA)
        .toLowerCase()
        .localeCompare(String(valB).toLowerCase()); // Si on est dans l'ordre croissant , on retourne cmp tel quel exemple :  "apple".localeCompare("banana") => cmp = -1 => il mettras apple avant banana
      // sinon on inverse le résultat de cmp pour inverser l'ordre de tri exemple : "apple".localeCompare("banana") => cmp = -1 => en ordre décroissant on retourne 1 => il mettras banana avant apple
      return sortAsc.value ? cmp : -cmp; // Ajout du retour de la comparaison
    });
  }

  // on retoune le résultat final qui est la liste des personnes filtrée et triée selon les critères choisis par l'utilisateur
  return result;
});

// fonction qui gère le tri lorsque l'utilisateur clique sur une colonne , elle prend en paramètre la clé de la colonne sur laquelle on a cliqué
function toggleSort(key) {
  if (sortKey.value === key) {
    // Si on clique sur la même colonne que celle qui est déjà triée , alors on inverse l'ordre de tri
    if (sortAsc.value) {
      // Si on était dans l'ordre croissant , alors on passe en décroissant
      sortAsc.value = false;
    } else {
      // Si on est en décroissant , alors on réinitalise la sortKey pour forcer le tri a se refaire même si on clique sur la même colonne que celle qui est déjà triée et on remet l'ordre de tri en croissant par défaut
      sortKey.value = ""; // on la réinitialise pour forcer le tri a se refaire même si on clique sur la même colonne que celle qui est déjà triée
      sortAsc.value = true; // on remet l'ordre de tri en croissant par défaut
    }
  } else {
    // Si on clique sur une colonne différente de celle qui est déjà triée , alors on change la sortKey pour trier par cette nouvelle colonne et on remet l'ordre de tri en croissant par défaut
    sortKey.value = key;
    sortAsc.value = true;
  }
}

function clearFilter(key) {
  // on prend en paramètre la clé de la colonne pour laquelle on veut effacer le filtre
  filters.value[key] = ""; // on réinitialise le filtre de cette colonne en lui donnant une chaîne vide , ce qui aura pour effet de supprimer le filtrage pour cette colonne et d'afficher toutes les lignes
}
</script>

<template>
  <div class="person-table-wrapper">
    <table class="person-table">
      <thead>
        <tr>
          <th
            v-for="col in columns"
            :key="col.key"
            class="sortable"
            @click="toggleSort(col.key)"
          >
            {{ col.label }}
          </th>
          <!-- Si showSelect est vrai , alors on affiche une colonne supplémentaire pour le bouton de sélection -->
          <th v-if="showSelect" class="select-heading"></th>
        </tr>

        <tr class="filter-row">
          <!-- On rajoute une ligne dans le thead pour les inputs de filtrage -->
          <th v-for="col in columns" :key="'filter-' + col.key">
            <div class="filter-cell">
              <input
                v-model="filters[col.key]"
                aria-label="Filtrer"
                class="filter-input"
              />
              <button class="clear-filter" @click="clearFilter(col.key)">
                x
              </button>
            </div>
          </th>
          <th v-if="showSelect" class="filter-select"></th>
        </tr>
      </thead>

      <tbody>
        <!-- On affiche les lignes qui ont été filtrées et triées grâce à la variable computed filteredData -->
        <!-- Comm :key on met row.id || JSON.stringify(row) pour essayer d'utiliser un identifiant unique pour chaque ligne , si la ligne a une propriété id on l'utilise sinon on utilise une version stringifiée de la ligne entière comme clé de secours , ce qui n'est pas optimal mais ça permet d'éviter les erreurs de clé dupliquée si les données n'ont pas d'identifiant unique -->
        <tr v-for="row in filteredData" :key="row.id || JSON.stringify(row)">
          <td v-for="col in columns" :key="col.key">
            <!-- Je parcours chaque colonne pour afficher la valeur correspondante dans la ligne -->
            {{ row[col.key] }}
          </td>
          <td v-if="showSelect" class="select-cell">
            <button class="btn-select" @click="emit('select', row)">
              <!-- Quand on clique sur le  bouton de sélection d'une personne , on émet l'événement "select" en envoyant la ligne correspondante  au composant parent, ce qui permettra au composant parent de savoir quelle personne a été sélectionnée et de réagir en conséquence -->
              {{ selectLabel }}
            </button>
          </td>
        </tr>

        <tr v-if="filteredData.length === 0">
          <!-- Si après le filtrage il n'y a aucune ligne à afficher , alors on affiche un message pour indiquer qu'aucun résultat n'a été trouvé -->
          <td :colspan="columns.length + (showSelect ? 1 : 0)" class="no-data">
            <!-- colspan permet de faire en sorte que la cellule prenne toute la largeur du tableau , on ajoute 1 si showSelect est vrai pour prendre en compte la colonne supplémentaire du bouton de sélection -->
            Aucun résultat trouvé
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<style scoped>
.person-table-wrapper {
  width: 100%;
  overflow-x: auto;
}

.person-table {
  width: 100%;
  border-collapse: collapse;
  border: 2px solid #2b2d78;
  color: #000;
  font-size: 0.82rem;
  table-layout: fixed;
}

.person-table th,
.person-table td {
  height: 1.7rem;
  padding: 0.22rem 0.45rem;
  border: 1px solid #2b2d78;
  text-align: center;
  vertical-align: middle;
}

.person-table thead th {
  background-color: #2b2d78;
  color: #fff;
  font-size: 0.82rem;
  font-weight: 800;
}

.select-heading,
.filter-select,
.select-cell {
  width: 4.7rem;
}

.sortable {
  cursor: pointer;
  user-select: none;
}

.sortable:hover {
  background-color: #30338b;
}

.filter-row th {
  padding: 0.28rem 0.55rem;
  background-color: #2b2d78;
}

.filter-cell {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.35rem;
}

.filter-input {
  width: min(11rem, 100%);
  height: 1.25rem;
  padding: 0.05rem 0.3rem;
  border: 1px solid #98a7b3;
  border-radius: 4px;
  background-color: #f7fbff;
  font-size: 0.78rem;
}

.clear-filter,
.btn-select {
  border: 1px solid #b67a00;
  border-radius: 3px;
  background-color: #ffa500;
  color: #000;
  cursor: pointer;
  font-weight: 500;
}

.clear-filter {
  width: 1rem;
  height: 1rem;
  padding: 0;
  font-size: 0.75rem;
  line-height: 0.8rem;
}

.person-table tbody tr:nth-child(odd) {
  background-color: #d7e5f0;
}

.person-table tbody tr:nth-child(even) {
  background-color: #edfafa;
}

.person-table tbody tr:hover {
  background-color: #c8d9e5;
}

.btn-select {
  padding: 0.25rem 0.42rem;
  font-size: 0.72rem;
}

.btn-select:hover,
.clear-filter:hover {
  background-color: #ffb321;
}

.no-data {
  background-color: #edfafa;
  color: red;
  font-weight: bold;
  font-style: normal;
  text-align: center;
}
</style>
