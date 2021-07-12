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
    QSettings s("CS123, Lab04");
    lightColor = s.value("lightColor", QColor(255, 255, 255)).value<QColor>();
    lightIntensity = s.value("lightIntensity", 5.f).toFloat();
    attQuadratic = s.value("attQuadratic", 1.0f).toFloat();
    attLinear = s.value("attLinear", 0.0f).toFloat();
    attConstant = s.value("attConstant", 0.0f).toFloat();
    sphereLColor = s.value("sphereLColor", QColor(0, 170, 255)).value<QColor>();
    sphereMColor = s.value("sphereMColor", QColor(255, 100, 230)).value<QColor>();
    sphereRColor = s.value("sphereRColor", QColor(170, 255, 0)).value<QColor>();
    ambientIntensity = s.value("ambient", 0.2f).toFloat();
    diffuseIntensity = s.value("diffuse", 0.7f).toFloat();
    specularIntensity = s.value("specular", 0.7f).toFloat();
    shininess = s.value("shininess", 40.f).toFloat();
}

void Settings::saveSettings() {
    QSettings s("CS123, Lab04");
    s.setValue("lightIntensity", lightIntensity);
    s.setValue("lightColor", lightColor);
    s.setValue("attQuadratic", attQuadratic);
    s.setValue("attLinear", attLinear);
    s.setValue("attConstant", attConstant);
    s.setValue("sphereLColor", sphereLColor);
    s.setValue("sphereMColor", sphereMColor);
    s.setValue("sphereRColor", sphereRColor);
    s.setValue("ambient", ambientIntensity);
    s.setValue("diffuse", diffuseIntensity);
    s.setValue("specular", specularIntensity);
    s.setValue("shininess", shininess);
}
