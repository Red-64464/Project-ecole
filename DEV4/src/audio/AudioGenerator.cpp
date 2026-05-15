/**
 * @file AudioGenerator.cpp
 * @brief Implémentation du générateur de son synthétique (onde sinusoïdale).
 *
 * Calcule mathématiquement une onde sinusoïdale et la place dans un buffer stéréo.
 *
 * @note Différence avec la version laboratoire :
 *   - Avant : init() ouvrait son propre flux PortAudio (conflit avec AudioEngine).
 *   - Maintenant : process() remplit simplement un buffer, comme AudioPlayer.
 *     C'est AudioEngine qui gère le seul flux PortAudio du projet.
 */

 
#include "AudioGenerator.h"
#include "core/Constants.h"   ///< SAMPLE_RATE
#include <cmath>               ///< std::sin, M_PI



/**
 * @brief Remplit le buffer avec une onde sinusoïdale.
 *
 * L'onde sinusoïdale est calculée avec la formule :
 *   sample = volume * sin(2π × frequency × temps)
 *
 * La variable currentTimeInSeconds avance d'un pas (1 / SAMPLE_RATE)
 * à chaque échantillon, ce qui fait "avancer" la vague de manière continue
 * entre deux appels successifs.
 *
 * @param output    Buffer stéréo à remplir (taille : frames * 2 floats).
 * @param frames    Nombre d'échantillons à produire (généralement 256).
 * @param frequency Fréquence de la note en Hz (ex : 440.0 = La).
 * @param volume    Volume de 0.0 (silence) à 1.0 (maximum).
 */

 void AudioGenerator::process(float* output, int frames, float frequency, float volume) {
    for (int i = 0; i < frames; i++) { // Je parcours chaque sample à produire
        /** Calcul de la valeur de l'onde sinusoïdale à cet instant précis. */
        float sample = volume * static_cast<float>(
            std::sin(2.0 * M_PI * frequency * currentTimeInSeconds)
        );

        output[i * 2]     = sample; ///< Canal gauche
        output[i * 2 + 1] = sample; ///< Canal droit
        // On mets la même valeur dans les deux canaux pour un son mono centré.
        
        /** Avance le temps d'un échantillon pour le prochain calcul. */
        currentTimeInSeconds += 1.0 / SAMPLE_RATE;
    }
}
