/**
 * @brief Program entry point.
 *
 * Creates the main window and starts the application.
 */

#include "ui/MainWindow.h"

/**
 * @brief Starts the program.
 *
 * Creates MainWindow, initializes the application and runs the main loop.
 *
 * @return 0 on clean exit.
 */
int main() {
    MainWindow mainWindow;
    mainWindow.init();
    mainWindow.run();
    return 0;
}
