/**
 * @brief AudioPlayer implementation.
 *
 * Loads a WAV file, starts playback and produces audio samples for the
 * audio engine. Buffer access is protected with a mutex so the audio
 * thread is never blocked by the main thread loading a new file.
 */

#include "AudioPlayer.h"
#include "AudioFileReader.h"
#include "core/Constants.h" // SAMPLE_RATE, NUM_CHANNELS
#include <algorithm>        // std::fill

bool AudioPlayer::loadFile(std::string_view path) {
    // Delegate the actual loading and SDL conversion to AudioFileReader.
    std::vector<float> tempBuffer;
    if (!AudioFileReader::loadFile(path, SAMPLE_RATE, tempBuffer))
        return false;

    // Lock the mutex while we swap the buffer. The audio thread uses
    // try_lock so it will simply emit silence during this brief swap
    // instead of waiting for us.
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        playing = false;              // Stop any in-progress playback.
        buffer  = std::move(tempBuffer); // Move avoids a copy of the samples.
    }
    return true;
}

void AudioPlayer::start(bool reversePlay) {
    // Try-lock so we never block the audio thread.
    std::unique_lock<std::mutex> lock(bufferMutex, std::try_to_lock);

    // If we could not get the lock, or no file is loaded, do nothing.
    if (!lock.owns_lock() || buffer.empty()) return;

    reverse = reversePlay;

    // Number of stereo frames in the buffer (each frame = 2 floats).
    int totalFrames = static_cast<int>(buffer.size()) / NUM_CHANNELS;

    // Reverse playback starts at the end, forward playback at the start.
    position = reverse ? (totalFrames - 1) : 0;

    playing = true;
}

void AudioPlayer::process(float* output, int frames, float volume, bool muted) {
    // Try-lock so the audio thread is never stalled by a concurrent load.
    std::unique_lock<std::mutex> lock(bufferMutex, std::try_to_lock);

    // No lock, not playing, or no file: write silence and return early.
    if (!lock.owns_lock() || !playing || buffer.empty()) {
        std::fill(output, output + frames * NUM_CHANNELS, 0.0f);
        return;
    }

    int totalFrames = static_cast<int>(buffer.size()) / NUM_CHANNELS;

    for (int i = 0; i < frames; i++) {
        // Sample finished playing: emit silence for the rest of the buffer.
        if (!playing) {
            output[i * 2]     = 0.0f; // Left channel.
            output[i * 2 + 1] = 0.0f; // Right channel.
            continue;
        }

        if (muted) {
            output[i * 2]     = 0.0f;
            output[i * 2 + 1] = 0.0f;
        } else {
            // Apply the per-track volume to both stereo channels.
            output[i * 2]     = buffer[position * 2]     * volume;
            output[i * 2 + 1] = buffer[position * 2 + 1] * volume;
        }

        // Advance (or rewind) the read position.
        if (reverse) {
            position--;
            if (position < 0) playing = false;
        } else {
            position++;
            if (position >= totalFrames) playing = false;
        }
    }
}
