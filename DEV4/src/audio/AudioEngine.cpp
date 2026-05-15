/**
 * @brief Implémentation du moteur audio.
 *
 * Démarre PortAudio, configure la sortie audio, et contient le callback
 * appelé ~172 fois/sec pour produire le son en temps réel.
 */

#include "AudioEngine.h"
#include "core/DrumMachine.h"  // Pour accéder aux données de la drum machine
#include <SDL3/SDL.h>          // Pour SDL_Log (afficher des messages d'erreur)

/**
 * @brief Prépare et démarre le moteur audio.
 * @param dm Pointeur vers la DrumMachine (données partagées).
 *
 * Étapes :
 *   1) Pa_Initialize() prépare PortAudio
 *   2) outputParams   configure la sortie (stéréo, float32, latence basse)
 *   3) Pa_OpenStream() crée le flux avec notre callback
 *   4) Pa_StartStream() démarre le flux (le callback commence à être appelé)
 */
void AudioEngine::init(DrumMachine* dm) {
    drumMachine = dm; // On garde le pointeur pour y accéder depuis le callback

    PaError err = Pa_Initialize(); // Initialise toute la bibliothèque PortAudio
    if (err != paNoError) {
        SDL_Log("PortAudio init failed: %s", Pa_GetErrorText(err));
        return;
    }

    PaStreamParameters outputParams; // Configuration de la sortie audio

    outputParams.device = Pa_GetDefaultOutputDevice(); //  Si aucun appareil de sortie n'est trouvé, c'est une erreur critique
    if (outputParams.device == paNoDevice) {
        SDL_Log("No default audio output device found");
        return;
    }

    outputParams.channelCount = NUM_CHANNELS;          // 2 canaux = stéréo (gauche + droite)
    outputParams.sampleFormat = paFloat32;             // Float 32 bits par échantillon
    outputParams.suggestedLatency =
        Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency; // Latence la plus basse possible
    outputParams.hostApiSpecificStreamInfo = nullptr;

    // Pa_OpenStream() crée le flux audio.
    // On passe `this` dans userData pour récupérer l'objet dans le callback.
    // &stream = c'est l'adresse où on va stocker le flux créé  
    // nullptr = pas de flux d'entrée (micro)
    // &outputParams = configuration de la sortie
    err = Pa_OpenStream(&stream, nullptr, &outputParams,
                        SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paNoFlag, audioCallback, this);
    if (err != paNoError) {
        SDL_Log("PortAudio open stream failed: %s", Pa_GetErrorText(err));
        return;
    }

    Pa_StartStream(stream); // Démarre le flux  ,  audioCallback() est appelé en boucle
}

/**
 * @brief Arrête et ferme le moteur audio proprement.
 *
 * Appelé à la fermeture de l'application.
 */
void AudioEngine::shutdown() {
    if (stream) {
        Pa_StopStream(stream);   // Arrête le flux audio
        Pa_CloseStream(stream);  // Libère les ressources du flux
        stream = nullptr;        // Mise à null par sécurité
    }
    Pa_Terminate(); // Ferme complètement PortAudio
}

/**
 * @brief Charge un fichier WAV dans un AudioPlayer.
 * @param track Numéro de piste (0 à 3).
 * @param path  Chemin vers le fichier WAV.
 * @return true si le chargement a réussi.
 *
 * @note Le chargement se fait dans un thread séparé pour ne pas bloquer
 *       l'interface graphique. join() attend la fin du thread avant de retourner.
 */
bool AudioEngine::loadFile(int track, std::string_view path) {
    if (track < 0 || track >= NUM_TRACKS) return false;

    bool success = false;

    // On crée un thread pour charger le fichier sans bloquer l'UI. Le thread modifie la variable "success" pour indiquer si ça a marché.
    // [&]() { ... } = lambda (fonction anonyme) qui permet d'utiliser les variables locales (comme "success" car elle se trouve dans le fonction ) à l'intérieur du thread. C'est plus simple que de faire une fonction séparée.
    std::thread loadThread([&]() {
        success = players[track].loadFile(path);
    });

    loadThread.join(); // Attend la fin du thread avant de continuer
    return success;
}

/**
 * @brief Bascule entre Play et Stop.
 *
 * Si la lecture est en cours - arrête le séquenceur.
 * Si elle est arrêtée - démarre le séquenceur.
 */
void AudioEngine::togglePlay() {
    if (drumMachine->playing.load()) {
        sequencer.stop(drumMachine);   // En train de jouer - on arrête
    } else {
        sequencer.start(drumMachine);  // Arrêté - on démarre
    }
}

/**
 * @brief Callback audio appelé automatiquement par PortAudio (~172 fois/sec).
 *
 * C'est le cœur du système audio. PortAudio demande 256 échantillons à chaque appel.
 *
 * Étapes :
 *   1) Séquenceur  avance les pas et déclenche les sons
 *   2) AudioPlayer chaque piste remplit son buffer (volume + mute)
 *   3) Delay       applique l'écho sur chaque piste
 *   4) Mixer       combine les 4 pistes en un seul buffer de sortie
 * @param input          Données micro entrantes (non utilisé ici).
 * @param output         Buffer à remplir pour les haut-parleurs.
 * @param framesPerBuffer Nombre d'échantillons à produire (256).
 * @param timeInfo       Informations de timing PortAudio (non utilisé).
 * @param statusFlags    Flags de statut PortAudio (non utilisé).
 * @param userData        Pointeur vers l'objet AudioEngine (passé via Pa_OpenStream).
 * @return paContinue pour dire à PortAudio de continuer à appeler le callback.
 * 
 */
int AudioEngine::audioCallback(const void* /*input*/, void* output,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* /*timeInfo*/,
                                PaStreamCallbackFlags /*statusFlags*/,
                                void* userData) {
    auto* engine = static_cast<AudioEngine*>(userData); // Récupère l'objet AudioEngine depuis userData
    auto* out    = static_cast<float*>(output);          // Buffer de sortie (haut-parleurs)
    auto* dm     = engine->drumMachine;                  // Accès aux données de la drum machine
    int   frames = static_cast<int>(framesPerBuffer);    // 256 échantillons à produire

   
    engine->sequencer.process(dm, engine->players); //  Le séquenceur avance les pas et déclenche les sons

   
    float trackBuffers[NUM_TRACKS][BUFFER_SIZE]; //  Chaque piste produit son son, puis on applique l'écho
    for (int i = 0; i < NUM_TRACKS; i++) {
        // Je remplis trackBuffers[i] avec le son de la piste i, en appliquant le volume et le mute. C'est la partie "AudioPlayer".
        engine->players[i].process(trackBuffers[i], frames,
                                   dm->volume[i].load(), dm->muted[i].load()); 
        // Ensuite, j'applique l'effet de delay sur trackBuffers[i] en fonction des paramètres de la piste i. C'est la partie "Delay".
        engine->delays[i].process(trackBuffers[i], frames,
                                  dm->delayTime[i].load(), dm->delayMix[i].load(),
                                  dm->delayEnabled[i].load());
    }

    // Le mixer additionne les 4 pistes en un seul buffer de sortie
    engine->mixer.process(out,
                          trackBuffers[0], trackBuffers[1],
                          trackBuffers[2], trackBuffers[3], frames);

    return paContinue; // variable spéciale de PortAudio qui indique de continuer a appeler cette fonction callback
}
