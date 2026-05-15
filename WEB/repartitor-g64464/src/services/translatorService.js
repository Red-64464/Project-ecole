/**
 * Ce fichier gere les traducteurs dans Supabase.
 * Il permet de recuperer la liste des traducteurs.
 * Il sert pour choisir un traducteur et voir ses statistiques.
 */

import { ensureFreshSession, supabase } from "@/supabase"; 
/**
 * Récupère la liste de TOUS les traducteurs
 * @returns {Array} Liste des traducteurs [{id, firstname, lastname, expectedthroughput}]
 */

export async function getTranslators() {
  if (!supabase) return []; 
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("translator") 
    .select("*") // On sélectionne toutes les colonnes
    .order("id"); // Tri par trigramme (ordre alphabétique)

  if (error) {
    console.error("Erreur récupération traducteurs :", error.message); // Affiche l'erreur dans la console
    return []; // Retourne un tableau vide en cas d'erreur
  }
  return data; // Retourne la liste des traducteurs
}

/**
 * Récupère UN traducteur par son trigramme
 * @param {string} id - Le trigramme du traducteur (ex: "CTO")
 * @returns {Object|null} Le traducteur ou null
 */
// Retourne UN seul traducteur en cherchant par son trigramme (ex: "CTO")
export async function getTranslatorById(id) {
  if (!supabase) return null; // Si Supabase n'est pas configuré, retourne null
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("translator") // Table "translator"
    .select("*") // Toutes les colonnes
    .eq("id", id) // Filtre : id = le trigramme donné en paramètre
    .single(); // On attend UN seul résultat (error si zéro ou plusieurs)

  if (error) {
    console.error("Erreur récupération traducteur :", error.message);
    return null; // Retourne null si le traducteur n'existe pas
  }
  return data; // Retourne l'objet traducteur
}

/**
 * Récupère la charge de travail de tous les traducteurs
 * Utilise la vue SQL "translator_workload" créée dans la base
 * @returns {Array} [{translator, words, busydays, texts}]
 */
// Retourne la charge de travail de chaque traducteur (depuis une vue SQL pré-calculée)
export async function getTranslatorWorkloads() {
  if (!supabase) return []; // Si Supabase n'est pas configuré, retourne un tableau vide
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("translator_workload") // Vue SQL qui calcule la charge (mots, jours occupés, textes en cours)
    .select("*"); // Toutes les colonnes de la vue

  if (error) {
    console.error("Erreur récupération charge de travail :", error.message);
    return []; // Tableau vide en cas d'erreur
  }
  return data; // [{translator: "CTO", words: 1234, busydays: 3.5, texts: 2}, ...]
}

/**
 * Récupère la liste des traducteurs enrichie de leur charge de travail
 * Même logique que getAllEditorWorkloads() dans editorService.js
 * @returns {Array} [{id, firstname, lastname, expectedthroughput, busydays, texts, words}]
 */
export async function getAllTranslatorWorkloads() {
  const [translators, workloads] = await Promise.all([
    getTranslators(), // Liste complète des traducteurs
    getTranslatorWorkloads(), // Charge depuis la vue SQL
  ]);

  return translators.map((translator) => {
    const wl = workloads.find((item) => item.translator === translator.id);
    return {
      ...translator,
      busydays: wl ? Math.round(wl.busydays * 10) / 10 : 0,
      texts: wl ? wl.texts : 0,
      words: wl ? wl.words : 0,
    };
  });
}
