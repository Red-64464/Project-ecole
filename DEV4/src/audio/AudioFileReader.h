/**
 * @brief WAV file loader.
 *
 * Loads a .wav file from disk and converts it to a float stereo 44100 Hz
 * buffer ready to be played by the audio engine.
 */

#ifndef AUDIO_FILE_READER_H
#define AUDIO_FILE_READER_H

#include <vector>      // std::vector to hold the audio samples.
#include <string_view> // Pass a string without copying it.

class AudioFileReader {
public:
    /**
     * @brief Load a .wav file and convert it to interleaved float stereo.
     *
     * SDL reads the file, converts it to float stereo at @p sampleRate Hz,
     * and the result is copied into @p outBuffer.
     *
     * @param path        Path to the .wav file.
     * @param sampleRate  Target sample rate (44100 Hz for this project).
     * @param outBuffer   Vector of floats filled with the converted audio.
     * @return            true on success, false otherwise.
     */
    static bool loadFile(std::string_view path, int sampleRate,
                         std::vector<float>& outBuffer);
};

#endif // AUDIO_FILE_READER_H
