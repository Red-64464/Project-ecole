/**
 * @file MainWindow.cpp
 * @brief Initialization, main loop and file handling.
 *
 * This file contains:
 *   - SDL, ImGui and audio engine initialization;
 *   - the main application loop;
 *   - preferences loading at startup;
 *   - the open-file dialog handling.
 *
 * The drawing routines live in MainWindowDraw.cpp,
 * MainWindowSequencer.cpp and MainWindowInspector.cpp.
 */

#include "MainWindow.h"
#include "core/Preferences.h"
#include <thread>
#include <chrono>
#include <string>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

/// Target frame rate of the UI loop.
static constexpr float FRAMERATE = 60.0f;

/// Target frame duration (1 second / FRAMERATE).
static const std::chrono::duration<double, std::milli>
    TARGET_FRAMETIME(1000.0 / FRAMERATE);

/**
 * @brief Extract the file name from a full path.
 *
 * Example: "C:/sounds/kick.wav" returns "kick.wav".
 *
 * @param path Full file path.
 * @return File name only.
 */
static std::string getFileName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos)
        return path.substr(pos + 1);
    return path;
}

/**
 * @brief Callback invoked by SDL when the user picks a file.
 *
 * Stores the chosen path in the drum machine so the main thread can
 * pick it up on the next frame.
 *
 * @param userdata Pointer to our DrumMachine.
 * @param filelist NULL-terminated list of chosen files.
 */
static void onFileDialogResult(void* userdata, const char* const* filelist,
                               int /*filter*/) {
    auto* drumMachine = static_cast<DrumMachine*>(userdata);
    if (filelist && filelist[0]) {
        drumMachine->pendingFilePath = filelist[0];
        drumMachine->fileDialogComplete = true;
    }
}

void MainWindow::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Error: SDL_Init(): %s", SDL_GetError());
        return;
    }

    // Hidden first so the user does not see the window being built; we
    // show it after positioning it in the centre of the screen.
    window = SDL_CreateWindow("Drum Machine", 1100, 400,
                              SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Error: SDL_CreateWindow(): %s", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s", SDL_GetError());
        return;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    audioEngine.init(&drumMachine);

    loadPreferences();
}

void MainWindow::loadPreferences() {
    Preferences::load(drumMachine.filePath);

    for (int track = 0; track < NUM_TRACKS; track++) {
        if (!drumMachine.filePath[track].empty()) {
            if (audioEngine.loadFile(track, drumMachine.filePath[track]))
                drumMachine.fileName[track] = getFileName(drumMachine.filePath[track]);
            else
                drumMachine.filePath[track].clear();
        }
    }
}

void MainWindow::openFileDialog() {
    static const SDL_DialogFileFilter wavFilter[] = { { "WAV files", "wav" } };

    SDL_ShowOpenFileDialog(onFileDialogResult, &drumMachine, window,
                           wavFilter, 1, nullptr, false);
}

void MainWindow::handleFileDialogResult() {
    if (!drumMachine.fileDialogComplete) return;
    drumMachine.fileDialogComplete = false;

    int track = drumMachine.selectedTrack;
    if (audioEngine.loadFile(track, drumMachine.pendingFilePath)) {
        drumMachine.filePath[track] = drumMachine.pendingFilePath;
        drumMachine.fileName[track] = getFileName(drumMachine.filePath[track]);
        Preferences::save(drumMachine.filePath);
    }
}

void MainWindow::run() {
    // Background clear colour (grey-blue).
    const auto clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        // Pump every pending event into ImGui.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Begin a new ImGui frame.
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        draw();

        ImGui::Render();

        SDL_SetRenderDrawColorFloat(renderer, clearColor.x, clearColor.y,
                                    clearColor.z, clearColor.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        // Cap the UI loop at FRAMERATE.
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = frameEnd - frameStart;
        if (elapsed < TARGET_FRAMETIME)
            std::this_thread::sleep_for(TARGET_FRAMETIME - elapsed);
    }

    // Release every resource in reverse creation order.
    audioEngine.shutdown();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
