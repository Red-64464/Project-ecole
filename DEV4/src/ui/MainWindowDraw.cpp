/**
 * @brief Mise en page générale et contrôles de lecture de la fenêtre principale.
 *
 * Ce fichier contient 2 méthodes :
 * - MainWindow::draw() : organise la mise en page générale.
 * - MainWindow::drawPlaybackControls() : affiche les boutons Play/Stop et le BPM.
 *
 */

#include "MainWindow.h"
#include <string>
#include <algorithm>
#include "imgui.h"


static const ImVec4 COLOR_BLUE(0.2f, 0.4f, 0.8f, 1.0f); 
static const ImVec4 COLOR_BLUE_HOVER(0.3f, 0.5f, 0.9f, 1.0f);

/**
 * @brief Dessine la fenêtre principale.
 *
 * Cette méthode affiche toute l'interface :
 * les boutons, la grille et le panneau de droite.
 */
void MainWindow::draw(){
    // On vérifie si l'utilisateur a choisi un fichier 
    handleFileDialogResult();

    // On récupère le "viewport" = la zone de travail de la fenêtre
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);    // Position = coin haut-gauche
    ImGui::SetNextWindowSize(viewport->WorkSize);  // Taille = toute la fenêtre

    
    // ImGui::Begin() crée une "fenêtre ImGui" (un panneau).
    // Les flags désactivent le titre, empêche de redimensionner , de déplacer 
    ImGui::Begin("##main", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // On dessine les contrôles de lecture (Play/Stop, BPM) en haut
    drawPlaybackControls();

    ImGui::Spacing();


    // La largeur du panneau de droite est fixe (250 pixels)
    float inspectorWidth = 250.0f;
    // La largeur du panneau de gauche = tout le reste - 10 pixels de marge
    float leftWidth = ImGui::GetContentRegionAvail().x - inspectorWidth - 10.0f;

    // ImGui::BeginChild() crée une sous-zone (un panneau dans le panneau).
    // "LeftPanel" est un identifiant unique pour cette zone.
    ImGui::BeginChild("LeftPanel", ImVec2(leftWidth, 0), false);
    drawSequencerGrid();  
    ImGui::EndChild();  


    ImGui::SameLine();

    // Le panneau de droite (avec une bordure = true en dernier paramètre)
    ImGui::BeginChild("RightPanel", ImVec2(inspectorWidth, 0), true);
    drawInspector();      
    ImGui::EndChild();    

    // ImGui::End() ferme la fenêtre ImGui ouverte par Begin()
    ImGui::End();

}


/**
 * @brief Dessine les boutons de lecture.
 *
 * Cette méthode affiche le bouton Play/Stop
 * et les boutons pour changer le BPM.
 */
void MainWindow::drawPlaybackControls() {

    bool isPlaying = drumMachine.playing.load();

    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_BLUE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_HOVER);

   
    if (ImGui::Button(isPlaying ? "Stop" : "Play", ImVec2(60, 30)))
        audioEngine.togglePlay();  // On bascule entre Play et Stop

    // PopStyleColor(2) enlève les 2 couleurs qu'on avait empilées
    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    // On lit le BPM actuel
    int bpm = drumMachine.bpm.load();


    // ImGui a besoin d'un identifiant unique pour chaque bouton.
    if (ImGui::Button(" - ##bpm", ImVec2(30, 30)))
        // std::max(30, bpm - 1) empêche le BPM de descendre en dessous de 30
        drumMachine.bpm.store(std::max(30, bpm - 1));

    ImGui::SameLine();
    // ImGui::Text() affiche du texte. %d sera remplacé par la valeur de bpm.
    ImGui::Text("%d bpm", bpm);
    ImGui::SameLine();

    // Bouton "+" pour augmenter le BPM
    if (ImGui::Button(" + ##bpm", ImVec2(30, 30)))
        // std::min(300, bpm + 1) empêche le BPM de dépasser 300
        drumMachine.bpm.store(std::min(300, bpm + 1));
}
