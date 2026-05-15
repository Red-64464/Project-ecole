<!-- Ce fichier sert a choisir l'editeur d'un texte.
     Il montre la liste des editeurs et leur charge de travail.
     Quand on valide, il envoie le choix a la page d'attribution. -->
<script setup>
import { ref } from "vue";
import PersonTable from "@/components/PersonTable.vue";

defineProps({
  editors: { type: Array, required: true },
  existingEditions: { type: Array, default: () => [] },
});
const emit = defineEmits(["assign"]);

const selectedEditor = ref(null);
const showEditors = ref(false);

const editorColumns = [
  { key: "id", label: "Trigramme" },
  { key: "firstname", label: "Prénom" },
  { key: "lastname", label: "Nom" },
  { key: "toTreat", label: "À traiter" },
  { key: "waiting", label: "En attente" },
];

function submit() {
  if (!selectedEditor.value) { 
    alert("Veuillez sélectionner un éditeur.");
    return;
  }
  emit("assign", { editor: selectedEditor.value });
  selectedEditor.value = null;
}
</script>

<template>
  <section class="panel edition-panel">
    <h4>Édition</h4>

    <div class="line-title">
      <span>Éditeur</span>
      <button class="btn-toggle" @click="showEditors = !showEditors">
        {{ showEditors ? "Cacher les éditeurs" : "Montrer les éditeurs" }}
      </button>
    </div>

    <p v-if="existingEditions.length > 0" class="existing-list">
      <span v-for="item in existingEditions" :key="item.editor">
        {{ item.editor }} — {{ item.finished ? "terminé" : "en cours" }}
      </span>
    </p>

    <p v-if="selectedEditor" class="selected-line">
      Actuellement sélectionné :
      {{ selectedEditor.firstname }} {{ selectedEditor.lastname }} ({{
        selectedEditor.id
      }})
      <button class="mini-clear" @click="selectedEditor = null">x</button>
    </p>

    <PersonTable
      v-if="showEditors"
      :columns="editorColumns"
      :data="editors"
      select-label="Choisir"
      @select="
        (e) => {
          selectedEditor = e;
          showEditors = false;
        }
      "
    />

    <button v-if="selectedEditor" class="btn-validate" @click="submit">
      Valider
    </button>
  </section>
</template>

<style scoped>
.panel {
  border: 2px solid #6b72ad;
  border-radius: 4px;
  background-color: #eefafa;
  padding: 1rem 1.1rem;
  box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.2);
}

.edition-panel {
  max-width: 54rem;
  margin: 0 auto;
  padding: 1.6rem 1.25rem 2rem;
}

.panel h4 {
  margin: 0 0 0.45rem;
  color: #2b2d78;
  font-size: 2.4rem;
  font-weight: 900;
}

.line-title {
  display: flex;
  align-items: center;
  gap: 0.45rem;
  margin-bottom: 0.65rem;
  color: #2b2d78;
  font-size: 1.85rem;
  font-weight: 900;
}

.btn-toggle,
.btn-validate,
.mini-clear {
  border: 1px solid #8a4f00;
  border-radius: 3px;
  background-color: #ffa500;
  color: #000;
  cursor: pointer;
  font-weight: 700;
  box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.32);
}

.btn-toggle {
  padding: 0.25rem 0.45rem;
  font-size: 0.68rem;
}
.btn-validate {
  display: block;
  margin-top: 0.7rem;
  padding: 0.55rem 0.85rem;
  font-size: 0.82rem;
}
.mini-clear {
  width: 1rem;
  height: 1rem;
  margin-left: 0.25rem;
  padding: 0;
  font-size: 0.7rem;
  line-height: 0.7rem;
  box-shadow: none;
}

.selected-line,
.existing-list {
  margin: 0.45rem 0;
  color: #000;
  font-size: 0.78rem;
  line-height: 1.35;
}

.existing-list {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}
</style>
