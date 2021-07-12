#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <assert.h>
#include "databinding.h"
#include "settings.h"
#include <QColorDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    QGLFormat qglFormat;
    qglFormat.setVersion(4,0);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    m_ui->setupUi(this);
    QGridLayout *gridLayout = new QGridLayout(m_ui->centralWidget);
    m_glWidget = new GLWidget(qglFormat, this);
    gridLayout->addWidget(m_glWidget, 0, 1);

    settings.loadSettingsOrDefaults();
    dataBind();

    // Restore the UI settings
    QSettings qtSettings("CS123", "Lab04");
    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings) {
        delete b;
    }
    delete m_ui;
    delete m_glWidget;
}

void MainWindow::dataBind() {
#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->lightIntensitySlider, m_ui->lightIntensityEdit,
                                            settings.lightIntensity,
                                            0.f,    // minValue
                                            20.f)); // maxValue

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->attQuadraticSlider, m_ui->attQuadraticEdit,
                                            settings.attQuadratic, 0.f, 10.f));

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->attLinearSlider, m_ui->attLinearEdit,
                                            settings.attLinear, 0.f, 10.f));

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->attConstantSlider, m_ui->attConstantEdit,
                                            settings.attConstant, 0.f, 10.f));

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->ambientSlider, m_ui->ambientEdit,
                                            settings.ambientIntensity, 0.f, 1.f));

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->diffuseSlider, m_ui->diffuseEdit,
                                            settings.diffuseIntensity, 0.f, 1.f));

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->specularSlider, m_ui->specularEdit,
                                            settings.specularIntensity, 0.f, 1.f));

    BIND(FloatBinding::bindSliderAndTextbox(m_ui->shininessSlider, m_ui->shininessEdit,
                                            settings.shininess, 5.f, 100.f));

    BIND(ColorBinding::bindButtonAndTextboxes(m_ui->lightColorButton,
                                              m_ui->lightREdit, m_ui->lightGEdit, m_ui->lightBEdit,
                                              settings.lightColor));

    BIND(ColorBinding::bindButtonAndTextboxes(m_ui->sphereLColorButton,
                                              m_ui->sphereLREdit, m_ui->sphereLGEdit, m_ui->sphereLBEdit,
                                              settings.sphereLColor));

    BIND(ColorBinding::bindButtonAndTextboxes(m_ui->sphereMColorButton,
                                              m_ui->sphereMREdit, m_ui->sphereMGEdit, m_ui->sphereMBEdit,
                                              settings.sphereMColor));

    BIND(ColorBinding::bindButtonAndTextboxes(m_ui->sphereRColorButton,
                                              m_ui->sphereRREdit, m_ui->sphereRGEdit, m_ui->sphereRBEdit,
                                              settings.sphereRColor));

#undef BIND
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("CS123", "Lab04");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::settingsChanged() {
    m_glWidget->update();
}
