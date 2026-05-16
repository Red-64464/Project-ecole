/**
 * @brief Main window declaration.
 *
 * Owns the SDL window, the ImGui context and bridges the UI with
 * the audio engine.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SDL3/SDL.h>          // SDL window and renderer types.
#include "core/DrumMachine.h"  // Shared drum machine state.
#include "audio/AudioEngine.h" // Audio engine.

class MainWindow {
public:
    /**
     * @brief Initialize SDL, ImGui, the audio engine and load preferences.
     */
    void init();

    /**
     * @brief Run the main application loop.
     */
    void run();

private:
    /// Draw every UI element for the current frame.
    void draw();

    /// Draw the playback controls (Play/Stop + BPM buttons).
    void drawPlaybackControls();

    /// Draw the sequencer grid (4 tracks x 16 steps + volume + mute).
    void drawSequencerGrid();

    /// Draw the inspector panel for the selected track.
    void drawInspector();

    /// Handle the result of the WAV file dialog when one is pending.
    void handleFileDialogResult();

    /// Load the WAV paths saved in preferences.txt at startup.
    void loadPreferences();

    /// Open the native file dialog to pick a WAV file.
    void openFileDialog();

    SDL_Window*   window{nullptr};     ///< Main SDL window.
    SDL_Renderer* renderer{nullptr};   ///< SDL renderer used by ImGui.
    DrumMachine   drumMachine;         ///< Drum machine state.
    AudioEngine   audioEngine;         ///< Audio engine producing the sound.
};

#endif // MAINWINDOW_H
