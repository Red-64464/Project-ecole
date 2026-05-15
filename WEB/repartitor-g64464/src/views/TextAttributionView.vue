<!-- Ce fichier sert a attribuer un texte.
     Le repartiteur choisit un traducteur, un editeur et verifie les dates.
     Il peut aussi voir la charge de travail et le texte precedent. -->
<script setup>
import { ref, onMounted, watch } from "vue";
import { useRoute, useRouter } from "vue-router";
import { getTextById } from "@/services/textService";
import { getAllTranslatorWorkloads } from "@/services/translatorService";
import { getAllEditorWorkloads } from "@/services/editorService";
import { assignTranslation } from "@/services/translationService";
import { assignEdition } from "@/services/editionService";
import TextInfoCard from "@/components/TextInfoCard.vue";
import TranslatorAssignPanel from "@/components/TranslatorAssignPanel.vue";
import EditorAssignPanel from "@/components/EditorAssignPanel.vue";

const route = useRoute();
const router = useRouter();

const text = ref(null); // le texte a attribuer
const loading = ref(true);
const error = ref(""); 
const translators = ref([]); // liste des traducteurs  avec leur charge de travail 
const editorWorkloads = ref([]); // liste des editeurs avec leur charge de boulot 

async function loadData() {
  loading.value = true;
  error.value = "";

  const textId = parseInt(route.params.id); // je récupère l'id du texte depuis l'url
  text.value = await getTextById(textId); // je récupère les infos du texte depuis le service

  if (!text.value) {
    error.value = `Texte #${textId} introuvable`;
    loading.value = false;
    return;
  }

  const [translatorWL, editorWL] = await Promise.all([
    getAllTranslatorWorkloads(), // je récupère les traducteurs avec leur charge de travail
    getAllEditorWorkloads(), // je récupère la charge de travail des editeurs
  ]);

  translators.value = translatorWL;

  editorWorkloads.value = editorWL;

  loading.value = false;
}

onMounted(loadData); // On éxecute la fonction loadData au chargement du composant 
watch(() => route.params.id, loadData); // Si l'id du texte change ( par exemple si on passe a un autre texte )  , on relance la fonction loadData pour recharger les nouvelles données 

async function handleTranslatorAssign({ translator, deadline }) {
    // Je récupère le traducteur sélectionné et la deadline depuis le composant enfant
  const result = await assignTranslation(text.value.id, translator.id, deadline);
  if (result) {
    alert("Traduction attribuee avec succes !");
    await loadData();
  } else {
    alert("Erreur lors de l'attribution.");
  }
}

function getTranslatorForEdition() {
  const translations = text.value.translation || [];
  const sortedTranslations = [...translations].sort(
    (a, b) => new Date(b.attributed || 0) - new Date(a.attributed || 0),
  );
  return sortedTranslations[0]?.translator || null;
}

async function handleEditorAssign({ editor }) {
    // Je veux récupérer le trigramme du traducteur déjà assigné (s'il existe) pour le passer à l'édition
  const translatedBy = getTranslatorForEdition();
  const result = await assignEdition(text.value.id, editor.id, translatedBy);
  if (result) {
    alert("Edition attribuee avec succes !");
    await loadData();
  } else {
    alert("Erreur lors de l'attribution.");
  }
}

function goToPrecedent() {
  router.push({ name: "text-attribution", params: { id: text.value.precedent } });
}
</script>

<template>
  <div class="attribution-view">
    <div v-if="error" class="error-card">{{ error }}</div>
    <p v-else-if="loading" class="loading-message">Chargement...</p>

    <template v-else-if="text"> <!--  Si le texte a attribué existe  , on affiche tout -->
      <div class="top-grid">
        <TextInfoCard :text="text" @go-precedent="goToPrecedent" />
        <TranslatorAssignPanel
          :translators="translators"
          :text-deadline="text.deadline"
          :existing-translations="text.translation || []"
          @assign="handleTranslatorAssign"
        />
      </div>
      <EditorAssignPanel
        :editors="editorWorkloads"
        :existing-editions="text.edition || []"
        @assign="handleEditorAssign"
      />
    </template>
  </div>
</template>

<style scoped>
.attribution-view {
  min-height: calc(100vh - 194px);
  margin: -2rem;
  padding: 0.65rem 1.55rem 2rem;
  background-color: #eaf6ff;
  color: #000;
}

.top-grid {
  display: grid;
  grid-template-columns: minmax(26rem, 1fr) minmax(28rem, 1fr);
  gap: 1.25rem;
  margin-bottom: 1.25rem;
}

.error-card,
.loading-message { font-size: 1.2rem; font-weight: 800; }
.error-card { border: 2px solid #b33; background-color: #ffe7e7; padding: 1rem; }

@media (max-width: 900px) {
  .attribution-view { margin: -2rem; padding: 1rem; }
  .top-grid { grid-template-columns: 1fr; }
}
</style>
