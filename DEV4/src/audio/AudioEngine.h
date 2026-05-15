/**
 * @brief Le moteur audio principal — chef d'orchestre de tout le son.
 *
 * Cette classe fait le lien entre PortAudio (la bibliothèque qui parle aux
 * haut-parleurs) et tous les modules audio du projet (players, séquenceur,
 * delays, mixer).
 *
 * @note audioCallback() est une fonction spéciale appelée automatiquement
 * par PortAudio environ 172 fois par seconde (44100 / 256) pour demander
 * les prochaines données audio à envoyer aux haut-parleurs.
 */

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "portaudio.h"           // Bibliothèque PortAudio pour la sortie audio
#include "audio/AudioPlayer.h"   // Les lecteurs audio (un par piste)
#include "audio/StepSequencer.h" // Le séquenceur pas-à-pas
#include "audio/Delay.h"         // L'effet de delay (écho)
#include "audio/Mixer.h"         // Le mixeur qui combine les pistes
#include "core/Constants.h"      // Les constantes (NUM_TRACKS, etc.)
#include <string_view>           // Pour string_view : lire une string sans la copier
#include <thread>                // Pour std::thread : lancer un travail dans un thread séparé


struct DrumMachine; // Déclaration anticipée : on dit que DrumMachine existe sans inclure tout le fichier

class AudioEngine {
public:
    /**
     * @brief Initialise PortAudio et démarre le flux audio.
     *
     * Étapes : initialise PortAudio, récupère l'appareil de sortie par défaut,
     * ouvre le flux avec audioCallback, puis démarre le flux.
     *
     * @param dm Pointeur vers la DrumMachine pour accéder à toutes les données partagées.
     */
    void init(DrumMachine* dm);

    /**
     * @brief Arrête le flux audio et libère toutes les ressources PortAudio.
     *
     * Appelle Pa_StopStream(), Pa_CloseStream() puis Pa_Terminate().
     * Doit être appelé à la fermeture de l'application.
     */
    void shutdown();

    /**
     * @brief Charge un fichier WAV dans une piste donnée.
     *
     * Le chargement se fait dans un thread séparé pour ne pas bloquer
     * l'interface graphique si le fichier est volumineux.
     *
     * @param track Numéro de la piste (0 à NUM_TRACKS-1).
     * @param path  Chemin du fichier WAV (ex : "C:/sons/kick.wav").
     * @return      true si le chargement a réussi, false sinon.
     */
    bool loadFile(int track, std::string_view path);

    /**
     * @brief Bascule entre Play et Stop.
     *
     * Si la lecture est en cours - appelle sequencer.stop().
     * Si la lecture est arrêtée - appelle sequencer.start().
     */
    void togglePlay();

private:
    /**
     * @brief Callback audio appelé automatiquement par PortAudio (~172 fois/seconde).
     *
     * C'est le cœur du système audio. À chaque appel, il :
     *   1. Fait avancer le séquenceur (timing + déclenchement des sons)
     *   2. Remplit un buffer par piste via les AudioPlayers
     *   3. Applique l'effet delay sur chaque piste
     *   4. Mélange les 4 pistes en un seul buffer de sortie via le Mixer
     *
     * @note Déclarée  static car PortAudio est une bibliothèque C qui ne
     * comprend pas les méthodes d'objet. L'objet AudioEngine est récupéré
     * via  userData (passé lors de Pa_OpenStream avec this).
     *
     * @param input          Données micro entrantes (non utilisé ici).
     * @param output         Buffer de sortie à remplir pour les haut-parleurs.
     * @param framesPerBuffer Nombre d'échantillons à produire (256).
     * @param timeInfo       Informations de timing PortAudio (non utilisé).
     * @param statusFlags    Flags de statut PortAudio (non utilisé).
     * @param userData       Pointeur vers l'objet AudioEngine courant.
     * @return                paContinue pour maintenir le flux actif.
     */
    static int audioCallback(const void* input, void* output,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData);

    PaStream* stream{nullptr};          // Flux audio PortAudio (connexion vers les haut-parleurs)
    DrumMachine* drumMachine{nullptr};  // Pointeur vers la DrumMachine (données partagées)
    AudioPlayer players[NUM_TRACKS];    // Un lecteur audio par piste (4 au total)
    StepSequencer sequencer;            // Séquenceur qui gère le timing et déclenche les sons
    Delay delays[NUM_TRACKS];           // Un effet delay (écho) par piste (4 au total)
    Mixer mixer;                        // Mixeur qui combine les 4 pistes en un seul son
};

#endif // AUDIO_ENGINE_H
