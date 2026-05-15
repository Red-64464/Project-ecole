/**
 * @brief Dessine la grille du séquenceur.
 *
 * Ce fichier contient la méthode drawSequencerGrid().
 */

#include "MainWindow.h"
#include <string>      
#include "imgui.h"     

static const ImVec4 COLOR_TURQUOISE(0.0f, 0.8f, 0.8f, 1.0f); // Couleur du step si activé   
static const ImVec4 COLOR_YELLOW(1.0f, 0.9f, 0.0f, 1.0f); // Couleur du curseur qui avance 
static const ImVec4 COLOR_ORANGE(1.0f, 0.5f, 0.0f, 1.0f); // Couleur de la piste sélectioné 
static const ImVec4 COLOR_LIGHT_GRAY(0.85f, 0.85f, 0.85f, 1.0f); // Couleur de la step au repos


static const char* SEQ_TRACK_NAMES[] = {"Track 1", "Track 2", "Track 3", "Track 4"};

/**
 * @brief Dessine la grille du séquenceur.
 *
 * Cette méthode affiche les 4 pistes,
 * les 16 pas, le volume et le bouton Mute.
 */
void MainWindow::drawSequencerGrid() {
    // On lit le pas actuel du séquenceur et l'état de lecture
    int currentStep = drumMachine.currentStep.load();
    bool isPlaying = drumMachine.playing.load();

   
    for (int track = 0; track < NUM_TRACKS; track++) {
        
        bool isSelected = (drumMachine.selectedTrack == track);

       
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_ORANGE);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_ORANGE);
        }

        // Le "##sel" est un id caché qui rend le bouton unique pour ImGui.
        std::string selLabel = std::string(SEQ_TRACK_NAMES[track]) + "##sel";

       
        if (ImGui::Button(selLabel.c_str(), ImVec2(70, 25)))
            drumMachine.selectedTrack = track;

        // On enlève les couleurs orange si la piste était sélectionnée
        if (isSelected) ImGui::PopStyleColor(2);

        
        for (int step = 0; step < NUM_STEPS; step++) {
          
            ImGui::SameLine(0, 2);

            
            bool isActive = drumMachine.grid[track][step].load();

            bool isCurrent = (step == currentStep && isPlaying);

            // On choisit la couleur des steps
            ImVec4 color = COLOR_LIGHT_GRAY;
            if (isCurrent)     color = COLOR_YELLOW;
            else if (isActive) color = COLOR_TURQUOISE;

      
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            // Au survol, on assombrit légèrement la couleur (* 0.85)
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                ImVec4(color.x * 0.85f, color.y * 0.85f, color.z * 0.85f, 1.0f));

            // On crée un identifiant unique caché : "##s0_0", "##s0_1", etc.
            std::string stepLabel = "##s" + std::to_string(track) + "_" + std::to_string(step);

           
            if (ImGui::Button(stepLabel.c_str(), ImVec2(25, 25)))
                drumMachine.grid[track][step].store(!isActive);

            // On enlève les 2 couleurs empilées
            ImGui::PopStyleColor(2);
        }

      
        ImGui::SameLine();
        // PushItemWidth fixe la largeur du prochain widget (le slider) à 80 pixels
        ImGui::PushItemWidth(80);

        float vol = drumMachine.volume[track].load();
        std::string volLabel = "##vol" + std::to_string(track);

        if (ImGui::SliderFloat(volLabel.c_str(), &vol, 0.0f, 1.0f, "%.2f"))
            drumMachine.volume[track].store(vol);

        ImGui::PopItemWidth();

   
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
