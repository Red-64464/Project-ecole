/**
 * @brief WAV file loader implementation.
 *
 * Reads the file with SDL, converts it to the target format
 * (float, stereo, 44100 Hz) and copies the samples into the
 * output buffer.
 */

#include "AudioFileReader.h"
#include "core/Constants.h" // NUM_CHANNELS, SAMPLE_RATE
#include <SDL3/SDL.h>       // SDL handles the actual WAV decoding.
#include <string>           // SDL takes a const char*, so we need a real std::string.

bool AudioFileReader::loadFile(std::string_view path, int sampleRate,
                               std::vector<float>& outBuffer) {
    // SDL_LoadWAV expects a null-terminated C string.
    std::string pathStr(path);

    SDL_AudioSpec spec;       // Format read from the WAV header.
    Uint8* rawBuf = nullptr;  // Raw samples allocated by SDL.
    Uint32 rawLen = 0;        // Size of the raw buffer in bytes.

    if (!SDL_LoadWAV(pathStr.c_str(), &spec, &rawBuf, &rawLen)) {
        SDL_Log("AudioFileReader: Failed to load WAV: %s", SDL_GetError());
        return false;
    }

    // Target format: interleaved float stereo at the requested sample rate.
    SDL_AudioSpec dstSpec;
    dstSpec.format   = SDL_AUDIO_F32;
    dstSpec.channels = NUM_CHANNELS;
    dstSpec.freq     = sampleRate;

    Uint8* floatBuf = nullptr;
    int floatLen    = 0;
    if (!SDL_ConvertAudioSamples(&spec, rawBuf, static_cast<int>(rawLen),
                                 &dstSpec, &floatBuf, &floatLen)) {
        SDL_Log("AudioFileReader: Failed to convert audio: %s", SDL_GetError());
        SDL_free(rawBuf);
        return false;
    }

    SDL_free(rawBuf); // Raw samples no longer needed.

    // Reinterpret the converted byte buffer as a sequence of floats and
    // copy it into the output vector. The vector then owns the samples.
    int sampleCount = floatLen / static_cast<int>(sizeof(float));
    auto* floatData = reinterpret_cast<float*>(floatBuf);
    outBuffer.assign(floatData, floatData + sampleCount);

    SDL_free(floatBuf);
    return true;
}
