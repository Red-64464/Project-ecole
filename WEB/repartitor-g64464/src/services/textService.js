/**
 * Ce fichier gere les textes dans Supabase.
 * Il permet de recuperer la liste des textes et un texte par son id.
 * Il sert pour afficher les textes et attribuer un texte a un traducteur/editeur.
 */

import { ensureFreshSession, supabase } from "@/supabase";

/**
 * Récupère la liste de TOUS les textes
 * @returns {Array} Liste des textes [{id, cote, title, received, deadline, wordcount, precedent}]
 */
export async function getAllTexts() {
  if (!supabase) return [];
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("text")
    .select("*, translation(*), edition(*)")
    .order("id");

  if (error) {
    console.error("Erreur récupération textes :", error.message);
    return [];
  }
  return data;
}

/**
 * Récupère UN texte par son id, avec ses traductions et éditions associées
 * @param {number} id - L'id du texte
 * @returns {Object|null} Le texte ou null
 */
export async function getTextById(id) {
  if (!supabase) return null;
  await ensureFreshSession();
  const { data, error } = await supabase
    .from("text")
    .select("*, translation(*), edition(*)")
    .eq("id", id)
    .single();

  if (error) {
    console.error("Erreur récupération texte :", error.message);
    return null;
  }
  return data;
}
