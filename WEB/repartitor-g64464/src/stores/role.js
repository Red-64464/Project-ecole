/**
 * Ce store garde le role choisi dans l'application.
 * Il sait si on est repartiteur, traducteur ou editeur.
 * Il garde aussi la personne choisie et la sauvegarde en localStorage.
 */

import { defineStore } from "pinia";
import { ref, computed } from "vue";

export const useRoleStore = defineStore("role", () => {
  // On lit le rôle sauvé dans le localStorage (survit au rechargement de page)
  const role = ref(localStorage.getItem("role") || "repartiteur");

  const identity = ref(JSON.parse(localStorage.getItem("identity")) || null); // L'identité de la personne choisie (null pour le répartiteur)

  // Le trigramme de la personne (ex: "CTO"), ou null si on est répartiteur
  const identityId = computed(() => identity.value?.id ?? null);

  // Le nom complet de la personne (ex: "Cheslie Tove"), ou chaîne vide si répartiteur
  const identityName = computed(() => {
    if (!identity.value) return ""; // Pas d'identité → chaîne vide
    return `${identity.value.firstname} ${identity.value.lastname}`; // Concaténation prénom + nom
  });

  const isRepartiteur = computed(() => role.value === "repartiteur"); // true si le rôle actuel est répartiteur
  const isTraducteur = computed(() => role.value === "traducteur"); // true si le rôle actuel est traducteur
  const isEditeur = computed(() => role.value === "editeur"); // true si le rôle actuel est éditeur

  function setRepartiteur() {
    role.value = "repartiteur";
    identity.value = null; // Aucune identité pour le répartiteur
    localStorage.setItem("role", "repartiteur");
    localStorage.removeItem("identity"); // Supprime l'identité sauvegardée
  }

  function setTraducteur(translator) {
    role.value = "traducteur";
    identity.value = translator;
    localStorage.setItem("role", "traducteur");
    localStorage.setItem("identity", JSON.stringify(translator));
  }

  function setEditeur(editor) {
    role.value = "editeur";
    identity.value = editor;
    localStorage.setItem("role", "editeur");
    localStorage.setItem("identity", JSON.stringify(editor));
  }

  // Réinitialise le rôle au répartiteur (utilisé lors de la déconnexion)
  function reset() {
    setRepartiteur();
  }

  return {
    role,
    identity,
    identityId,
    identityName,
    isRepartiteur,
    isTraducteur,
    isEditeur,
    setRepartiteur,
    setTraducteur,
    setEditeur,
    reset,
  };
});
