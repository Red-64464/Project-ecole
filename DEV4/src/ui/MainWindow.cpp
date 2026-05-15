/**
 * @file MainWindow.cpp
 * @brief Initialisation, boucle principale et gestion des fichiers.
 *
 * Ce fichier contient :
 * - l'initialisation de SDL, ImGui et du moteur audio
 * - la boucle principale de l'application
 * - le chargement des preferences sauvegardees
 * - la gestion de la boite de dialogue pour ouvrir un fichier WAV
 *
 * Les fonctions de dessin sont dans MainWindowDraw.cpp.
 */

// SDL3  : bibliothèque pour créer des fenêtres, gérer les événements (clics, clavier),
// ImGui : bibliothèque pour créer facilement des interfaces graphiques (boutons, sliders, etc.)

// SDL3 construit la fenêtre et gère les interactions 
// ImGui : dessine les éléments d'interface dedans et reçoit les événements de SDL3 (clics, clavier, etc.) pour faire réagir les boutons et autres éléments d'interface

#include "MainWindow.h"
#include "core/Preferences.h"      
#include <thread>                   
#include <chrono>                  
#include <string>                   
#include "imgui.h"                  // Bibliothèque ImGui pour l'interface graphique
#include "imgui_impl_sdl3.h"        // ImGui : permets a ImgGui de recevoir des évènements SDL3 : par exmeple des clic souris ou clavier ou fermeture de la fenêtre ou autre 
#include "imgui_impl_sdlrenderer3.h" // ImGui : cea permets a ImGui de s'afficher a l'écran avec le moteur de rendu de dessin SDL3


const float FRAMERATE = 60.0f;

// Le temps qu'une image devrait prendre : 1000 ms / 60 ≈ 16.67 millisecondes
// " Si je veux afficher 60 images par seconde, chaque image doit être affichée pendant environ 16.67 ms (1000 ms / 60). Si une image est affichée plus rapidement que ça, on attend le temps restant pour ne pas dépasser 60 FPS."
// target_framerate = combien d'images dois durer une seconde 
// on fait ca pour que le programme garde une vitesse stable sans sa parfois li peut aller trop vite ou trop lentement 
const std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

/**
 * @brief Extrait le nom du fichier depuis un chemin complet.
 *
 * Exemple :
 * "C:/sons/kick.wav" devient "kick.wav".
 *
 * @param path Chemin complet du fichier.
 * @return Nom du fichier seul.
 */
static std::string getFileName(const std::string& path) {
   
    size_t pos = path.find_last_of("/\\");
    // Si on en a trouvé un, on renvoie tout ce qui est après
    if (pos != std::string::npos)
        return path.substr(pos + 1);
    // Si on arrive ici c'est que le chemin  ne contient pas de / ou de \, donc on suppose que c'est déjà un nom de fichier
    // Sinon le chemin ne contient pas de dossier, on renvoie le chemin entier
    return path;
}


/**
 * @brief Recupere le resultat de la boite de dialogue.
 *
 * SDL appelle cette fonction automatiquement quand l'utilisateur
 * a termine de choisir un fichier.
 *
 * @param userdata Pointeur vers notre DrumMachine.
 * @param filelist Liste des fichiers choisis.
 * @param filter Filtre selectionne.
 */
static void onFileDialogResult(void* userdata, const char* const* filelist, int /*filter*/) {
   
    auto* dm = static_cast<DrumMachine*>(userdata);
   
    if (filelist && filelist[0]) { 
    
        dm->pendingFilePath = filelist[0]; // pendingFilePath est une variable qui stocke le chemin du fichier sélectioné que on a pas encore traité
 
        dm->fileDialogComplete = true;
    }
}

/**
 * @brief Initialise tout ce qu'il faut pour lancer l'application.
 *
 * Cette fonction prepare SDL, la fenetre, le renderer,
 * ImGui, le moteur audio et les preferences sauvegardees.
 */
void MainWindow::init() {
    // SDL_Init() vient de la bibliothèque SDL.
    // Elle initialise le système vidéo ( qui s'occupe d'afficher la fenêtre et le graphique ) et audio.
    // Si ça échoue, on affiche une erreur et on arrête.
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Error: SDL_Init(): %s", SDL_GetError());
        return;
    }

    // crée une fenêtre avec :
    //   - "Drum Machine" comme titre
    //   - 1100 pixels de large et 400 de haut
    //   - SDL_WINDOW_HIDDEN : la fenêtre est cachée au début pour éviter de la voir se construire, on l'affichera plus tard avec SDL_ShowWindow() après l'avoir centrée et préparée
    //   - SDL_WINDOW_RESIZABLE : l'utilisateur peut redimensionner la fenêtre
    window = SDL_CreateWindow("Drum Machine", 1100, 400,
                              SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    if (!window) { 
        SDL_Log("Error: SDL_CreateWindow(): %s", SDL_GetError());
        return;
    }

    // SDL_CreateRenderer() crée un "renderer" = l'outil qui dessine dans la fenêtre.
    // nullptr = on laisse SDL choisir le meilleur renderer disponible.
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s", SDL_GetError());
        return;
    }
  
    // On centre la fenêtre sur l'écran ( largeur, hauteur )
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    // Maintenant on rend la fenêtre visible
    SDL_ShowWindow(window);



    IMGUI_CHECKVERSION();
    // Crée le "contexte" ImGui (l'espace de travail interne d'ImGui)
    ImGui::CreateContext();
    // Applique le thème sombre (couleurs sombres pour l'interface)
    ImGui::StyleColorsDark();

    ImGui::GetStyle().WindowRounding = 0.0f;

    // Connecte ImGui à SDL3 pour qu'il reçoive les événements (clics, clavier...)
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    // Connecte ImGui au renderer SDL3 pour qu'il puisse dessiner
    ImGui_ImplSDLRenderer3_Init(renderer);

    // On démarre le moteur audio en lui donnant accès aux données de la drum machine
    audioEngine.init(&drumMachine);

    // On charge les fichiers WAV qui étaient sauvegardés de la session précédente
    loadPreferences();
}

/**
 * @brief Charge les fichiers WAV sauvegardes.
 *
 * Cette fonction est appelee au demarrage pour retrouver
 * les fichiers de la derniere session.
 */

void MainWindow::loadPreferences() {
    // On lit les chemins de fichiers depuis le fichier preferences.txt des 4 pistes 
    Preferences::load(drumMachine.filePath);

    for (int i = 0; i < NUM_TRACKS; i++) {
        // Si le chemin n'est pas vide (= un fichier avait été sauvegardé)
        if (!drumMachine.filePath[i].empty()) {
            // On essaie de charger le fichier WAV dans le player de cette piste
            if (audioEngine.loadFile(i, drumMachine.filePath[i]))
                // Si ça marche, on extrait le nom du fichier pour l'afficher
                drumMachine.fileName[i] = getFileName(drumMachine.filePath[i]);
            else
                // Si le fichier n'existe plus ou est invalide, on efface le chemin du fichier pour la piste actuelle 
                drumMachine.filePath[i].clear();
        }
    }
}

/**
 * @brief Ouvre la boite de dialogue pour choisir un fichier.
 *
 * L'utilisateur peut choisir un fichier WAV sur son ordinateur.
 */
void MainWindow::openFileDialog() {

    static const SDL_DialogFileFilter wavFilter[] = { { "WAV files", "wav" } };

    // Elle ouvre la boîte de dialogue native du système d'exploitation.
    // Quand l'utilisateur choisit un fichier, la fonction onFileDialogResult
    // window = la boîte de dialogue sera attachée à notre fenêtre principale.
    SDL_ShowOpenFileDialog(onFileDialogResult, &drumMachine, window, wavFilter, 1, nullptr, false);
}

/**
 * @brief Traite le fichier choisi par l'utilisateur.
 *
 * Cette fonction est appelee a chaque frame dans draw().
 * Si un fichier a ete choisi, elle le charge dans la piste selectionnee
 * et sauvegarde les preferences.
 */

void MainWindow::handleFileDialogResult() {
    
    if (!drumMachine.fileDialogComplete) return;
    // On remet le flag à false car le fichier vient d'être traité 
    drumMachine.fileDialogComplete = false;

    // Le fichier sera chargé dans la piste actuellement sélectionnée
    int track = drumMachine.selectedTrack;
    // On essaie de charger le fichier WAV choisi
    if (audioEngine.loadFile(track, drumMachine.pendingFilePath)) {
        // Si ça marche, on sauvegarde le chemin et le nom du fichier
        drumMachine.filePath[track] = drumMachine.pendingFilePath;
        drumMachine.fileName[track] = getFileName(drumMachine.filePath[track]);
        // On sauvegarde les préférences pour la prochaine fois
        Preferences::save(drumMachine.filePath);
    }
}

/**
 * @brief Lance la boucle principale de l'application.
 *
 * Cette fonction tourne tant que la fenetre est ouverte.
 * A chaque frame, elle gere les evenements, dessine l'interface
 * et affiche le resultat a l'ecran.
 *
 * La boucle s'arrete quand l'utilisateur ferme la fenetre.
 */

void MainWindow::run() {
    // La couleur de fond de la fenêtre (gris-bleu)
    const auto clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // done = false tant que l'application tourne, true quand on ferme
    bool done = false;
    while (!done) {
        auto frameStart = std::chrono::high_resolution_clock::now();

   
        // SDL_PollEvent() vérifie s'il y a des événements en attente (clic de souris, touche clavier, fermeture de fenêtre, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // tant qu'il y a des événements à traiter en gros on les traite un par un
            // On passe l'événement à ImGui pour qu'il réagisse aux clics/clavier
            ImGui_ImplSDL3_ProcessEvent(&event);
            // Si l'utilisateur a cliqué la croix pour fermer la fenêtre
            // SDL envoie un évènement pour dire que le programme dois s'arrêter 
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            // Même chose mais via l'événement spécifique "fermeture de fenêtre"
            // Quelqu'un a demandé de fermer note fenêtre (par exemple en cliquant sur la croix)
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }


        ImGui_ImplSDLRenderer3_NewFrame();  // Prépare le renderer pour ImGui afin de pouvoir dessiner l'interface
        ImGui_ImplSDL3_NewFrame();          // Met à jour les événements SDL pour ImGui 
        ImGui::NewFrame();                  // Commence une nouvelle frame ImGui ( une frame c'est une image de l'interface )

        // On dessine toute l'interface (boutons, grille, sliders, etc.)
        draw();

        // ImGui::Render() finalise les données de dessin d'ImGui
        ImGui::Render();

        // On remplit l'écran avec la couleur de fond
        SDL_SetRenderDrawColorFloat(renderer,
            clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        SDL_RenderClear(renderer); // Efface l'écran avec la couleur de fond car on va redessiner par dessus
        // On dessine l'interface ImGui par-dessus
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        // On affiche le résultat final à l'écran
        SDL_RenderPresent(renderer);

      

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = frameEnd - frameStart;
 
        if (elapsed < TARGET_FRAMETIME)
            std::this_thread::sleep_for(TARGET_FRAMETIME - elapsed);
    }

    // Quand on sort de la boucle (l'utilisateur a fermé la fenêtre),
    // on libère toutes les ressources dans l'ordre inverse de leur création.
    audioEngine.shutdown();              // Arrête le moteur audio
    ImGui_ImplSDLRenderer3_Shutdown();  
    ImGui_ImplSDL3_Shutdown();           
    ImGui::DestroyContext();            
    SDL_DestroyRenderer(renderer);       
    SDL_DestroyWindow(window);          
    SDL_Quit();                          
}
