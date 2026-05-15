<!-- Ce fichier sert a choisir le traducteur d'un texte.
     Il permet aussi de choisir une date limite de traduction.
     Il affiche un avertissement si cette date depasse la deadline du texte. -->
<script setup>
import { ref } from "vue";
import PersonTable from "@/components/PersonTable.vue";

const props = defineProps({
  // la liste des traducteurs disponibles   
  translators: { type: Array, required: true },
  // la deadline globale du texte 
  textDeadline: { type: String, default: null },
  // la liste des traductions déjà attribuées pour ce texte ( si il y en a )
  existingTranslations: { type: Array, default: () => [] },
});
// emit quand on veut signaler au parent que le repartiteur a choisi un traducteur et une deadline pour la traduction du texte
const emit = defineEmits(["assign"]);

const selectedTranslator = ref(null);
const translationDeadline = ref("");
const deadlineWarning = ref("");
const showTranslators = ref(false);

const translatorColumns = [
  { key: "id", label: "Trigramme" },
  { key: "firstname", label: "Prénom" },
  { key: "lastname", label: "Nom" },
  { key: "expectedthroughput", label: "Mots/jour" },
  { key: "busydays", label: "Jours occupés" },
  { key: "texts", label: "Textes" },
];

function checkDeadline() {
    
  if (!translationDeadline.value || !props.textDeadline) {
    deadlineWarning.value = "";
    return;
  }
  const deadline = new Date(translationDeadline.value);
  const globalDeadline = new Date(props.textDeadline);
  deadlineWarning.value =
    deadline > globalDeadline
      ? "Attention : cette date dépasse la date limite globale du texte !"
      : "";
}

function formatShortDate(dateStr) {
  if (!dateStr) return "Non définie";
  return new Date(dateStr).toLocaleString("fr-FR", {
    day: "2-digit",
    month: "2-digit",
    year: "numeric",
    hour: "2-digit",
    minute: "2-digit",
  });
}

// fonction qui est appelée quand le repartiteur clique sur le bouton "Valider" pour enregistrer son choix de traducteur et de deadline
function submit() {
  if (!selectedTranslator.value || !translationDeadline.value) {
    alert("Veuillez sélectionner un traducteur et une date limite.");
    return;
  }
  emit("assign", {
    translator: selectedTranslator.value,
    deadline: translationDeadline.value,
  });
  selectedTranslator.value = null;
  translationDeadline.value = "";
  deadlineWarning.value = "";
}
</script>

<template>
  <section class="panel">
    <h4>Traduction</h4>

    <div class="line-title">
      <span>Traducteur</span>
      <button class="btn-toggle" @click="showTranslators = !showTranslators">
        {{
          showTranslators ? "Cacher les traducteurs" : "Montrer les traducteurs"
        }}
      </button>
    </div>

    <p v-if="existingTranslations.length > 0" class="existing-list">
      <span v-for="item in existingTranslations" :key="item.translator">
        {{ item.translator }} — {{ item.finished ? "terminé" : "en cours" }}
      </span>
    </p>

    <p v-if="selectedTranslator" class="selected-line">
      Actuellement sélectionné :
      {{ selectedTranslator.firstname }} {{ selectedTranslator.lastname }} ({{
        selectedTranslator.id
      }})
      <button class="mini-clear" @click="selectedTranslator = null">x</button>
    </p>

    <PersonTable
      v-if="showTranslators"
      :columns="translatorColumns"
      :data="translators"
      select-label="Choisir"
      @select="
        (t) => {
          selectedTranslator = t;
          showTranslators = false; // on cache la liste des traducteurs après en avoir sélectionné un
        }
      "
    />

    <div class="deadline-form">
      <label for="translation-deadline">Délai</label>
      <input
        id="translation-deadline"
        v-model="translationDeadline"
        type="date"
        @change="checkDeadline"
      />
    </div>

    <h5>Enregistrer</h5>
    <p v-if="selectedTranslator && translationDeadline" class="help-text">
      Attribuer à {{ selectedTranslator.firstname }}
      {{ selectedTranslator.lastname }} ({{ selectedTranslator.id }}) pour le
      {{ formatShortDate(translationDeadline) }}.
    </p>
    <p v-if="deadlineWarning" class="warning">{{ deadlineWarning }}</p>

    <button class="btn-validate" @click="submit">Valider</button>
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

.panel h4 {
  margin: 0 0 0.45rem;
  color: #2b2d78;
  font-size: 1.35rem;
  font-weight: 900;
}

.line-title {
  display: flex;
  align-items: center;
  gap: 0.45rem;
  margin-bottom: 0.65rem;
  color: #2b2d78;
  font-size: 1rem;
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
.existing-list,
.help-text,
.warning {
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
.warning {
  color: #8a4f00;
  font-weight: 800;
}

.deadline-form {
  display: flex;
  align-items: center;
  gap: 0.55rem;
  margin-top: 0.75rem;
}

.deadline-form label,
h5 {
  margin: 0;
  color: #2b2d78;
  font-size: 1rem;
  font-weight: 900;
}

.deadline-form input {
  height: 1.45rem;
  border: 1px solid #99a6b5;
  background-color: #fff;
  font-size: 0.86rem;
}
</style>
