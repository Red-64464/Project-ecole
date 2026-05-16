/**
 * @brief Save and load user preferences.
 *
 * Stores the WAV file path chosen for each track in @c preferences.txt
 * so they can be reloaded automatically on the next launch (the user
 * does not need to pick the same sounds every time).
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
#include "Constants.h"

/**
 * @brief Static helpers to save and load preferences.
 *
 * All methods are static: no object is needed.
 * Usage: @c Preferences::save(...) or @c Preferences::load(...)
 */
class Preferences {
public:
    /**
     * @brief Save the WAV file paths to @c preferences.txt.
     * @param filePaths Array of @c NUM_TRACKS file paths to save.
     */
    static void save(const std::string filePaths[NUM_TRACKS]);

    /**
     * @brief Load the WAV file paths from @c preferences.txt.
     * @param filePaths Array of @c NUM_TRACKS file paths to fill with the values read.
     */
    static void load(std::string filePaths[NUM_TRACKS]);
};

#endif // PREFERENCES_H
