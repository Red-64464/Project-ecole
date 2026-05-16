/**
 * @brief Audio engine implementation.
 *
 * Starts PortAudio, configures the output and exposes the callback
 * invoked ~172 times per second to produce real-time audio.
 */

#include "AudioEngine.h"
#include "core/DrumMachine.h" // Access to the shared drum machine state.
#include <SDL3/SDL.h>         // SDL_Log for error reporting.

/**
 * @brief Prepare and start the audio engine.
 * @param drumMachine Pointer to the shared DrumMachine.
 *
 * Steps:
 *   1) Pa_Initialize() boots PortAudio.
 *   2) outputParams configures the output (stereo, float32, low latency).
 *   3) Pa_OpenStream() creates the stream with our callback.
 *   4) Pa_StartStream() starts the stream (the callback begins firing).
 */
void AudioEngine::init(DrumMachine* drumMachine) {
    this->drumMachine = drumMachine;

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        SDL_Log("PortAudio init failed: %s", Pa_GetErrorText(err));
        return;
    }

    PaStreamParameters outputParams;
    outputParams.device = Pa_GetDefaultOutputDevice();
    if (outputParams.device == paNoDevice) {
        SDL_Log("No default audio output device found");
        return;
    }

    outputParams.channelCount = NUM_CHANNELS;       // 2 channels = stereo (left + right).
    outputParams.sampleFormat = paFloat32;          // 32-bit float per sample.
    outputParams.suggestedLatency =
        Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = nullptr;

    // Pa_OpenStream creates the audio stream.
    // We pass `this` as userData so the callback can recover the AudioEngine.
    err = Pa_OpenStream(&stream, nullptr, &outputParams,
                        SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paNoFlag, audioCallback, this);
    if (err != paNoError) {
        SDL_Log("PortAudio open stream failed: %s", Pa_GetErrorText(err));
        return;
    }

    Pa_StartStream(stream); // The callback now fires periodically.
}

/**
 * @brief Cleanly stop and close the audio engine.
 *
 * Called when the application exits.
 */
void AudioEngine::shutdown() {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        stream = nullptr;
    }
    Pa_Terminate();
}

/**
 * @brief Load a WAV file into one AudioPlayer.
 * @param track Track index (0 to NUM_TRACKS - 1).
 * @param path  Path to the WAV file.
 * @return true on success.
 *
 * @note AudioPlayer::loadFile uses a mutex internally so the audio thread
 * keeps producing sound during the buffer swap.
 */
bool AudioEngine::loadFile(int track, std::string_view path) {
    if (track < 0 || track >= NUM_TRACKS) return false;
    return players[track].loadFile(path);
}

/**
 * @brief Toggle between Play and Stop.
 */
void AudioEngine::togglePlay() {
    if (drumMachine->playing.load()) {
        sequencer.stop(drumMachine);
    } else {
        sequencer.start(drumMachine);
    }
}

/**
 * @brief Audio callback invoked automatically by PortAudio (~172 times/sec).
 *
 * Heart of the audio system. PortAudio asks for 256 frames per call.
 *
 * Steps:
 *   1) Sequencer  : advance the steps and trigger sounds.
 *   2) AudioPlayer: each track fills its buffer (volume + mute applied).
 *   3) Delay      : echo applied to each track buffer.
 *   4) Mixer      : combine the 4 tracks into the output buffer.
 *
 * @return paContinue to keep the stream running.
 */
int AudioEngine::audioCallback(const void* /*input*/, void* output,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* /*timeInfo*/,
                                PaStreamCallbackFlags /*statusFlags*/,
                                void* userData) {
    auto* engine = static_cast<AudioEngine*>(userData);
    auto* out    = static_cast<float*>(output);
    auto* dm     = engine->drumMachine;
    int   frames = static_cast<int>(framesPerBuffer);

    // 1) Sequencer advances the steps and triggers any active sound.
    engine->sequencer.process(dm, engine->players);

    // 2 + 3) Each track produces its sound, then the echo is applied.
    float trackBuffers[NUM_TRACKS][BUFFER_SIZE];
    for (int track = 0; track < NUM_TRACKS; track++) {
        engine->players[track].process(trackBuffers[track], frames,
                                       dm->volume[track].load(),
                                       dm->muted[track].load());

        engine->delays[track].process(trackBuffers[track], frames,
                                      dm->delayTime[track].load(),
                                      dm->delayMix[track].load(),
                                      dm->delayEnabled[track].load());
    }

    // 4) Sum the 4 track buffers into the output buffer.
    engine->mixer.process(out,
                          trackBuffers[0], trackBuffers[1],
                          trackBuffers[2], trackBuffers[3], frames);

    return paContinue;
}
