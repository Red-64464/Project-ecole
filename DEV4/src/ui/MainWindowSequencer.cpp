/**
 * @file MainWindowSequencer.cpp
 * @brief Draws the sequencer grid (track buttons + step grid + volume + mute).
 */

#include "MainWindow.h"
#include <string>
#include "imgui.h"

static const ImVec4 COLOR_TURQUOISE(0.0f, 0.8f, 0.8f, 1.0f);   ///< Active step.
static const ImVec4 COLOR_YELLOW(1.0f, 0.9f, 0.0f, 1.0f);      ///< Step being played.
static const ImVec4 COLOR_ORANGE(1.0f, 0.5f, 0.0f, 1.0f);      ///< Selected / muted highlight.
static const ImVec4 COLOR_LIGHT_GRAY(0.85f, 0.85f, 0.85f, 1.0f); ///< Idle step.

static const char* SEQ_TRACK_NAMES[] = {
    "Track 1", "Track 2", "Track 3", "Track 4"
};

void MainWindow::drawSequencerGrid() {
    int currentStep = drumMachine.currentStep.load();
    bool isPlaying = drumMachine.playing.load();

    for (int track = 0; track < NUM_TRACKS; track++) {
        bool isSelected = (drumMachine.selectedTrack == track);

        // Highlight the selected track button in orange.
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_ORANGE);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_ORANGE);
        }

        // The "##sel" suffix gives ImGui a unique id without showing it.
        std::string selLabel = std::string(SEQ_TRACK_NAMES[track]) + "##sel";

        if (ImGui::Button(selLabel.c_str(), ImVec2(70, 25)))
            drumMachine.selectedTrack = track;

        if (isSelected) ImGui::PopStyleColor(2);

        // 16 step buttons per track.
        for (int step = 0; step < NUM_STEPS; step++) {
            ImGui::SameLine(0, 2);

            bool isActive = drumMachine.grid[track][step].load();
            bool isCurrent = (step == currentStep && isPlaying);

            ImVec4 color = COLOR_LIGHT_GRAY;
            if (isCurrent)     color = COLOR_YELLOW;
            else if (isActive) color = COLOR_TURQUOISE;

            ImGui::PushStyleColor(ImGuiCol_Button, color);
            // Slightly darker on hover (multiply RGB by 0.85).
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                ImVec4(color.x * 0.85f, color.y * 0.85f, color.z * 0.85f, 1.0f));

            std::string stepLabel = "##s" + std::to_string(track) +
                                    "_" + std::to_string(step);

            if (ImGui::Button(stepLabel.c_str(), ImVec2(25, 25)))
                drumMachine.grid[track][step].store(!isActive);

            ImGui::PopStyleColor(2);
        }

        // Volume slider after the step grid.
        ImGui::SameLine();
        ImGui::PushItemWidth(80);

        float volume = drumMachine.volume[track].load();
        std::string volLabel = "##vol" + std::to_string(track);

        if (ImGui::SliderFloat(volLabel.c_str(), &volume, 0.0f, 1.0f, "%.2f"))
            drumMachine.volume[track].store(volume);

        ImGui::PopItemWidth();

        // Mute button (orange when muted).
        ImGui::SameLine();
        bool isMuted = drumMachine.muted[track].load();

        if (isMuted) {
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_ORANGE);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_ORANGE);
        }

        std::string muteLabel = "Mute##" + std::to_string(track);

        if (ImGui::Button(muteLabel.c_str()))
            drumMachine.muted[track].store(!isMuted);

        if (isMuted) ImGui::PopStyleColor(2);
    }
}
