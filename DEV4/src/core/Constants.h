/**
 * @file Constants.h
 * @brief Contient les constantes simples utilisees dans tout le projet.
 *
 * Ce fichier regroupe les valeurs fixes du programme.
 * Cela permet de modifier une valeur a un seul endroit plûtot que de le faire partout .
 */

/**
 * @brief Empeche ce fichier d'etre charge plusieurs fois.
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Nombre de pistes dans la drum machine .
 *
 * Ici, il y a 4 pistes .
 */
const int NUM_TRACKS = 4;

/**
 * @brief Nombre de pas ( de case ) dans le sequenceur.
 *
 * Ici, il y a 16 pas.
 */
const int NUM_STEPS = 16;

/**
 * @brief Taux d'echantillonnage audio c'est le nombre de frames  lu par seconde en stereo .
 *
 * La valeur 44100 veut dire que le son est traite 44100 fois par seconde.
 * 
 * 88200 samples par seconde en mono ( 44100 * 2 pour stereo ) .
 */
const int SAMPLE_RATE = 44100;

/**
 * @brief Nombre de frames dans un buffer traitees a chaque appel audio.
 *
 * Une petite valeur rend le son plus reactif.
 */
const int FRAMES_PER_BUFFER = 256;

/**
 * @brief Nombre de canaux audio.
 *
 * 2 veut dire stereo : gauche et droite.
 * 
 * Cela permets d'avoir un son stereo.
 */
const int NUM_CHANNELS = 2;

/**
 * @brief Taille totale du buffer audio.
 *
 * On multiplie le nombre de frames par le nombre de canaux.
 * Ici, cela donne 512 valeurs.
 * 
 * 256 frames * 2 canaux = 512 valeurs dans le buffer audio.
 */
const int BUFFER_SIZE = FRAMES_PER_BUFFER * NUM_CHANNELS;


/**
 * @brief Taille du buffer de delai audio.
 *
 * Cette constante correspond a 88200 samples, soit 1 seconde
 * d'audio stereo a 44100 Hz.
 */

const int DELAY_BUFFER_SIZE = SAMPLE_RATE * NUM_CHANNELS;


/**
 * @brief Duree d'un step en battements.
 *
 * 0.25 veut dire un quart de battement.
 * Cela sert a regler la vitesse d'avance du sequenceur.
 */
const double STEP_DURATION = 0.25;

#endif 