<script setup>
/**
 * Page de gestion des rôles de l'application Repartitor.
 *
 * Cette vue permet à l'utilisateur de choisir son rôle actuel parmi
 * répartiteur, traducteur ou éditeur, et de sélectionner l'identité précise si besoin.
 *
 * @component
 */
import { ref, onMounted } from "vue";
import { useRoleStore } from "@/stores/role";
import { getTranslators } from "@/services/translatorService";
import { getEditors } from "@/services/editorService";
import PersonTable from "@/components/PersonTable.vue";

const roleStore = useRoleStore();

const translators = ref([]); // Liste des traducteurs disponibles
const editors = ref([]); // Liste des éditeurs disponibles
const showTranslators = ref(true); // Affiche ou cache la liste des traducteurs
const showEditors = ref(true); // Affiche ou cache la liste des éditeurs
const loading = ref(true); // Indique si les données sont en cours de chargement

const personColumns = [
  // Colonnes à afficher dans les tableaux de personnes
  { key: "id", label: "Trigramme" },
  { key: "firstname", label: "Prénom" },
  { key: "lastname", label: "Nom" },
];

// onMounted permets de charger les données des traducteurs et éditeurs dès que le composant est monté donc dès que l'utilisateur arrive sur la page des rôles. On utilise Promise.all pour faire les deux requêtes en parallèle et réduire le temps de chargement. Une fois les données récupérées, on les stocke dans les variables correspondantes et on met loading à false pour afficher les tableaux.
onMounted(async () => {
  loading.value = true; // true car on attends les données
  const [translatorData, editorData] = await Promise.all([
    // On récupère les données mais on utilise Promise.all afin de faire les deux requêtes en parallèle et réduire le temps de chargement
    getTranslators(),
    getEditors(),
  ]);
  translators.value = translatorData; // on remplis le tableau des traducteurs avec les données récupérées
  editors.value = editorData; // on remplis le tableau des éditeurs avec les données récupérées
  loading.value = false; // on a les données, on peut afficher les tableaux, on met loading à false
});

// Ces fonctions permettent de donner un rôle a moi même  , quan don clique sur un traducteur ou un éditeur on devient lui même

function selectTranslator(translator) {
  // fonction qui  uqi est appelé quand on sélectionne un traducteur dans le tableau , elle reçoit en paramètre l'objet du traducteur sélectionné et elle utilise la méthode setTraducteur du store de rôle pour mettre à jour le rôle actuel avec le traducteur sélectionné.
  roleStore.setTraducteur(translator); // On attribue le rôle de traducteur a
}

function selectEditor(editor) {
  roleStore.setEditeur(editor);
}
</script>

<template>
  <div class="roles-view">
    <h2>Changement de rôle</h2>
    <!-- Si les données sont en cours de chargement , on affiche ce message -->
    <p v-if="loading" class="loading-message">Chargement des données...</p>
    <!-- Si les données sont prètes  j'affiche ceci -->
    <template v-else>
      <section class="role-section repartiteur-section">
        <h3>Répartiteur</h3>
        <div class="repartiteur-action">
          <button
            class="btn-main-role"
            :class="{ 'btn-active': roleStore.isRepartiteur }"
            @click="roleStore.setRepartiteur()"
          >
            {{
              roleStore.isRepartiteur
                ? "Rôle actuel"
                : "Revenir au rôle de répartiteur"
            }}
          </button>
        </div>
      </section>

      <section class="role-section">
        <h3>
          Traducteur
          <button
            class="btn-toggle"
            @click="showTranslators = !showTranslators"
          >
            {{ showTranslators ? "Cacher la liste" : "Afficher la liste" }}
          </button>
        </h3>

        <p v-if="roleStore.isTraducteur" class="current-role">
          Actuellement sélectionné :
          <strong>
            {{ roleStore.identity.firstname }}
            {{ roleStore.identity.lastname }} ({{ roleStore.identityId }})
          </strong>
        </p>

        <PersonTable
          v-if="showTranslators"
          :columns="personColumns"
          :data="translators"
          select-label="Choisir"
          @select="selectTranslator"
        />
      </section>

      <section class="role-section">
        <h3>
          Éditeur
          <button class="btn-toggle" @click="showEditors = !showEditors">
            {{ showEditors ? "Cacher la liste" : "Afficher la liste" }}
          </button>
        </h3>

        <p v-if="roleStore.isEditeur" class="current-role">
          Actuellement sélectionné :
          <strong>
            {{ roleStore.identity.firstname }}
            {{ roleStore.identity.lastname }} ({{ roleStore.identityId }})
          </strong>
        </p>

        <PersonTable
          v-if="showEditors"
          :columns="personColumns"
          :data="editors"
          select-label="Choisir"
          @select="selectEditor"
        />
      </section>
    </template>
  </div>
</template>

<style scoped>
@import url("https://fonts.googleapis.com/css2?family=Audiowide&display=swap");

.roles-view {
  max-width: none;
  min-height: calc(100vh - 194px);
  margin: -2rem;
  padding: 0.65rem 1.5rem 2rem;
  background-color: #eaf6ff;
  color: #000;
}

.roles-view h2 {
  margin: 0 0 1.65rem;
  padding: 0;
  border: 0;
  color: #000;
  font-family: "Audiowide", "Orbitron", "Trebuchet MS", Arial, sans-serif;
  font-size: clamp(2.3rem, 4.7vw, 3.55rem);
  font-variant: normal;
  font-weight: 400;
  letter-spacing: 0;
  line-height: 1;
  text-align: center;
  text-transform: uppercase;
}

.loading-message {
  font-size: 1.2rem;
  font-weight: 700;
}

.role-section {
  margin-bottom: 2rem;
}

.repartiteur-section {
  margin-bottom: 1.6rem;
}

.role-section h3 {
  display: flex;
  align-items: center;
  gap: 0.55rem;
  margin: 0 0 1.35rem;
  color: #000;
  font-size: 1.85rem;
  font-weight: 800;
  line-height: 1;
}

.repartiteur-action {
  display: flex;
  justify-content: center;
  margin-top: 1.9rem;
}

.btn-main-role,
.btn-toggle {
  border: 1px solid #b67a00;
  border-radius: 3px;
  background-color: #ffa500;
  color: #8a4f00;
  cursor: pointer;
  font-weight: 700;
  box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.28);
}

.btn-main-role {
  min-width: 16.5rem;
  padding: 0.72rem 1rem;
  font-size: 1rem;
}

.btn-toggle {
  padding: 0.27rem 0.48rem;
  font-size: 0.68rem;
  color: #000;
  box-shadow: 1px 1px 3px rgba(0, 0, 0, 0.3);
}

.btn-main-role:hover,
.btn-toggle:hover {
  background-color: #ffb321;
}

.btn-active {
  color: #8a4f00;
}

.current-role {
  display: inline-block;
  margin: 0 0 0.8rem;
  padding: 0.35rem 0.6rem;
  border: 1px solid #2b2d78;
  background-color: #d7e5f0;
  color: #000;
  font-size: 0.95rem;
}

@media (max-width: 760px) {
  .roles-view {
    margin: -2rem;
    padding: 1rem;
  }

  .roles-view h2 {
    font-size: 2rem;
  }

  .role-section h3 {
    align-items: flex-start;
    flex-direction: column;
    font-size: 1.55rem;
  }

  .repartiteur-action {
    justify-content: flex-start;
  }

  .btn-main-role {
    min-width: 0;
    width: 100%;
  }
}
</style>
