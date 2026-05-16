/**
 * @brief The main audio engine — conductor of all the sound.
 *
 * Bridges PortAudio (the library that talks to the speakers) with every
 * audio module of the project (players, sequencer, delays, mixer).
 *
 * @note audioCallback() is a special function called automatically by
 * PortAudio about 172 times per second (44100 / 256) to ask for the
 * next chunk of audio data to play through the speakers.
 */

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "portaudio.h"           // PortAudio library for audio output.
#include "audio/AudioPlayer.h"   // Per-track audio player.
#include "audio/StepSequencer.h" // Step sequencer.
#include "audio/Delay.h"         // Delay (echo) effect.
#include "audio/Mixer.h"         // Mixer that sums the tracks.
#include "core/Constants.h"      // NUM_TRACKS and friends.
#include <string_view>           // Pass a string without copying it.

struct DrumMachine; // Forward declaration — full definition not needed here.

class AudioEngine {
public:
    /**
     * @brief Initialize PortAudio and start the audio stream.
     *
     * Steps: initialize PortAudio, query the default output device,
     * open the stream with audioCallback, then start the stream.
     *
     * @param drumMachine Pointer to the DrumMachine instance shared with the UI.
     */
    void init(DrumMachine* drumMachine);

    /**
     * @brief Stop the audio stream and release every PortAudio resource.
     *
     * Calls Pa_StopStream(), Pa_CloseStream() then Pa_Terminate().
     * Must be called when the application is closing.
     */
    void shutdown();

    /**
     * @brief Load a WAV file into the requested track.
     *
     * Loading happens on the main thread — the WAV files used for drum hits
     * are short and decode quickly. AudioPlayer guards its buffer with a
     * mutex so the audio thread is never disturbed during the swap.
     *
     * @param track Track index (0 to NUM_TRACKS - 1).
     * @param path  Path to the WAV file.
     * @return      true on success, false otherwise.
     */
    bool loadFile(int track, std::string_view path);

    /**
     * @brief Toggle between Play and Stop.
     *
     * Calls sequencer.stop() if playback is running, sequencer.start() otherwise.
     */
    void togglePlay();

private:
    /**
     * @brief Audio callback invoked automatically by PortAudio (~172 times/sec).
     *
     * Heart of the audio system. Each call:
     *   1. advances the step sequencer (timing + triggering)
     *   2. fills one buffer per track via the AudioPlayers
     *   3. applies the delay effect on each track
     *   4. mixes the 4 tracks into a single output buffer
     *
     * @note Declared static because PortAudio is a C library and cannot call
     * member functions directly. The AudioEngine instance is retrieved via
     * the @c userData parameter passed to Pa_OpenStream.
     *
     * @return paContinue to keep the stream alive.
     */
    static int audioCallback(const void* input, void* output,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData);

    PaStream* stream{nullptr};         ///< PortAudio stream handle (the link to the speakers).
    DrumMachine* drumMachine{nullptr}; ///< Shared drum machine state.
    AudioPlayer players[NUM_TRACKS];   ///< One audio player per track.
    StepSequencer sequencer;           ///< Timing and triggering of sounds.
    Delay delays[NUM_TRACKS];          ///< One delay effect per track.
    Mixer mixer;                       ///< Combines the 4 tracks into one output buffer.
};

#endif // AUDIO_ENGINE_H
