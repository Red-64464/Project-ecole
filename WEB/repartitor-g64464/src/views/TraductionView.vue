<!-- Ce fichier affiche les textes donnes a un traducteur.
     Le traducteur voit seulement ses propres textes.
     Il peut marquer une traduction comme terminee. -->
<script setup>
import { ref, onMounted } from "vue";
import { useRoleStore } from "@/stores/role";
import {
  getTranslationsByTranslator,
  finishTranslation,
} from "@/services/translationService";
import WorkerTextsTable from "@/components/WorkerTextsTable.vue";

const roleStore = useRoleStore();
const translations = ref([]); // Liste des traductions du traducteur 
const loading = ref(true); // Indique si les données sont en cours de chargement 

async function loadData() {
  loading.value = true;
  if (roleStore.identityId) { // Si l'identité du traducteur est dispo , on charge ses traductions 
    translations.value = await getTranslationsByTranslator(roleStore.identityId);
  }
  loading.value = false;
}

onMounted(loadData); // On éxecute la fonction loadData au chargement du composant

// fonction qui est appelée quand le traducteur marque une traduction comme terminée
async function handleFinish(translation) {
  const success = await finishTranslation(
    translation.text.id,
    roleStore.identityId,
  );
  if (success) await loadData();
  else alert("Erreur lors de la mise a jour. Veuillez reessayer.");
}
</script>

<template>
  <div class="traduction-view">
    <p v-if="loading" class="loading-message">Chargement de vos textes...</p>
    <WorkerTextsTable
      v-else
      :items="translations"
      mode="traduction"
      @finish="handleFinish"
    />
  </div>
</template>

<style scoped>
.traduction-view { width: 100%; }
.loading-message { color: #000; font-size: 1.1rem; font-weight: 800; }
</style>
