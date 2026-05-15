/**
 * @brief Structure centrale qui stocke tout l'état de la drum machine.
 *
 * Contient la grille du séquenceur, le BPM, les volumes, les fichiers audio
 * et les paramètres d'effets (delay, reverse).
 *
 * @note Les membres partagés avec le thread audio utilisent @c std::atomic
 * pour éviter les problèmes de lecture/écriture simultanée entre threads donc pour éviter la concurrence entre le thread de l'interface utilisateur et le thread audio.
 */
#ifndef DRUM_MACHINE_H
#define DRUM_MACHINE_H

#include <atomic>
#include <string>
#include "Constants.h"

/**
 * @brief Structure principale de la drum machine.
 *
 * Accessible par l'interface graphique et le thread audio.
 * Les membres atomiques sont thread-safe ; les autres sont réservés à l'UI.
 */

struct DrumMachine
{

    /// Tempo en battements par minute. Par défaut : 120 BPM.
    std::atomic<int> bpm{120};

    /// @c true si la lecture est en cours, @c false si arrêtée.
    std::atomic<bool> playing{false};

    /// Position de lecture actuelle dans le séquenceur (0 à NUM_STEPS-1).
    std::atomic<int> currentStep{0};

    // Grille du séquenceur : true si la note est activée, false sinon. Dimensions : NUM_TRACKS x NUM_STEPS.
    // 4 x 16 cases , chaque case représente une note , si la case est à true alors la note est jouée , sinon elle n'est pas jouée
    std::atomic<bool> grid[NUM_TRACKS][NUM_STEPS];

    /// Volume de chaque piste. Plage : 0.0 (silence) à 1.0 (max).
    std::atomic<float> volume[NUM_TRACKS];

    /// @c true si la piste est en mute (son coupé).
    std::atomic<bool> muted[NUM_TRACKS];

    /// @c true si le son de la piste est joué à l'envers.
    std::atomic<bool> reverse[NUM_TRACKS];

    /// @c true si l'effet delay (écho) est activé sur la piste.
    std::atomic<bool> delayEnabled[NUM_TRACKS];

    /// Durée du delay en secondes. Plage : 0.0 à 1.0.
    std::atomic<float> delayTime[NUM_TRACKS];

    /// Niveau du mix de l'écho. Plage : 0.0 (aucun) à 1.0 (fort).
    std::atomic<float> delayMix[NUM_TRACKS];

    /// Chemin complet du fichier WAV chargé pour chaque piste. Ex : @c "C:/sons/kick.wav"
    std::string filePath[NUM_TRACKS];

    /// Nom du fichier seul (sans chemin), affiché dans l'UI. Ex : @c "kick.wav"
    std::string fileName[NUM_TRACKS];

    /// Index de la piste sélectionnée dans l'interface (0 à NUM_TRACKS-1).
    int selectedTrack{0};

    /// Chemin du fichier en attente de chargement (choisi via la boîte de dialogue).
    std::string pendingFilePath;

    /// @c true quand l'utilisateur a validé le choix d'un fichier.
    bool fileDialogComplete{false};

    // Le constructeur : initialise toutes les valeurs par défaut
    DrumMachine();
};




#endif // DRUM_MACHINE_H