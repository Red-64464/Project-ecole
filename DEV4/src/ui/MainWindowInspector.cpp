/**
 * @file MainWindowInspector.cpp
 * @brief Draws the inspector panel for the selected track.
 */

#include "MainWindow.h"
#include <string>
#include "imgui.h"

static const ImVec4 INSP_COLOR_BLUE(0.2f, 0.4f, 0.8f, 1.0f);
static const ImVec4 INSP_COLOR_BLUE_HOVER(0.3f, 0.5f, 0.9f, 1.0f);

static const char* INSP_TRACK_NAMES[] = {
    "Track 1", "Track 2", "Track 3", "Track 4"
};

void MainWindow::drawInspector() {
    int track = drumMachine.selectedTrack;

    ImGui::Text("%s", INSP_TRACK_NAMES[track]);
    ImGui::Separator();

    const std::string& name = drumMachine.fileName[track];
    ImGui::Text("%s", name.empty() ? "(no file)" : name.c_str());

    // Open... button to pick a WAV file from disk.
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, INSP_COLOR_BLUE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INSP_COLOR_BLUE_HOVER);
    if (ImGui::Button("Open ..."))
        openFileDialog();
    ImGui::PopStyleColor(2);

    ImGui::Spacing();

    // Reverse and Delay checkboxes.
    bool reverse = drumMachine.reverse[track].load();
    if (ImGui::Checkbox("Reverse", &reverse))
        drumMachine.reverse[track].store(reverse);

    bool delayEnabled = drumMachine.delayEnabled[track].load();
    if (ImGui::Checkbox("Delay FX", &delayEnabled))
        drumMachine.delayEnabled[track].store(delayEnabled);

    // Delay time and mix sliders (both 0.0 - 1.0).
    float delayTime = drumMachine.delayTime[track].load();
    if (ImGui::SliderFloat("Delay time", &delayTime, 0.0f, 1.0f))
        drumMachine.delayTime[track].store(delayTime);

    float delayMix = drumMachine.delayMix[track].load();
    if (ImGui::SliderFloat("Delay mix", &delayMix, 0.0f, 1.0f))
        drumMachine.delayMix[track].store(delayMix);
}
