#include "StepSequencer.h"
#include "core/Constants.h"      // Pour FRAMES_PER_BUFFER, SAMPLE_RATE, etc.
#include "core/DrumMachine.h"    // Pour accéder aux données de la drum machine comme la grille, le BPM, etc.
#include "audio/AudioPlayer.h"   // Pour pouvoir déclencher les sons

    /**
     * @brief Démarre la lecture du séquenceur (appelé quand on clique "Play").
     *
     * Remet le curseur au pas 0, met playing à true, et lève le drapeau
     * startRequested pour que le thread audio prépare le premier battement.
     * On a startRequested pour que quand le thread UI démarre il puisse prévenir le thread audio via une variable booléènne
     * @param dm Pointeur vers la drum machine dont on démarre la lecture.
     */

     void StepSequencer::start(DrumMachine* dm) {
        dm->currentStep.store(0); // Remet le curseur au pas 0

        startRequested.store(true); // Lève le drapeau pour que le thread audio prépare le premier battement

         dm->playing.store(true); // Met playing à true pour indiquer que la lecture est en cours

     }

    /**
     * @brief Arrête la lecture du séquenceur (appelé quand on clique "Stop").
     *
     * Met playing à false, remet le curseur au pas 0, et lève le drapeau
     * stopRequested pour que le thread audio remette ses compteurs à zéro.
     *
     * @param dm Pointeur vers la drum machine dont on arrête la lecture.
     */

     void StepSequencer::stop(DrumMachine* dm){

        dm->playing.store(false); // On met playing à false = la lecture s'arrête

        dm->currentStep.store(0); // On remet le curseur au pas 0

        stopRequested.store(true); // On lève un "drapeau" pour dire au thread audio : "il faut s'arrêter"

     }

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

     void StepSequencer::process(DrumMachine* dm, AudioPlayer players[4]) {

        // On s'occupe des drapeaux startRequested et stopRequested : si l'UI a demandé de démarrer ou d'arrêter, on réagit en conséquence

        if(startRequested.load()){ // .load() permets de lire la valeur donc on dis si stopRequested est à true alors on traite la demande d'arrêt
            startRequested.store(false);     // On a traité la demande, on baisse le drapeau
            beatAccumulator = 0.0;
            needsFirstBeat = true; // On dit qu'on a besoin du premier battement pour que le séquenceur puisse se synchroniser correctement
        }

        if(stopRequested.load()){
            stopRequested.store(false); // On a traité la demande donc on le remet a false
            beatAccumulator = 0.0; // On remet le compteur de temps à zéro
            needsFirstBeat = false; // On dit qu'on a besoin du premier battement pour que le séquenceur puisse se synchroniser correctement
        }


        if(!dm->playing.load()) return; // Si la lecture n'est pas en cours, on ne fait rien


        // --- Premier battement ---
        // Donc quand on appuie sur Play  , on veut entendre le premiers pas imédiatement 
        if(needsFirstBeat){ // Si on a besoin du premier battement 
            needsFirstBeat = false; // On dit qu'on n'en a plus besoin (on va le jouer maintenant)
            // On veut regarder quel est le pas actuel ^
            int step = dm->currentStep.load(); // On lit le pas actuel (0 à 15)
            // Pour chaque piste , si la case est activée , on déclanche le son 
            for (int track = 0; track < NUM_TRACKS; track++)
            {
                if(dm->grid[track][step].load()){ // Si la case de la grille pour cette piste et ce pas est à true (activée)
                    players[track].start(dm->reverse[track].load()); // On déclenche le son de cette piste , en vérifiant si elle doit être jouée à l'envers ou pas
                }
            }

        }

       // On va calculer le temps écoulé 
       
       // 256 / 44100 = 0.0058 secondes par buffer , on convertit ça en battements pour savoir quand avancer d'un pas
       const double bufferDuration =  static_cast<double>(FRAMES_PER_BUFFER) / SAMPLE_RATE; 
       // On récupère le BPM et on le convertis en double
       double bpm = static_cast<double>(dm->bpm.load());

        // On calcule combien de "battements" ce buffer représente.
        // Formule : (durée en secondes × BPM) / 60
        // Par exemple à 120 BPM : (0.0058 × 120) / 60 ≈ 0.0116 battements
       double durationInBeats = (bufferDuration * bpm) / 60.0;

       beatAccumulator += durationInBeats; // On ajoute ce temps écoulé au compteur

       

       // --- Avancement des pas ---
    // Quand le compteur dépasse STEP_DURATION (0.25 = un quart de battement),
    // on passe au pas suivant. On utilise "while" au lieu de "if" au cas où
        while (beatAccumulator >= STEP_DURATION) {
                // On soustrait la durée d'un pas du compteur
                beatAccumulator -= STEP_DURATION;

                // On passe au pas suivant.
                // Le % NUM_STEPS (modulo 16) fait revenir à 0 après le pas 15.
                // Par exemple : (15 + 1) % 16 = 0 → on revient au début.
                int step = (dm->currentStep.load() + 1) % NUM_STEPS;
                dm->currentStep.store(step); // et on enregistre le step actuel dans la variable currentStep 

                // On déclenche tous les sons activés à ce nouveau pas
                for (int track = 0; track < NUM_TRACKS; track++) {
                    if (dm->grid[track][step].load()) {
                        players[track].start(dm->reverse[track].load());
                    }
                }
            }


     }