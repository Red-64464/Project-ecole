<!-- Ce fichier affiche la page principale du repartiteur.
     Il montre tous les textes dans un tableau.
     Un clic sur un texte ouvre la page d'attribution. -->
<script setup>
import { ref, onMounted } from "vue";
import { useRouter } from "vue-router";
import { getAllTexts } from "@/services/textService";
import TextsTable from "@/components/TextsTable.vue";

const router = useRouter();

const texts = ref([]); //contient tous les textes a afficher dans le tableau
const loading = ref(true); //indique si les textes sont en cours de chargement

onMounted(async () => { 
  loading.value = true; // indique que le chargement commence
  texts.value = await getAllTexts(); // je récupère touts les textes depuis le service
  loading.value = false; // indique que le chargement est terminé
});

function goToText(id) {
  router.push({ name: "text-attribution", params: { id } });
}
</script>

<template>
  <div class="repartition-view">
    <p v-if="loading" class="loading-message">Chargement des textes...</p>
    <TextsTable v-else :texts="texts" @select="goToText" />
  </div>
</template>

<style scoped>
.repartition-view {
  width: 100%;
}
.loading-message {
  font-size: 1.15rem;
  font-weight: 700;
}
</style>
