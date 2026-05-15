/**
 * Ce fichier cree la connexion avec Supabase.
 * Les services l'utilisent pour lire/modifier la base de donnees.
 * Le store auth l'utilise pour connecter et deconnecter l'utilisateur.
 */

import { createClient } from '@supabase/supabase-js';


const supabaseUrl = import.meta.env.VITE_SUPABASE_URL || ""; // URL du projet Supabase, ou chaîne vide si pas définie
const supabaseKey = import.meta.env.VITE_SUPABASE_PUBLISHABLE_KEY || ""; // Clé publique Supabase (anon key), ou chaîne vide

if (!supabaseUrl || !supabaseKey) {
  console.warn(
    " Variables Supabase manquantes ! Créez un fichier .env avec VITE_SUPABASE_URL et VITE_SUPABASE_PUBLISHABLE_KEY",
  );
}

// On crée le client Supabase , un client c'est un objet qui nous permet de faire des requetes a la base de donnees Supabase.
export const supabase = supabaseUrl && supabaseKey ? createClient(supabaseUrl , supabaseKey) : null;

export async function ensureFreshSession() {
  if (!supabase) return null;

  const { data, error } = await supabase.auth.getSession();
  if (error || !data.session) return null;

  const { data: refreshedData, error: refreshError } =
    await supabase.auth.refreshSession();

  if (refreshError) {
    console.error("Session expiree :", refreshError.message);
    return null;
  }

  return refreshedData.session;
}

