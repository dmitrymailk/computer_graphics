/**
 * @file    Settings.h
 *
 * This file contains various settings and enumerations that you will need to use in the various
 * assignments. The settings are bound to the GUI via static data bindings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "RGBA.h"

// Enumeration values for the Filters that the user can select in the GUI.
enum FilterType {
    FILTER_INVERT,
    FILTER_GRAYSCALE,
    FILTER_IDENTITY,
    FILTER_SHIFT_LEFT,
    FILTER_SHIFT_RIGHT,
    NUM_FILTER_TYPES
};

/**
 * @struct Settings
 *
 * Stores application settings for the CS123 GUI.
 *
 * You can access all app settings through the "settings" global variable.
 * The settings will be automatically updated when things are changed in the
 * GUI (the reverse is not true however: changing the value of a setting does
 * not update the GUI).
*/
struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    // Saves the current settings to disk.
    void saveSettings();

    // Filter
    int filterType;               // The selected filter @see FilterType
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
