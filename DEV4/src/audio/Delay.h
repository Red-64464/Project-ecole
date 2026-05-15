#ifndef DELAY_H
#define DELAY_H

#include <vector> // J'import l'include pour vector qui est une lsite dybamique en c++
#include "core/Constants.h"  // Pour DELAY_BUFFER_SIZE

/**
 * Classe qui applique un effet de delay (echo) au son.
 *
 * Le delay ajoute au son original une copie decalee dans le temps.
 *
 * Cette classe utilise un buffer circulaire pour enregistrer le son
 * puis le relire plus tard afin de creer l'echo.
 */

 class Delay{

 public:
     /**
     * Construit un effet de delay.
     *
     * Le buffer circulaire est initialise avec des valeurs a 0.0.
     */
    Delay();


    /**
     * Applique l'effet de delay au son.
     *
     * @param buffer le buffer audio a modifier
     * @param frames le nombre d'echantillons a traiter dans le buffer en paramètres 
     * @param delayTime le temps de decalage de l'echo
     * @param delayMix l'intensite de l'echo
     * @param enabled indique si l'effet est active ou non 
     */
    void  process(float* buffer,int frames , float delayTime , float delayMix , bool enabled);

 private:
    // Une liste de float qui va représenter le Buffer circulaire
    std::vector<float> circularBuffer;
    // C'est la position actuelle d'écriture dans le Buffer circulaire 
    // "Je suis ici, c'est là que je vais écrire le prochain son."
    int writeIndex{0};

 };

#endif