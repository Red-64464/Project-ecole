/**
 * @file MainWindowDraw.cpp
 * @brief Overall layout and playback controls of the main window.
 *
 * Contains two methods:
 *   - MainWindow::draw() : full layout of the application.
 *   - MainWindow::drawPlaybackControls() : Play/Stop and BPM widgets.
 */

#include "MainWindow.h"
#include <string>
#include <algorithm>
#include "imgui.h"

static const ImVec4 COLOR_BLUE(0.2f, 0.4f, 0.8f, 1.0f);
static const ImVec4 COLOR_BLUE_HOVER(0.3f, 0.5f, 0.9f, 1.0f);

void MainWindow::draw() {
    // Pick up the WAV file the user may have chosen on the previous frame.
    handleFileDialogResult();

    // Fill the whole window with a single ImGui panel.
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("##main", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    drawPlaybackControls();

    ImGui::Spacing();

    // Right inspector has a fixed width; the sequencer takes the rest.
    constexpr float inspectorWidth = 250.0f;
    float leftWidth = ImGui::GetContentRegionAvail().x - inspectorWidth - 10.0f;

    ImGui::BeginChild("LeftPanel", ImVec2(leftWidth, 0), false);
    drawSequencerGrid();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(inspectorWidth, 0), true);
    drawInspector();
    ImGui::EndChild();

    ImGui::End();
}

void MainWindow::drawPlaybackControls() {
    bool isPlaying = drumMachine.playing.load();

    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_BLUE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_HOVER);

    if (ImGui::Button(isPlaying ? "Stop" : "Play", ImVec2(60, 30)))
        audioEngine.togglePlay();

    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    int bpm = drumMachine.bpm.load();

    // BPM "-" button: clamped to 30 BPM minimum.
    if (ImGui::Button(" - ##bpm", ImVec2(30, 30)))
        drumMachine.bpm.store(std::max(30, bpm - 1));

    ImGui::SameLine();
    ImGui::Text("%d bpm", bpm);
    ImGui::SameLine();

    // BPM "+" button: clamped to 300 BPM maximum.
    if (ImGui::Button(" + ##bpm", ImVec2(30, 30)))
        drumMachine.bpm.store(std::min(300, bpm + 1));
}
