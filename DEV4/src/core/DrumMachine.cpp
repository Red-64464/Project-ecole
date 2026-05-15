/**
 * @brief Structure centrale qui stocke tout l'état de la drum machine.
 *
 * Contient la grille du séquenceur, le BPM, les volumes, les fichiers audio
 * et les paramètres d'effets (delay, reverse).
 *
 * @note Les membres partagés avec le thread audio utilisent @c std::atomic
 * pour éviter les problèmes de lecture/écriture simultanée entre threads donc pour éviter la concurrence entre le thread de l'interface utilisateur et le thread audio.
 */


#include "DrumMachine.h"


/**
 * @brief Constructeur par défaut de DrumMachine.
 *
 * Initialise toutes les pistes à leur état de départ :
 * grille vide, volume max, mute/reverse/delay désactivés.
 */
DrumMachine::DrumMachine(){
    for (int t = 0; t < NUM_TRACKS; t++) // Je parcours toutes les track ( 4 )
    {
        for (int s = 0; s < NUM_STEPS; s++) // Ainsi que touts les steps de chaque Track ( 16 ) 
        {
            grid[t][s].store(false); // Initialiser toutes les cases à false , on utilise .store pour les variables atomiques pour éviter les problèmes de concurrence entre le thread de l'interface utilisateur et le thread audio
        }

      
        volume[t].store(1.0f); // Volume à 1.0 = volume maximum par défaut
       
        muted[t].store(false); // Mute désactivé par défaut (on entend la piste)
      
        reverse[t].store(false);  // Reverse désactivé par défaut (le son joue normalement, pas à l'envers)
       
        delayEnabled[t].store(false); // Effet de delay désactivé par défaut
  
        delayTime[t].store(0.3f);      // Temps de delay par défaut : 0.3 seconde
     
        delayMix[t].store(0.5f);   // Mix du delay par défaut : 0.5 (moitié son original, moitié écho)
        
    }
    
}