/**
 * @brief DrumMachine constructor implementation.
 *
 * Initializes every per-track default value.
 */

#include "DrumMachine.h"

/**
 * @brief Default constructor.
 *
 * Sets every track to its initial state: empty grid, full volume,
 * mute / reverse / delay disabled, sensible default delay parameters.
 */
DrumMachine::DrumMachine() {
    for (int track = 0; track < NUM_TRACKS; track++) {
        // Reset every grid cell to false (no step active by default).
        // .store() is required for atomic<bool> to avoid data races.
        for (int step = 0; step < NUM_STEPS; step++) {
            grid[track][step].store(false);
        }

        volume[track].store(1.0f);        // Full volume.
        muted[track].store(false);        // Track is audible.
        reverse[track].store(false);      // Forward playback.
        delayEnabled[track].store(false); // Delay effect off.
        delayTime[track].store(0.3f);     // 0.3 second default delay time.
        delayMix[track].store(0.5f);      // 50% wet / 50% dry default mix.
    }
}
