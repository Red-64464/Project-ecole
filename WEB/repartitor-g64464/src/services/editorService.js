/**
 * Ce fichier gere les editeurs dans Supabase.
 * Il permet de recuperer la liste des editeurs.
 * Il sert pour choisir un editeur et voir ses statistiques.
 */

import { ensureFreshSession, supabase } from "@/supabase"; // Client Supabase pour les requêtes à la base de données


/**
 * Récupère la liste de TOUS les éditeurs
 * @returns {Array} Liste des éditeurs [{id, firstname, lastname}]
 */


export async function getEditors() {
  if (!supabase) return [];
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("editor") 
    .select("*") 
    .order("id"); 

  if (error) {
    console.error("Erreur récupération éditeurs :", error.message);
    return [];
  }
  return data; 
}

/**
 * Récupère UN éditeur par son trigramme
 * @param {string} id - Le trigramme (ex: "HRO")
 * @returns {Object|null} L'éditeur ou null
 */


export async function getEditorById(id) {
  if (!supabase) return null; 
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("editor") 
    .select("*") 
    .eq("id", id) 
    // de base Supabase retourne un tableau même pour une seule ligne, donc on lui dis on veut une liste plutôt que un tableau avec un seul truc 
    .single();

  if (error) {
    console.error("Erreur récupération éditeur :", error.message);
    return null;
  }
  return data; 
}


/**
 * Récupère la charge de travail depuis la VUE SQL "editor_workload" 
 * @returns {Array} [{editor, toTreat, waiting, pending}]
 */
export async function getEditorWorkloads() {
  if (!supabase) return []; 
  await ensureFreshSession();

  const [{ data: editions, error: editionsError }, { data: translations, error: translationsError }] =
    await Promise.all([
      supabase
        .from("edition")
        .select("editor, text, finished, attributed"),
      supabase
        .from("translation")
        .select("text, finished, attributed")
        .order("attributed", { ascending: false }),
    ]);

  if (editionsError || translationsError) {
    console.error(
      "Erreur récupération charge éditeurs :",
      editionsError?.message || translationsError?.message,
    );
    return []; 
  }

  const latestTranslationByText = new Map();
  for (const translation of translations || []) {
    if (!latestTranslationByText.has(translation.text)) {
      latestTranslationByText.set(translation.text, translation);
    }
  }

  const workloads = new Map();
  for (const edition of editions || []) {
    if (edition.finished) continue;

    const workload = workloads.get(edition.editor) || {
      editor: edition.editor,
      toTreat: 0,
      waiting: 0,
      pending: 0,
    };
    const translation = latestTranslationByText.get(edition.text);

    if (translation?.finished) {
      workload.toTreat += 1;
    } else {
      workload.waiting += 1;
    }
    workload.pending += 1;
    workloads.set(edition.editor, workload);
  }

  return [...workloads.values()];
}


/**
 * Récupère la liste des éditeurs enrichie de leur charge de travail
 * pour les traducteurs
 * @returns {Array} [{id, firstname, lastname, pending}]
 */
export async function getAllEditorWorkloads() {
  const [editors, workloads] = await Promise.all([
    getEditors(), // Liste complète des éditeurs (id, firstname, lastname)
    getEditorWorkloads(), // Charge depuis la vue SQL (editor, texts, pending)
  ]);

  // On fusionne : pour chaque éditeur, on cherche sa charge dans la vue SQL
  return editors.map((editor) => {
    const wl = workloads.find((w) => w.editor === editor.id); 
    return {
      ...editor, 
      toTreat: wl ? wl.toTreat : 0,
      waiting: wl ? wl.waiting : 0,
      pending: wl ? wl.pending : 0, 
    };
  });
}
