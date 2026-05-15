/**
 * Gere les editions dans Supabase.
 * Une edition = un texte donne a un editeur.
 */

import { ensureFreshSession, supabase } from "@/supabase";

/**
 * Recupere toutes les editions attribuees a un editeur.
 * @param {string} editorId - Le trigramme de l'editeur.
 * @returns {Array}
 */
export async function getEditionsByEditor(editorId) {
  if (!supabase) return [];
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("edition")
    .select(
      `
      *,
      text ( id, cote, title, received, deadline, wordcount, precedent )
    `,
    )
    .eq("editor", editorId)
    .order("attributed", { ascending: false });

  if (error) {
    console.error("Erreur recuperation editions :", error.message);
    return [];
  }

  return data;
}

/**
 * Attribue un texte a un editeur.
 * @param {number} textId - L'id du texte.
 * @param {string} editorId - Le trigramme de l'editeur.
 * @param {string|null} translatedBy - Le trigramme du traducteur si connu.
 * @returns {Object|null}
 */
export async function assignEdition(textId, editorId, translatedBy = null) {
  if (!supabase) return null;
  await ensureFreshSession();

  const insertData = {
    text: textId,
    editor: editorId,
  };

  if (translatedBy) {
    insertData.translated_by = translatedBy;
  }

  const { data, error } = await supabase
    .from("edition")
    .insert(insertData)
    .select()
    .single();

  if (error) {
    console.error("Erreur attribution edition :", error.message);
    return null;
  }

  return data;
}

/**
 * Marque une edition comme terminee.
 * @param {number} textId - L'id du texte.
 * @param {string} editorId - Le trigramme de l'editeur.
 * @returns {boolean}
 */
export async function finishEdition(textId, editorId) {
  if (!supabase) return false;
  await ensureFreshSession();

  const { error } = await supabase
    .from("edition")
    .update({ finished: new Date().toISOString() })
    .eq("text", textId)
    .eq("editor", editorId);

  if (error) {
    console.error("Erreur terminaison edition :", error.message);
    return false;
  }

  return true;
}

async function getLatestTranslation(textId) {
  if (!supabase) return false;
  await ensureFreshSession();

  const { data, error } = await supabase
    .from("translation")
    .select("translator, finished")
    .eq("text", textId)
    .order("attributed", { ascending: false })
    .limit(1);

  if (error) {
    console.error("Erreur verification traduction :", error.message);
    return null;
  }

  return data[0] ?? null;
}

/**
 * Verifie si la derniere traduction attribuee a un texte est terminee.
 * @param {number} textId - L'id du texte.
 * @returns {boolean}
 */
export async function isTranslationFinished(textId) {
  if (!supabase) return false;

  const translation = await getLatestTranslation(textId);
  return translation?.finished != null;
}
