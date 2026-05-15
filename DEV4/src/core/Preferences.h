/**
 * @brief Sauvegarde et chargement des préférences utilisateur.
 *
 * Permet de persister les chemins des fichiers audio (WAV) choisis pour chaque piste
 * dans un fichier texte (@c preferences.txt), afin de les recharger automatiquement 
 * au prochain lancement de l'application comme ca  l'utilisateur n'a pas besoin de recharger manuellement les fichiers audio à chaque fois qu'il lance l'application et de perdre  les sons qu'il a choisi pour chaque piste.
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
#include <map>
#include "Constants.h"

/**
 * @brief Gère la sauvegarde et le chargement des préférences.
 *
 * Toutes les méthodes sont statiques : aucun objet n'est nécessaire.
 * Utilisation : @c Preferences::save(...) ou @c Preferences::load(...)
 */

 class Preferences
 {
 public:
    /**
     * @brief Sauvegarde les chemins des fichiers WAV dans @c preferences.txt.
     * @param filePaths Tableau de @c NUM_TRACKS chemins de fichiers à sauvegarder.
     */
    static void save(const std::string filePaths[NUM_TRACKS]);

    
    /**
     * @brief Charge les chemins des fichiers WAV depuis @c preferences.txt.
     * @param filePaths Tableau de @c NUM_TRACKS chemins à remplir avec les valeurs lues.
     */
    static void load(std::string filePaths[NUM_TRACKS]);
 };

#endif // PREFERENCES_H