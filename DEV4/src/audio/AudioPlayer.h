/**
 * @brief Lecteur audio pour une piste de la drum machine.
 *
 * Gère le chargement d'un fichier WAV, le démarrage de la lecture et la
 * production audio échantillon par échantillon en stéréo float 44100 Hz.
 */

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <vector>       // Pour stocker les données audio dans un tableau dynamique
#include <string>       // Pour les chemins de fichiers
#include <string_view>  // Pour string_view : lire une string SANS la copier (plus rapide)
#include <mutex>        // Pour protéger l'accès aux données entre threads

class AudioPlayer {
public:
    // Charge un fichier WAV depuis le chemin donné et le convertit en
    // format stéréo float 44100 Hz. Renvoie true si ça a marché.
    // std::string_view = on reçoit le texte SANS le copier (juste un "regard" sur lui)
    bool loadFile(std::string_view path);

    // Démarre la lecture du son.
    // Si reversePlay est true, le son sera joué à l'envers.
    void start(bool reversePlay);

    // Remplit le buffer "output" avec les données audio de cette piste.
    // - output : le tableau à remplir avec le son
    // - frames : le nombre d'échantillons à produire
    // - volume : le volume de la piste (0.0 à 1.0)
    // - muted : true = la piste est muette (on met du silence)
    void process(float* output, int frames, float volume, bool muted);

private:
    // Le tableau qui contient toutes les données audio du fichier WAV,
    // en format stéréo (gauche, droite, gauche, droite...)
    std::vector<float> buffer;

    // La position actuelle de lecture dans le buffer (quel échantillon on lit)
    int position{0};

    // Est-ce que le son est en train d'être joué ?
    bool playing{false};

    // Est-ce que le son est joué à l'envers ?
    bool reverse{false};

    // Un "mutex" (verrou) qui protège le buffer quand on charge un fichier.
    // Cela empêche le thread audio de lire le buffer pendant qu'on est
    // en train de le remplir avec un nouveau fichier.
    std::mutex bufferMutex;
};

#endif // AUDIO_PLAYER_H
