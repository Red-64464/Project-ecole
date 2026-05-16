/**
 * @file Preferences.cpp
 * @brief Save and load user preferences.
 *
 * Format (as specified in the project brief): one WAV file path per line,
 * in track order (line 0 = track 0, line 1 = track 1, ...). An empty line
 * means the corresponding track has no file loaded.
 */

#include "Preferences.h"
#include <fstream>

/// Name of the text file used to store preferences.
static const std::string PREFS_FILE = "preferences.txt";

/**
 * @brief Save the WAV file paths to @c preferences.txt.
 *
 * One path is written per line, in track order. Empty tracks produce
 * an empty line so the line index always matches the track index.
 *
 * @param filePaths Array of @c NUM_TRACKS file paths to save.
 */
void Preferences::save(const std::string filePaths[NUM_TRACKS]) {
    std::ofstream file(PREFS_FILE);
    if (!file.is_open()) return;

    for (int track = 0; track < NUM_TRACKS; track++) {
        file << filePaths[track] << "\n";
    }
}

/**
 * @brief Load the WAV file paths from @c preferences.txt.
 *
 * Reads one line per track, in order. Missing lines (file shorter than
 * NUM_TRACKS) leave the corresponding entries untouched. If the file
 * does not exist yet (first launch), the function simply returns.
 *
 * @param filePaths Array of @c NUM_TRACKS file paths to fill.
 */
void Preferences::load(std::string filePaths[NUM_TRACKS]) {
    std::ifstream file(PREFS_FILE);
    if (!file.is_open()) return;

    std::string line;
    for (int track = 0; track < NUM_TRACKS && std::getline(file, line); track++) {
        filePaths[track] = line;
    }
}
