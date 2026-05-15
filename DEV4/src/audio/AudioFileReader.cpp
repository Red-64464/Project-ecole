/**
 * @brief Lecteur de fichiers WAV.
 *
 * Tu donnes un fichier .wav, ce fichier le lit,
 * le convertit dans le bon format et te le rend pret a etre joue.
 *
 */

#include "AudioFileReader.h" // Le fichier.h
#include "core/Constants.h"   // NUM_CHANNELS, SAMPLE_RATE
#include <SDL3/SDL.h>         // La bibliothèque SDL , c'est elle qui fait le vrai travail de lire le wav , le convertir ..
#include <string>             // On include string car SDL ne sait pas voir une string_view , il veut un vrai texte C (char*), on convertit le chemin du fichier en string pour ça


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
bool AudioFileReader::loadFile(std::string_view path, int sampleRate, std::vector<float>& outBuffer) {

    std::string pathStr(path); // Le chemin du fichir WAV , va arriver en StringView donc on va le convertir en string normal pour que SDL puisse le lire car il ne list pas les string_view , 

    SDL_AudioSpec spec; // Une fiche technique qui va extraire les informations du header du fichier wav (ex: combien de canaux, quel format, etc)
    Uint8* rawBuf = nullptr; // Un pointeur vers le son brut , SDL plus tard va le remplir avec les données du fichier wav
    // SDL va allouer de la RAM pour stocker le son brut du fichier wav 
    // Ensuite SDL va ensuite donner l'adresse du son qui se trouve en mémoire à rawBuf 
    Uint32 rawLen = 0; // Cela sera la taille du son brut en bytes (octets) , SDL va aussi le remplir

   if (!SDL_LoadWAV(pathStr.c_str(), &spec, &rawBuf, &rawLen)) { // SDL va lire le fichier wav et extraire les données qui se trouve dedans 
   
        SDL_Log("AudioFileReader: Failed to load WAV: %s", SDL_GetError());
        return false;

    }


    SDL_AudioSpec dstSpec; // C'est une variable qui va contenir les infos du format de sortie que nous on veut (ex: stéréo, 44100 Hz, etc)
    dstSpec.format   = SDL_AUDIO_F32;    // le format des nombres on veut des floats 
    dstSpec.channels = NUM_CHANNELS;     // Le nombre de canaux ( il y en a 2 ) 
    dstSpec.freq     = sampleRate;       // Le tauc d'échantillonnage (44100 Hz)

    Uint8* floatBuf = nullptr; // Un pointeur vers le son converti (en float stéréo 44100 Hz) , SDL va allouer de la RAM pour stocker le son converti et donner l'adresse à floatBuf
    int floatLen    = 0; // Cela sera la taille du son converti en bytes (octets) , SDL va aussi le remplir
    // & dstSpec : on donne à SDL les infos du format de sortie que nous on veut
    // spec contient les informations du son de base 
    // rawBuf contient le son brut du fichier wav
    // static_cast<int>(rawLen) convertit la taille du son brut de Uint32 à int (car SDL_ConvertAudioSamples attend un int pour la taille)
    // Voici le format que l'on veut en sortie avec toutes les informations (stéréo, 44100 Hz, float)
    if (!SDL_ConvertAudioSamples(&spec, rawBuf, static_cast<int>(rawLen),
                                  &dstSpec, &floatBuf, &floatLen)) {

        SDL_Log("AudioFileReader: Failed to convert audio: %s", SDL_GetError());
        SDL_free(rawBuf);
        return false;
    }

    SDL_free(rawBuf); // Libérer la mémoire du son brut (on n'en a plus besoin)


    // On va calculer combien de float dans le buffer 
    int sampleCount = floatLen / static_cast<int>(sizeof(float));
    //  FloatBuf de base est un tableau d'octet brut , on va le convertir en tableau de float pour pouvoir le copier dans outBuffer
    auto* floatData = reinterpret_cast<float*>(floatBuf);
    // On va copier touts les float de floatData dans la mémoire pour les mettre dans le buffer avec la méthode assign
    // On met floatData pour commencer du début et floatData + sampleCount pour aller jusqu'à la fin du buffer de float (car sampleCount c'est le nombre de float dans le buffer) 
    outBuffer.assign(floatData, floatData + sampleCount);

    SDL_free(floatBuf); //  Libérer la mémoire temporaire car plus besoin
    return true;        


}