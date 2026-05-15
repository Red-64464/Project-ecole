#ifndef AUDIO_FILE_READER_H
#define AUDIO_FILE_READER_H

#include <vector> // std::vector (pour stocker les échantillons audio) c'est une liste
#include <string_view> // C'est une manière légère de passer des chaînes de caractères (comme std::string mais sans faire de copie)

/**
 * @brief Lecteur de fichiers WAV.
 *
 * Charge un fichier .wav depuis le disque et le convertit
 * en buffer float stereo 44100 Hz pret a etre joue par le moteur audio.
 *
 */

 class AudioFileReader {
    public:
    /**
     * @brief  Charge un fichier .wav et le convertit en float stéréo prêt à être joué.
     *
     * SDL lit le fichier, le convertit en float stéréo 44100 Hz,
     * puis copie le résultat dans outBuffer.
     *
     * @param path        Chemin vers le fichier .wav
     * @param sampleRate  Vitesse de lecture cible (44100 Hz dans notre projet)
     * @param outBuffer   Liste de floats remplie avec le son converti (passée par référence)
     * @return            true si le chargement a réussi, false sinon
     */
    static bool loadFile(std::string_view path , int sampleRate , std::vector<float>& outBuffer);
 };
 
#endif 
