/**
 * @file Mixer.cpp
 * @brief Contient l'implementation du mixeur audio.
 *
 * Ce fichier sert a melanger plusieurs sons en un seul son final.
 * Pour faire cela, on additionne les valeurs audio entre elles.
 */

#include "Mixer.h"
#include "core/Constants.h"  // Pour NUM_CHANNELS
#include <algorithm>         // Pour std::clamp

    /**
     * @brief Melange 4 buffers audio dans un seul  buffer de sortie.
     *
     * @param output Buffer de sortie.
     * @param buf1 Buffer  de la piste 1.
     * @param buf2 Buffer  de la piste 2.
     * @param buf3 Buffer  de la piste 3.
     * @param buf4 Buffer  de la piste 4.
     * @param frames Nombre de frames a traiter , par exemple 256 frames par buffer.
     */

     void Mixer::process(float* output,
                        const float* buf1, const float* buf2,
                        const float* buf3, const float* buf4,
                        int frames) {

                int totalSamples = frames * NUM_CHANNELS; // Nombre total de valeurs audio dans le buffer car il y a 512 samples (stereo)

                for (int i = 0; i < totalSamples; i++)
                {
                    // On additionne les 4 pistes, puis on limite le résultat entre -1.0 et 1.0
                    // pour éviter la distorsion si plusieurs pistes jouent fort en même temps.
                    float mixed = buf1[i] + buf2[i] + buf3[i] + buf4[i];
                    output[i] = std::clamp(mixed, -1.0f, 1.0f);
                }
                
                        }