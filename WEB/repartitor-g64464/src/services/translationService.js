/**
 * Ce fichier gere les traductions dans Supabase.
 * Une traduction = un texte donne a un traducteur.
 * Il sert aussi a marquer la traduction comme terminee.
 */

import { ensureFreshSession, supabase } from "@/supabase"; 

/**
 * Récupère toutes les traductions attribuées à UN traducteur
 * Inclut les infos du texte lié (cote, titre, deadline, etc.)
 * @param {string} translatorId - Le trigramme du traducteur (ex: "CTO")
 * @returns {Array} Liste des traductions avec les textes associés
 */


export async function getTranslationsByTranslator(translatorId) {
  if (!supabase) return []; // Supabase non configuré → tableau vide
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("translation") // Table "translation" 
    .select(
      `
      *,
      text ( id, cote, title, received, deadline, wordcount, precedent )
    `, // On joint la table "text" pour avoir les infos du texte en même temps
    )
    .eq("translator", translatorId) 
    .order("attributed", { ascending: false }); // Du plus récent au plus ancien

  if (error) {
    console.error("Erreur récupération traductions :", error.message);
    return [];
  }
  return data; 
}

/**
 * Attribue un texte à un traducteur (crée une nouvelle traduction)
 * @param {number} textId - L'id du texte
 * @param {string} translatorId - Le trigramme du traducteur
 * @param {string} deadline - La date limite au format ISO (ex: "2026-04-15T18:00:00")
 * @returns {Object|null} La traduction créée ou null en cas d'erreur
 */

export async function assignTranslation(textId, translatorId, deadline) {
  if (!supabase) return null; // Supabase non configuré → null
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("translation") // Table "translation"
    .insert({
      text: textId,
      translator: translatorId, 
      deadline: deadline, 
    })
    .select() // Récupère les données insérées
    .single(); // On attend un seul résultat

  if (error) {
    console.error("Erreur attribution traduction :", error.message);
    return null;
  }
  return data; 
}

/**
 * Marque une traduction comme terminée (met la date de fin)
 * @param {number} textId - L'id du texte
 * @param {string} translatorId - Le trigramme du traducteur
 * @returns {boolean} true si succès, false sinon
 */

export async function finishTranslation(textId, translatorId) {
  if (!supabase) return false; // Supabase non configuré → échec
  await ensureFreshSession();
  const { error } = await supabase
    .from("translation") // Table "translation"
    .update({ finished: new Date().toISOString() }) // Met la date/heure actuelle dans finished
    .eq("text", textId) 
    .eq("translator", translatorId); 

  if (error) {
    console.error("Erreur terminaison traduction :", error.message);
    return false; 
  }
  return true; 
}
