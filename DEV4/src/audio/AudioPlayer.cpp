/**
 * @brief Implémentation du lecteur audio
 *
 * Ce fichier contient le code pour charger un WAV, démarrer la lecture
 * et produire le son échantillon par échantillon.
 */

#include "AudioPlayer.h"
#include "AudioFileReader.h"  // Chargement et conversion WAV
#include "core/Constants.h"  // Pour SAMPLE_RATE, NUM_CHANNELS
#include <SDL3/SDL.h>
#include <algorithm>         // Pour std::fill

/**
 * @brief Charge un fichier WAV depuis le disque.
 *
 * Delegue le chargement et la conversion a AudioFileReader,
 * puis stocke le resultat dans le buffer interne.
 *
 * @param path   Chemin du fichier (ex: "C:/sons/kick.wav")
 * @return       true si le chargement a reussi, false sinon
 */
bool AudioPlayer::loadFile(std::string_view path) {
    // On délègue tout le chargement et la conversion SDL à AudioFileReader.
    
    // On crée une boite et on demande a AudioFile de la remplir avec le son final 
    std::vector<float> tempBuffer;
    if (!AudioFileReader::loadFile(path, SAMPLE_RATE, tempBuffer))
        return false;

    // On verrouille le mutex pour modifier le buffer en toute sécurité
    // Car il y as 2 threads qui tournent en même temps : 
    // 1 -- Le thread audio qui lit le buffer pour jouer le son 
    // 2 -- Le thread  principal qui veut écrire un nouveau son dans le buffer
    {
        
        std::lock_guard<std::mutex> lock(bufferMutex); // On verrouille le mutex pour protéger l'accès au buffer pendant la mise a jour 
        playing = false;                        // On arrête la lecture en cours avant de changer le son 
        buffer  = std::move(tempBuffer);        // On transfère tempBuffer dans buffer au lieu de tout copier (plus rapide)
    }
    return true;
}

/**
 * @brief Demarre la lecture du son.
 *
 * Utilise try_lock pour ne pas bloquer le thread audio
 * si le mutex est deja pris.
 *
 * @param reversePlay  true pour jouer le son a l'envers
 */
void AudioPlayer::start(bool reversePlay) {
    // On essaye de verrouiller le mutex sans attendre
    // On mets unique_lock avec try_to_lock pour ne pas bloquer le thread audio si le mutex est déjà pris donc on abandonne le start Parce que le thread audio doit produire du son en permanence, sans jamais s'arrêter , car si il s'arrête il  lag.
    std::unique_lock<std::mutex> lock(bufferMutex, std::try_to_lock);

    // Si on n'a pas réussi à verrouiller le lock OU si le buffer est vide (pas de fichier chargé),
    // on ne fait rien
    if (!lock.owns_lock() || buffer.empty()) return;

    // On mémorise si on joue à l'envers ou non
    reverse = reversePlay;

    // On calcule le nombre total de "frames" (paires gauche/droite)
    // en divisant la taille du buffer par 2 (car stéréo)
    // 512 / 2 = 256 frames
    int totalFrames = static_cast<int>(buffer.size()) / NUM_CHANNELS;

    // Si on joue à l'envers, on commence à la fin. Sinon, on commence au début.
    position = reverse ? (totalFrames - 1) : 0;

    // On active la lecture
    playing = true;
}

/**
 * @brief Remplit le buffer de sortie avec le son de cette piste.
 *
 * Appelee a chaque cycle audio (~172 fois/sec).
 * Lit les donnees du buffer et les copie dans output.
 *
 * @param output  Tableau de sortie a remplir
 * @param frames  Nombre d'echantillons a produire
 * @param volume  Volume de 0.0 (silence) a 1.0 (max)
 * @param muted   true = piste muette, on met du silence
 */
 
void AudioPlayer::process(float* output, int frames, float volume, bool muted) {
    // On essaye de verrouiller le mutex (sans attendre si impossible)
    std::unique_lock<std::mutex> lock(bufferMutex, std::try_to_lock);

    // Si on ne peut pas verrouiller, ou si le son ne joue pas, ou pas de fichier :
    // on remplit le buffer de sortie avec du silence (0.0)
    if (!lock.owns_lock() || !playing || buffer.empty()) {
        // std::fill remplit tout le tableau avec 0.0 (silence)
        // output = début | output + frames * NUM_CHANNELS = fin du tableau
        // 0.0f = valeur de silence pour un échantillon audio
        std::fill(output, output + frames * NUM_CHANNELS, 0.0f);
        return;
    }

    // Nombre total de frames dans le fichier audio chargé
    // et on le transforme en INT
    int totalFrames = static_cast<int>(buffer.size()) / NUM_CHANNELS;

    // On parcourt chaque frame demandée donc tout le buffer de sortie
    for (int i = 0; i < frames; i++) {
        // Si le son a fini de jouer, on met du silence pour le reste
        if (!playing) {
            output[i * 2] = 0.0f;      // Canal gauche = silence , le canal de gauche sera toujours un nombre pair 
            output[i * 2 + 1] = 0.0f;  // Canal droit = silence , le canal de droit sera toujours un nombre impair
            continue;  // On passe directement au prochain i
        }

        if (muted) {
            // Si la piste est muette, on met du silence
            output[i * 2] = 0.0f;      // Canal gauche = silence , le canal de gauche sera toujours un nombre pair 
            output[i * 2 + 1] = 0.0f;  // Canal droit = silence , le canal de droit sera toujours un nombre impair
        } else {
            // On copie les données audio en appliquant le volume.
            // position * 2 = canal gauche, position * 2 + 1 = canal droit
            // On multiplie par volume pour ajuster le volume.
            // buffer = le fichier chargé depuis le disque , les données du son wav sont stockées dans ce buffer
            // output = le buffer final 
            output[i * 2]     = buffer[position * 2]     * volume;
            output[i * 2 + 1] = buffer[position * 2 + 1] * volume;
        }

        // On avance (ou recule) la position de lecture
        if (reverse) {
            position--;  // En arrière
            // Si on est arrivé avant le début, le son est fini car on ne peut plus reculer 
            if (position < 0) playing = false;
        } else {
            position++;  // En avant
            // Si on a dépassé la fin du fichier, le son est fini donc on arrête de le jouer 
            if (position >= totalFrames) playing = false;
        }
    }
}
