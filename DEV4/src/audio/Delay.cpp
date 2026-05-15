    #include "Delay.h"
    #include <algorithm> // Pour std::clamp ( qui permets de limiter une valeur entre une valeur minimale et maximale )


    /**
     * Construit un effet de delay.
     *
     * Le buffer circulaire est initialise avec des valeurs a 0.0.
     * CircularBuffer( nombres de cases a créer , valeur de départ dans chaque case )
     */
    Delay::Delay() : circularBuffer(DELAY_BUFFER_SIZE , 0.0f){};


    /**
     * Applique l'effet de delay au son.
     *
     * @param buffer le buffer audio a modifier
     * @param frames le nombre d'echantillons a traiter dans le buffer en paramètres 
     * @param delayTime le temps de decalage de l'echo
     * @param delayMix l'intensite de l'echo
     * @param enabled indique si l'effet est active ou non 
     */

    
     void Delay::process(float* buffer, int frames, float delayTime, float delayMix, bool enabled) {

        if(!enabled) return; // Si l'effet d'écho n'est pas activer je quitte la fonction et je ne fais rien 

        // Cela sert a calculer de combien de cases on doist reculer dans le buffer circulaire pour créer l'echo 
        // delayTime = le temps du delay , par exmeple : 0.5 secondes , SAMPLE_RATE = 44100 échantillons par seconde , NUM_CHANNELS = 2 (stereo)
        // static_cast<int> permets de transformer le résultat en nombre entier ( par exemple : 22050.7 = 22050 )
        // exemple : (0,5 * 44100) * 2 = 44100 , pour faire un écho de 0,5 secondes en stéréo il faut reculer de 44100 valeurs dans le buffer circulaire  , donc on part de la position actuelle writeIndex , puis on recule de delaySamples pour trouver readIndex pour lire l'ancien son 
        // on transforme en int pour avoir un nombre entier de cases a reculer dans le buffer circulaire , car on ne peut pas reculer de 22050.7 cases , on doit reculer de 22050 cases
        int delaySamples = static_cast<int>(delayTime* SAMPLE_RATE) * NUM_CHANNELS;

        // On force delaySamples donc le temps de delay a rester dans une valeur correcte pour éviter les problèmes de dépassement de buffer
        // On commence a 2 car si on met 0 ou 1 , on risque de lire des valeurs qui n'ont pas encore été écrites dans le buffer circulaire , et on finit a DELAY_BUFFER_SIZE - 2 pour éviter de dépasser la taille du buffer circulaire
        delaySamples = std::clamp(delaySamples , 2 , DELAY_BUFFER_SIZE - 2);

        // On part de writeIndex et on recule de delaySamples pour trouver readIndex , et on utilise le modulo pour revenir au début du buffer circulaire si on dépasse la fin
        // on met  % DELAY_BUFFER_SIZE pour faire en sorte que si on dépasse la fin du buffer circulaire , on revient au début , c'est ce qui rend le buffer circulaire
        int readIndex = (writeIndex - delaySamples + DELAY_BUFFER_SIZE)% DELAY_BUFFER_SIZE;

        // le nombres de sampless en tout c'est  le nombre de frames multiplié par le nombre de canaux ( stéréo = 2 ) , cela nous donne le nombre total de valeurs à traiter dans le buffer audio
        // Par exemple : 256 * 2 = 512 samples dans le buffer audio a traiter 
        int totalSamples = frames * NUM_CHANNELS;
        
        for (int i = 0; i < totalSamples; i++)
        {
            // Le circularBuffer = la mémoire du passé , il stocke touts les son qui sont passé donc on lit dedans pour récupéré l'echo ( readIndex )
            // Buffer = ce que l'utlisateur entends donc on le modifie pour y rajouter l'echo 
            /*
            1 ) Le chemin complet : on lit l'écho donc le son du passé depuis le circular buffer 
            2 ) On ajoute ensuite l'écho lu au son original dans le buffer  afin de créer le son final qui est le son original + l'écho
            3 ) On écrit ensuite le son actuel ( qui est le son original + l'écho ) dans le circular buffer pour qu'il puisse être utilisé comme écho dans le futur , donc pour le stocker dans le circularBuffer a la position writeIndex
            4 ) On avance ensuite dans le buffer circulaire pour lire le prochain écho et pour écrire le prochain son , on utilise le modulo pour revenir au début du buffer circulaire si on dépasse la fin
            */

            // On lit la valeur du passé dans le buffer circulaire  = c'est l'écho 
            float delayed = circularBuffer[readIndex];
            // buffer[i] c'est le son original 
            // delayMix c'est le volume de l'écho et delayed c'est l'ancien son ou est le readIndex 
            // donc c'est le son actuel + l'ancien son * écho 
            buffer[i] = buffer[i] + delayMix * delayed; // On ajoute l'écho au son original , delayMix est le volume de l'écho

            circularBuffer[writeIndex] = buffer[i]; // On écrit le son actuel dans le buffer circulaire pour qu'il puisse être utilisé comme écho dans le futur donc pour le stocker dans le circularBuffer a la position writeIndex

            readIndex = (readIndex + 1)% DELAY_BUFFER_SIZE; // On avance dans le buffer circulaire pour lire le prochain écho , on utilise le modulo pour revenir au début du buffer circulaire si on dépasse la fin
            writeIndex = (writeIndex + 1)% DELAY_BUFFER_SIZE; // On avance dans le buffer circulaire pour écrire le prochain son , on utilise le modulo pour revenir au début du buffer circulaire si on dépasse la fin
        }
        
        


     }