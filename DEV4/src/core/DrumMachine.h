/**
 * @brief Central data structure holding the whole state of the drum machine.
 *
 * Contains the sequencer grid, the BPM, per-track volumes, loaded files
 * and effect parameters (delay, reverse).
 *
 * @note Members shared with the audio thread use @c std::atomic so that
 * reads and writes between the UI thread and the audio thread never race.
 */

#ifndef DRUM_MACHINE_H
#define DRUM_MACHINE_H

#include <atomic>
#include <string>
#include "Constants.h"

/**
 * @brief Main drum machine state.
 *
 * Accessed both by the graphical interface and by the audio thread.
 * Atomic members are thread-safe; the others are UI-only.
 */
struct DrumMachine
{
    /// Tempo in beats per minute. Default: 120 BPM.
    std::atomic<int> bpm{120};

    /// @c true if playback is running, @c false if stopped.
    std::atomic<bool> playing{false};

    /// Current sequencer position (0 to NUM_STEPS - 1).
    std::atomic<int> currentStep{0};

    /// Sequencer grid: @c true if the step triggers the sound, @c false otherwise.
    /// Dimensions: NUM_TRACKS x NUM_STEPS = 4 x 16 cells.
    std::atomic<bool> grid[NUM_TRACKS][NUM_STEPS];

    /// Per-track volume. Range: 0.0 (silent) to 1.0 (max).
    std::atomic<float> volume[NUM_TRACKS];

    /// @c true if the track is muted (silent output).
    std::atomic<bool> muted[NUM_TRACKS];

    /// @c true if the track is played in reverse.
    std::atomic<bool> reverse[NUM_TRACKS];

    /// @c true if the delay effect is enabled on this track.
    std::atomic<bool> delayEnabled[NUM_TRACKS];

    /// Delay duration in seconds. Range: 0.0 to 1.0.
    std::atomic<float> delayTime[NUM_TRACKS];

    /// Delay mix level. Range: 0.0 (no echo) to 1.0 (loud echo).
    std::atomic<float> delayMix[NUM_TRACKS];

    /// Full path of the WAV file loaded for each track.
    std::string filePath[NUM_TRACKS];

    /// Filename only (no directory), shown in the UI.
    std::string fileName[NUM_TRACKS];

    /// Index of the track currently selected in the UI (0 to NUM_TRACKS - 1).
    int selectedTrack{0};

    /// Path of the file picked through the open dialog, waiting to be loaded.
    std::string pendingFilePath;

    /// @c true when the user has just confirmed a file in the open dialog.
    bool fileDialogComplete{false};

    /// Constructor: initializes every field to its default value.
    DrumMachine();
};

#endif // DRUM_MACHINE_H
