/**
 * @file Preferences.cpp
 * @brief Implémentation de la sauvegarde et du chargement des préférences.
 *
 * @c save() écrit les chemins de fichiers dans @c preferences.txt.
 * @c load() les relit depuis ce même fichier.
 */

#include "Preferences.h"
#include <fstream>
#include <map>

/// Nom du fichier texte utilisé pour stocker les préférences.
static const std::string PREFS_FILE = "preferences.txt";

/**
 * @brief Sauvegarde les chemins WAV dans @c preferences.txt.
 *
 * Chaque piste est écrite sous la forme @c clé=valeur (ex: @c track0=C:/sons/kick.wav).
 * La map garantit un ordre cohérent avant l'écriture.
 * On crée une map temporaire pour structurer les données avant de les écrire dans le fichier.
 *
 * @param filePaths Tableau de @c NUM_TRACKS chemins à sauvegarder.
 */
void Preferences::save(const std::string filePaths[NUM_TRACKS]){

        std::map<std::string, std::string> prefs; // Je crée une map pour stocker les préférences de manière structurée (clé-valeur) , la clé sera "track0", "track1", etc et la valeur sera le chemin du fichier wav correspondant à chaque piste
        for (int i = 0; i < NUM_TRACKS; i++)
        {
            std::string key = "track" + std::to_string(i);
            prefs[key] = filePaths[i]; // Je remplis la map avec les chemins de fichiers à sauvegarder , par exemple : prefs["track0"] = "C:/sons/kick.wav"
        }

        std::ofstream file(PREFS_FILE); // J'ouvre le fichier en écriture 
        if (!file.is_open()) return; // Si il n'existe pas je quitte la fonction et je ne fais rien
        
        for (const auto& [key, value] : prefs) { // Je parcours la map et j'écris chaque paire clé-valeur dans le fichier sous la forme "clé=valeur" , par exemple : "track0=C:/sons/kick.wav"
            file << key << "=" << value << "\n"; // J'écris la clé et la valeur dans le fichier , suivi d'un saut de ligne pour que chaque préférence soit sur une ligne différente
            // exemple : track0=C:/sons/kick.wav
        }
}


/**
 * @brief Recharge les chemins WAV depuis @c preferences.txt.
 *
 * Lit chaque ligne au format @c clé=valeur, stocke dans une map temporaire,
 * puis remet chaque valeur à la bonne case de @c filePaths selon son index.
 * Si le fichier n'existe pas (premier lancement), la fonction retourne sans rien faire.
 * Ici le rôle de la map est différent , elle sert à stocker les données lues du fichier de manière structurée avant de les remettre dans le tableau filePaths.
 * @param filePaths Tableau de @c NUM_TRACKS chemins à remplir.
 */


void Preferences::load(std::string filePaths[NUM_TRACKS]) {

    std::ifstream file(PREFS_FILE); // J'ouvre le fichier en lecture
    if (!file.is_open()) return; // Si le fichier n'existe pas (premier lancement), je quitte la fonction et je ne fais rien

    std::map<std::string, std::string> prefs; // Je crée une map 
    std::string line; // Je lis le fichier ligne par ligne
     while (std::getline(file, line)) {
        auto pos = line.find('='); // Je cherche le caractère '=' pour séparer la clé de la valeur
        if(pos == std::string::npos) continue; // Si il n'y a pas de '=' dans la ligne , je passe à la ligne suivante

        std::string key  = line.substr(0, pos); // Je récupère la clé (tout ce qui est avant le '=') , par exemple : "track0"
        std::string value = line.substr(pos + 1); // Je récupère la valeur
        prefs[key] = value;
     }

      /// Transfert de la map vers le tableau : chaque piste retrouve le bon index.
    for (int i = 0; i < NUM_TRACKS; i++) {
        std::string key = "track" + std::to_string(i); // Je recrée la clé pour chaque piste (track0, track1, etc) pour pouvoir retrouver la valeur correspondante dans la map
        if (prefs.count(key)) { // Si la clé existe dans la map , je remplis le tableau filePaths à la bonne case avec la valeur correspondante de la map , par exemple : filePaths[0] = prefs["track0"] = "C:/sons/kick.wav"
            filePaths[i] = prefs[key];
        }
    }


}
