/**
 * @brief Point d'entrée du programme.
 *
 * Ce fichier crée la fenêtre principale
 * et lance l'application.
 */

// On inclut le fichier MainWindow.h pour pouvoir utiliser la classe MainWindow
#include "ui/MainWindow.h"

/**
 * @brief Lance le programme.
 *
 * Cette fonction crée MainWindow,
 * initialise l'application et démarre la boucle principale.
 *
 * @return 0 si le programme se termine correctement.
 */
int main() {
    
    MainWindow mainWindow;

    mainWindow.init();

    mainWindow.run();

    return 0;
}
