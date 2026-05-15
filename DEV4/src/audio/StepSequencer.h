/**
 * @brief Le séquenceur pas-à-pas (step sequencer) — le "chef d'orchestre" de la drum machine.
 *
 * Son rôle est de savoir QUAND déclencher chaque son en fonction du tempo.
 *
 * Comment ça marche ?
 *   - Le séquenceur a 16 "pas" (steps) qui défilent en boucle.
 *   - À chaque pas, il regarde dans la grille quelles pistes sont activées.
 *   - Si une piste est activée à ce pas, il déclenche le son correspondant.
 *   - La vitesse de défilement dépend du BPM (battements par minute).
 *
 * @note Imagine un métronome qui avance sur 16 cases. À chaque case, il regarde
 *       "est-ce que je dois jouer un son ici ?" et si oui, il le joue.
 */

#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#include <atomic>  // Pour les variables partagées entre threads

// Déclarations anticipées (forward declarations) 
// On les écrit ici pour que en gros on puisse utiliser les types DrumMachine et AudioPlayer dans les méthodes de StepSequencer sans inclure leurs fichiers d'en-tête ici, ce qui évite des inclusions circulaires et accélère la compilation.
// "Ces types existent quelque part, fais-moi confiance. Tu verras leur contenu plus tard."
struct DrumMachine;
class AudioPlayer;

/**
 * @brief Séquenceur pas-à-pas qui gère le timing et le déclenchement des sons.
 *
 * Cette classe est le cœur rythmique de la drum machine. Elle est alimentée
 * par le thread audio (~172 fois/seconde) via process(), et contrôlée par
 * l'interface utilisateur via start() et stop().
 */

class StepSequencer {

public:
    /**
     * @brief Traite un buffer audio : vérifie s'il faut avancer d'un pas
     *        et déclenche les sons si nécessaire.
     *
     * C'est le cœur du séquenceur. Cette méthode est appelée à chaque cycle
     * audio, soit environ 172 fois par seconde (44100 / 256).
     * Elle accumule le temps écoulé et avance d'un pas dès que la durée
     * d'un pas est atteinte (STEP_DURATION = 0.25 battement).
     *
     * @param dm      Pointeur vers la drum machine (grille, BPM, état de lecture...).
     * @param players Tableau des 4 lecteurs audio, un par piste.
     */
    void process(DrumMachine* dm, AudioPlayer players[4]);
    
    /**
     * @brief Démarre la lecture du séquenceur (appelé quand on clique "Play").
     *
     * Remet le curseur au pas 0, met playing à true, et lève le drapeau
     * startRequested pour que le thread audio prépare le premier battement.
     * On a startRequested pour que quand le thread UI démarre il puisse prévenir le thread audio via une variable booléènne
     * @param dm Pointeur vers la drum machine dont on démarre la lecture.
     */
    void start(DrumMachine* dm);

    /**
     * @brief Arrête la lecture du séquenceur (appelé quand on clique "Stop").
     *
     * Met playing à false, remet le curseur au pas 0, et lève le drapeau
     * stopRequested pour que le thread audio remette ses compteurs à zéro.
     *
     * @param dm Pointeur vers la drum machine dont on arrête la lecture.
     */
    void stop(DrumMachine* dm);

private:
    /**
     * @brief Drapeau levé par l'UI quand l'utilisateur clique sur Play.
     *
     * Utilise std::atomic car il est écrit par le thread UI et lu par le
     * thread audio. atomic garantit qu'il n'y a pas de conflit entre les deux.
     */
    std::atomic<bool> startRequested{false};

    
    /**
     * @brief Drapeau levé par l'UI quand l'utilisateur clique sur Stop.
     *
     * Utilise std::atomic pour la même raison que startRequested :
     * accès concurrent entre le thread UI (écriture) et le thread audio (lecture).
     */
    std::atomic<bool> stopRequested{false};

    /**
     * @brief Compteur de temps écoulé en battements depuis le dernier pas.
     *
     * C'est comme un seau qui se remplit au fil du temps : à chaque appel de
     * process(), on y ajoute une petite fraction de battement. Quand il dépasse
     * STEP_DURATION (0.25), on passe au pas suivant et on lui soustrait 0.25
     * (sans le remettre à 0, pour ne pas perdre le reste et rester précis).
     *
     *Pourquoi remettre a 0 en  gros car si on ne remet pas à 0, on perd la fraction de temps qui dépasse 0.25, ce qui peut faire que le séquenceur dérive légèrement au fil du temps 
     * (par exemple, si on a 0.26, on joue le pas suivant et on remet à 0.01 au lieu de 0). En gardant cette fraction, on reste plus précis sur le timing.
     *
     * @note Pas besoin d'atomic : cette variable n'est utilisée que par le thread audio.
     */
    double beatAccumulator{0.0};

    /**
     * @brief Signal pour jouer le premier pas immédiatement au démarrage.
     *
     * Sans ce flag, après un clic sur Play, il faudrait attendre que
     * beatAccumulator atteigne 0.25 (~120ms) avant d'entendre le premier son.
     * Avec ce flag, process() joue le pas 0 dès son premier appel après start(),
     * sans attendre que le compteur se remplisse.
     *
     * Il est true pendant une fraction de seconde seulement : il passe à true
     * dans start(), et repasse à false dès que process() a joué le premier pas.
     *
     * @note Pas besoin d'atomic : uniquement utilisé par le thread audio.
     */
    bool needsFirstBeat{false};
}; 

#endif // STEP_SEQUENCER_H