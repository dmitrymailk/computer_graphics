#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <assert.h>
#include "databinding.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    QGLFormat qglFormat;
    qglFormat.setVersion(4,0);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    m_ui->setupUi(this);

    // canvas for lab 3
    QGridLayout *gridLayout = new QGridLayout(m_ui->lab3Canvas);
    m_lab3Canvas = new GLWidget(qglFormat, this, false);
    m_lab3Canvas->setMinimumSize(50, 50);
    gridLayout->addWidget(m_lab3Canvas, 0, 1);

    // canvas for lab 2
    QGridLayout *gridLayoutLab2 = new QGridLayout(m_ui->lab2Canvas);
    m_lab2Canvas = new GLWidget(qglFormat, this, true);
    m_lab2Canvas->setMinimumSize(50, 50);
    gridLayoutLab2->addWidget(m_lab2Canvas, 0, 1);

    // mfowler
    m_ui->tabWidget->setCurrentWidget(m_ui->lab3);
    m_ui->tabWidget->setCurrentWidget(m_ui->lab2);

    QGroupBox* genderGroupBox = new QGroupBox("shaderProgram");
    QVBoxLayout* genderGroupBoxLayout = new QVBoxLayout;
//    QRadioButton* maleRadioButton = new QRadioButton("Male");
//    QRadioButton* femaleRadioButton = new QRadioButton("Female");

//    genderGroupBoxLayout->addWidget(maleRadioButton);
//    genderGroupBoxLayout->addWidget(femaleRadioButton);
//    genderGroupBoxLayout->addStretch();
//    genderGroupBox->setLayout(genderGroupBoxLayout);

//    m_ui->centralWidget->layout()->addWidget(genderGroupBox);

    settings.loadSettingsOrDefaults();
    dataBind();

    // Restore the UI settings
    QSettings qtSettings("CS123", "Lab03");
    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings)
    {
        delete b;
    }
    delete m_ui;
    delete m_lab2Canvas;
    delete m_lab3Canvas;
}

void MainWindow::dataBind() {
#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

    BIND(ChoiceBinding::bindRadioButtons(NUM_SHADER_PROGRAMS, settings.shaderProgram,
                                    m_ui->solidShader,
                                    m_ui->gradientShader,
                                    m_ui->textureShader));

#undef BIND
}

void MainWindow::settingsChanged() {
    m_lab2Canvas->repaint();
    m_lab3Canvas->update();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("CS123", "Lab03");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}
