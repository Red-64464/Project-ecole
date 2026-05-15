#ifndef AUDIO_GENERATOR_H
#define AUDIO_GENERATOR_H


/**
 * @brief Générateur de son synthétique (onde sinusoïdale).
 *
 * Cette classe calcule mathématiquement un son pur à partir d'une fréquence
 * et d'un volume. Elle ne lit aucun fichier — elle invente le son en temps réel.
 * Son interface est identique à AudioPlayer : elle reçoit un buffer et le remplit.
 */
class AudioGenerator {
public:
     /**
     * @brief Remplit un buffer audio avec une onde sinusoïdale.
     *
     * Appelée par AudioEngine à chaque cycle audio (~172 fois/seconde).
     * La formule utilisée est : sample = volume * sin(2π × frequency × temps)
     *
     * @param output    Buffer stéréo à remplir (taille : frames * 2 floats).
     * @param frames    Nombre d'échantillons à produire (généralement 256).
     * @param frequency Fréquence de la note en Hz (ex : 440.0 = La).
     * @param volume    Volume de 0.0 (silence) à 1.0 (maximum).
     */
    void process(float* output, int frames, float frequency, float volume);

private:
     /**
     * @brief Temps écoulé en secondes depuis le début de la génération.
     *
     * Avance d'un pas (1 / SAMPLE_RATE) à chaque échantillon produit.
     * Ne se remet jamais à zéro pour éviter les coupures audibles entre
     * deux appels successifs de process().
     */
    double currentTimeInSeconds{0.0};
};

#endif // AUDIO_GENERATOR_H
