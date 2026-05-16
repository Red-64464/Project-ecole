/**
 * @brief Audio player for one drum machine track.
 *
 * Loads a WAV file, starts playback on demand and fills the audio buffer
 * sample by sample in interleaved float stereo at 44100 Hz.
 */

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <vector>      // Holds the loaded audio samples.
#include <string_view> // Pass a string without copying it.
#include <mutex>       // Guards the buffer when the file is being replaced.

class AudioPlayer {
public:
    /**
     * @brief Load a WAV file from disk into the player.
     *
     * The file is converted to interleaved float stereo at 44100 Hz.
     *
     * @param path Path to the WAV file.
     * @return true on success.
     */
    bool loadFile(std::string_view path);

    /**
     * @brief Start playing the loaded sound from the beginning.
     *
     * Uses try_lock so the audio thread is never blocked: if the buffer
     * is currently being swapped, this call simply does nothing.
     *
     * @param reversePlay true to play the sound backwards.
     */
    void start(bool reversePlay);

    /**
     * @brief Fill the output buffer with the next chunk of audio.
     *
     * Called by AudioEngine on the audio thread for each callback.
     *
     * @param output  Output buffer to fill (interleaved stereo).
     * @param frames  Number of frames to produce (typically 256).
     * @param volume  Track volume from 0.0 (silent) to 1.0 (max).
     * @param muted   true to write silence regardless of the buffer.
     */
    void process(float* output, int frames, float volume, bool muted);

private:
    /// Interleaved stereo samples loaded from the WAV file (L, R, L, R, ...).
    std::vector<float> buffer;

    /// Current read position in the buffer, expressed in frames.
    int position{0};

    /// true while the loaded sound is still playing.
    bool playing{false};

    /// true if the sample is being played backwards.
    bool reverse{false};

    /// Mutex that guards `buffer` against concurrent reads (audio thread)
    /// and writes (loadFile on the main thread).
    std::mutex bufferMutex;
};

#endif // AUDIO_PLAYER_H
