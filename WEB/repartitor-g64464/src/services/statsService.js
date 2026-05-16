/**
 * Gère les statistiques affichées dans l'application Repartitor.
 *
 * On lit des vues SQL définies dans ressource/repartitor-views.sql :
 *   - stats_global_current : valeurs globales actuelles (répartiteur)
 *   - translator_history   : historique par traducteur
 *   - editor_history       : historique par éditeur
 */

import { ensureFreshSession, supabase } from "@/supabase";

/**
 * Récupère le bloc "Actuellement" pour le répartiteur.
 * @returns {Object|null} Une seule ligne contenant toutes les métriques globales.
 */
export async function getGlobalCurrentStats() {
  if (!supabase) return null;
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("stats_global_current")
    .select("*")
    .single();

  if (error) {
    console.error("Erreur récupération stats globales :", error.message);
    return null;
  }
  return data;
}

/**
 * Récupère l'historique d'un traducteur pour un intervalle donné.
 *
 * @param {string} translatorId - Trigramme du traducteur.
 * @param {string} from - Date ISO (incluse).
 * @param {string} to - Date ISO (incluse).
 * @returns {Object} { attributed, finished, late, words }
 */
export async function getTranslatorStats(translatorId, from, to) {
  const empty = { attributed: 0, finished: 0, late: 0, words: 0 };
  if (!supabase) return empty;
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("translator_history")
    .select("*")
    .eq("translator", translatorId)
    .gte("attributed", from)
    .lte("attributed", to);

  if (error) {
    console.error("Erreur stats traducteur :", error.message);
    return empty;
  }

  // Agrégation côté client : PostgREST ne sait pas faire COUNT/SUM ici.
  return data.reduce(
    (acc, row) => ({
      attributed: acc.attributed + 1,
      finished: acc.finished + (row.done ? 1 : 0),
      late: acc.late + (row.late ? 1 : 0),
      words: acc.words + (row.wordcount || 0),
    }),
    { ...empty },
  );
}

/**
 * Charge de travail courante d'un traducteur (textes en cours et mots restants).
 *
 * @param {string} translatorId - Trigramme du traducteur.
 * @returns {Object} { texts, words }
 */
export async function getTranslatorCurrent(translatorId) {
  const empty = { texts: 0, words: 0 };
  if (!supabase) return empty;
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("translator_history")
    .select("*")
    .eq("translator", translatorId)
    .is("finished", null);

  if (error) {
    console.error("Erreur charge traducteur :", error.message);
    return empty;
  }
  return data.reduce(
    (acc, row) => ({
      texts: acc.texts + 1,
      words: acc.words + (row.wordcount || 0),
    }),
    { ...empty },
  );
}

/**
 * Récupère l'historique d'un éditeur pour un intervalle donné.
 *
 * @param {string} editorId - Trigramme de l'éditeur.
 * @param {string} from - Date ISO (incluse).
 * @param {string} to - Date ISO (incluse).
 * @returns {Object} { attributed, finished, late, words }
 */
export async function getEditorStats(editorId, from, to) {
  const empty = { attributed: 0, finished: 0, late: 0, words: 0 };
  if (!supabase) return empty;
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("editor_history")
    .select("*")
    .eq("editor", editorId)
    .gte("attributed", from)
    .lte("attributed", to);

  if (error) {
    console.error("Erreur stats éditeur :", error.message);
    return empty;
  }
  return data.reduce(
    (acc, row) => ({
      attributed: acc.attributed + 1,
      finished: acc.finished + (row.done ? 1 : 0),
      late: acc.late + (row.late ? 1 : 0),
      words: acc.words + (row.wordcount || 0),
    }),
    { ...empty },
  );
}

/**
 * Charge de travail courante d'un éditeur.
 *
 * @param {string} editorId - Trigramme de l'éditeur.
 * @returns {Object} { texts, words }
 */
export async function getEditorCurrent(editorId) {
  const empty = { texts: 0, words: 0 };
  if (!supabase) return empty;
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("editor_history")
    .select("*")
    .eq("editor", editorId)
    .is("finished", null);

  if (error) {
    console.error("Erreur charge éditeur :", error.message);
    return empty;
  }
  return data.reduce(
    (acc, row) => ({
      texts: acc.texts + 1,
      words: acc.words + (row.wordcount || 0),
    }),
    { ...empty },
  );
}
