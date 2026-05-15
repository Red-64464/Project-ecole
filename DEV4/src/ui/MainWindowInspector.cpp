/**
 * @file MainWindowInspector.cpp
 * @brief Dessine le panneau de droite.
 *
 * Ce fichier contient la méthode drawInspector().
 */

#include "MainWindow.h"
#include <string>      
#include "imgui.h"     


static const ImVec4 INSP_COLOR_BLUE(0.2f, 0.4f, 0.8f, 1.0f);
static const ImVec4 INSP_COLOR_BLUE_HOVER(0.3f, 0.5f, 0.9f, 1.0f);


static const char* INSP_TRACK_NAMES[] = {"Track 1", "Track 2", "Track 3", "Track 4"};

/**
 * @brief Dessine le panneau d'inspection.
 *
 * Ce panneau affiche les informations de la piste sélectionnée :
 * le nom de la piste, le fichier audio, Reverse, Delay FX
 * et les réglages du delay.
 */
void MainWindow::drawInspector() {

    int sel = drumMachine.selectedTrack;

   
    ImGui::Text("%s", INSP_TRACK_NAMES[sel]);
    // Un trait horizontal pour séparer visuellement
    ImGui::Separator();

    
    const std::string& name = drumMachine.fileName[sel];

    // On affiche le nom du fichier, ou "(no file)" s'il n'y en a pas
    ImGui::Text("%s", name.empty() ? "(no file)" : name.c_str());

  
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, INSP_COLOR_BLUE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INSP_COLOR_BLUE_HOVER);
    if (ImGui::Button("Open ...")) // On crée le bouton avec un écouteur d'évènements , si il est true c'est qu'on a cliquer dessus 
        openFileDialog();
    ImGui::PopStyleColor(2);

    ImGui::Spacing(); 


    bool rev = drumMachine.reverse[sel].load();
    if (ImGui::Checkbox("Reverse", &rev))
        drumMachine.reverse[sel].store(rev);

    bool delayOn = drumMachine.delayEnabled[sel].load();
    if (ImGui::Checkbox("Delay FX", &delayOn))
        drumMachine.delayEnabled[sel].store(delayOn);


    float dTime = drumMachine.delayTime[sel].load();
    if (ImGui::SliderFloat("Delay time", &dTime, 0.0f, 1.0f))
        drumMachine.delayTime[sel].store(dTime);

    float dMix = drumMachine.delayMix[sel].load();
    if (ImGui::SliderFloat("Delay mix", &dMix, 0.0f, 1.0f))
        drumMachine.delayMix[sel].store(dMix);
}
