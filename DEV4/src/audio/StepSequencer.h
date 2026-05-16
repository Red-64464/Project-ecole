/**
 * @brief Step sequencer — the rhythmic conductor of the drum machine.
 *
 * Decides WHEN to trigger each sound based on the tempo.
 *
 * How it works:
 *   - The sequencer has 16 steps that cycle endlessly.
 *   - For each step, it checks which tracks are enabled.
 *   - If a track is enabled on the current step, the sound is triggered.
 *   - The cycling speed depends on the BPM (beats per minute).
 */

#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#include <atomic>            // Atomic flags shared between threads.
#include "core/Constants.h"  // NUM_TRACKS

// Forward declarations to avoid pulling extra headers here.
struct DrumMachine;
class AudioPlayer;

/**
 * @brief Step sequencer driving timing and sound triggering.
 *
 * Fed by the audio thread (~172 times per second) through process(),
 * controlled by the UI thread through start() and stop().
 */
class StepSequencer {
public:
    /**
     * @brief Process one audio buffer: maybe advance the step,
     *        and trigger sounds when needed.
     *
     * Called for every audio callback (~172 times per second at 44100 / 256).
     * Accumulates the elapsed time and advances by one step whenever the
     * step duration (STEP_DURATION = 0.25 beat) is reached.
     *
     * @param drumMachine Pointer to the shared drum machine state.
     * @param players     Array of the 4 audio players, one per track.
     */
    void process(DrumMachine* drumMachine, AudioPlayer players[NUM_TRACKS]);

    /**
     * @brief Start playback (called when the UI clicks Play).
     *
     * Resets the cursor to step 0, sets playing to true and raises
     * startRequested so the audio thread can prepare the first beat.
     */
    void start(DrumMachine* drumMachine);

    /**
     * @brief Stop playback (called when the UI clicks Stop).
     *
     * Sets playing to false, resets the cursor to step 0 and raises
     * stopRequested so the audio thread resets its counters.
     */
    void stop(DrumMachine* drumMachine);

private:
    /// Flag raised by the UI when the user clicks Play.
    /// Atomic because it is written by the UI thread and read by the audio thread.
    std::atomic<bool> startRequested{false};

    /// Flag raised by the UI when the user clicks Stop.
    /// Atomic for the same reason as startRequested.
    std::atomic<bool> stopRequested{false};

    /// Time elapsed (in beats) since the last step transition.
    ///
    /// Works like a bucket that fills as time passes: each call to process()
    /// adds a fraction of a beat. When it crosses STEP_DURATION (0.25), we
    /// advance one step and subtract 0.25 (without resetting to 0) so the
    /// remainder is kept and timing stays precise over time.
    ///
    /// Audio-thread-only, no atomic needed.
    double beatAccumulator{0.0};

    /// Signal asking process() to trigger the first step immediately on Play.
    ///
    /// Without this flag the user would have to wait STEP_DURATION (~120 ms)
    /// before hearing the first sound. Audio-thread-only.
    bool needsFirstBeat{false};
};

#endif // STEP_SEQUENCER_H
