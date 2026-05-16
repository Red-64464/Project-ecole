/**
 * @brief Step sequencer implementation.
 *
 * Maintains the beat accumulator on the audio thread and triggers the
 * sounds whose grid cell is active when the cursor moves to the next step.
 */

#include "StepSequencer.h"
#include "core/Constants.h"
#include "core/DrumMachine.h"
#include "audio/AudioPlayer.h"

void StepSequencer::start(DrumMachine* drumMachine) {
    drumMachine->currentStep.store(0);
    startRequested.store(true);
    drumMachine->playing.store(true);
}

void StepSequencer::stop(DrumMachine* drumMachine) {
    drumMachine->playing.store(false);
    drumMachine->currentStep.store(0);
    stopRequested.store(true);
}

void StepSequencer::process(DrumMachine* drumMachine, AudioPlayer players[NUM_TRACKS]) {
    // Handle the start/stop flags raised by the UI thread.
    if (startRequested.load()) {
        startRequested.store(false);
        beatAccumulator = 0.0;
        needsFirstBeat = true;
    }

    if (stopRequested.load()) {
        stopRequested.store(false);
        beatAccumulator = 0.0;
        needsFirstBeat = false;
    }

    if (!drumMachine->playing.load()) return;

    // Trigger the first step immediately on Play so the user does not
    // have to wait one full step duration before hearing anything.
    if (needsFirstBeat) {
        needsFirstBeat = false;
        int step = drumMachine->currentStep.load();
        for (int track = 0; track < NUM_TRACKS; track++) {
            if (drumMachine->grid[track][step].load()) {
                players[track].start(drumMachine->reverse[track].load());
            }
        }
    }

    // Compute how much time has elapsed since the previous callback.
    // 256 frames / 44100 Hz = 0.0058 second per callback.
    const double bufferDuration =
        static_cast<double>(FRAMES_PER_BUFFER) / SAMPLE_RATE;

    double bpm = static_cast<double>(drumMachine->bpm.load());

    // Convert the elapsed seconds into beats:
    //   beats = seconds * BPM / 60
    // At 120 BPM, one second = 2 beats, so 0.0058 s = 0.01161 beat.
    double durationInBeats = (bufferDuration * bpm) / 60.0;

    beatAccumulator += durationInBeats;

    // Advance the step cursor as many times as needed. A while loop is
    // used in case a very high BPM (or large buffer) crosses several
    // step boundaries in a single callback.
    while (beatAccumulator >= STEP_DURATION) {
        beatAccumulator -= STEP_DURATION;

        // Cycle through the 16 steps: (15 + 1) % 16 = 0.
        int step = (drumMachine->currentStep.load() + 1) % NUM_STEPS;
        drumMachine->currentStep.store(step);

        // Trigger every active track on the new step.
        for (int track = 0; track < NUM_TRACKS; track++) {
            if (drumMachine->grid[track][step].load()) {
                players[track].start(drumMachine->reverse[track].load());
            }
        }
    }
}
