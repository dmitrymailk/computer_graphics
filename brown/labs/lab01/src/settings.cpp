#include "settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
  Loads the application settings, or, if no saved settings are available,
  loads default values for the settings. You can change the defaults here.
**/
void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    QSettings s("CS123", "Lab01");
    shape = s.value("shape", SHAPE_TRIANGLE).toInt();
    linesEnabled = s.value("linesEnabled", false).toBool();
}

void Settings::saveSettings() {
    QSettings s("CS123", "Lab01");
    s.setValue("shape", shape);
    s.setValue("linesEnabled", linesEnabled);
}
