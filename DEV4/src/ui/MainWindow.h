/**
 * @brief Declaration de la fenetre principale.
 *
 * Cette classe gere la fenetre SDL, l'interface ImGui
 * et le lien avec le moteur audio.
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SDL3/SDL.h>            // Bibliotheque SDL pour la fenetre et le rendu.
#include "core/DrumMachine.h"    // Donnees de la drum machine.
#include "audio/AudioEngine.h"   // Moteur audio.

/**
 * @brief Fenetre principale de l'application.
 */
class MainWindow {
public:
    /**
     * @brief Initialise la fenetre, ImGui, l'audio et les preferences.
     */
    void init();

    /**
     * @brief Lance la boucle principale de l'application.
     */
    void run();

private:
    /**
     * @brief Dessine toute l'interface.
     */
    void draw();

    /**
     * @brief Dessine les controles de lecture (play, pause, stop).
     */
    void drawPlaybackControls();

    /**
     * @brief Dessine la grille du sequenceur. ( 4 pistes x 16 pas )
     */
    void drawSequencerGrid();

    /**
     * @brief Dessine le panneau d'inspection. ( volume, delay, etc. )
     */
    void drawInspector();

    /**
     * @brief Gere le resultat de la selection d'un fichier WAV.
     */
    void handleFileDialogResult();

    /**
     * @brief Charge les preferences sauvegardees.
     */
    void loadPreferences();

    /**
     * @brief Ouvre une boite de dialogue pour choisir un fichier WAV.
     */
    void openFileDialog();

    /**
     * @brief Fenetre SDL principale.
     */
    SDL_Window* window{nullptr};

    /**
     * @brief Renderer SDL utilise pour l'affichage.
     */
    SDL_Renderer* renderer{nullptr};

    /**
     * @brief Donnees principales de la drum machine.
     */
    DrumMachine drumMachine;

    /**
     * @brief Moteur audio qui produit le son.
     */
    AudioEngine audioEngine;
};

#endif // MAINWINDOW_H
