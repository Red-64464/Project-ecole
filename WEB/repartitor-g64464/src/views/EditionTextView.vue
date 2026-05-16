<!-- Ce fichier affiche les textes donnes a un editeur.
     L'editeur peut marquer son travail comme termine.
     Mais il est bloque si la traduction du texte n'est pas finie. -->
<script setup>
import { ref, onMounted } from "vue";
import { useRoleStore } from "@/stores/role";
import {
  getEditionsByEditor,
  finishEdition,
  isTranslationFinished,
} from "@/services/editionService";
import WorkerTextsTable from "@/components/WorkerTextsTable.vue";
import { useNotifications } from "@/composables/useNotifications";

const roleStore = useRoleStore();
const { notify } = useNotifications();
const editions = ref([]); // Liste des éditions de l'éditeur (avec les infos du texte et du traducteur)
const loading = ref(true);
const translationStatuses = ref({}); // Objet qui stocke pour chaque texte si sa traduction est terminée  car un éditeur ne peut pas traduire un texte que si sa traduction est finie

async function loadData() {
  loading.value = true;
  if (roleStore.identityId) {
    editions.value = await getEditionsByEditor(roleStore.identityId);
    const statuses = {};
    for (const edition of editions.value) {
        // on regarde si la traduction du texte est terminée 
      statuses[edition.text.id] = await isTranslationFinished(edition.text.id);
    }
    translationStatuses.value = statuses;
  }
  loading.value = false;
}

onMounted(loadData); // On éxecute la fonction loadData au chargement du composant

async function handleFinish(edition) {
  const done = await isTranslationFinished(edition.text.id);
  if (!done) {
    notify(
      "La traduction n'est pas encore terminée. Vous ne pouvez pas éditer ce texte.",
      "error",
    );
    return;
  }
  const success = await finishEdition(edition.text.id, roleStore.identityId);
  if (success) {
    notify("Édition marquée comme terminée.", "success");
    await loadData();
  } else {
    notify("Erreur lors de la mise à jour. Veuillez réessayer.", "error");
  }
}
</script>

<template>
  <div class="edition-view">
    <p v-if="loading" class="loading-message">Chargement de vos textes...</p>
    <WorkerTextsTable
      v-else
      :items="editions"
      mode="edition"
      :translation-statuses="translationStatuses"
      @finish="handleFinish"
    />
  </div>
</template>

<style scoped>
.edition-view { width: 100%; }
.loading-message { color: #000; font-size: 1.1rem; font-weight: 800; }
</style>
