#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Databinding.h"
#include "Settings.h"
#include <math.h>
#include <assert.h>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Make sure the settings are loaded before the UI
    settings.loadSettingsOrDefaults();

    ui->setupUi(this);

    // Resize the window because the window is huge since all docks were visible.
    resize(1000, 600);

    // Bind the UI elements to their properties in the global Settings object, using binding
    // objects to simplify the code.  Each binding object connects to its UI elements and keeps
    // the UI and its setting in sync.

    QList<QAction*> actions;

#define SETUP_ACTION(dock, key) \
    actions.push_back(ui->dock->toggleViewAction()); \
    actions.back()->setShortcut(QKeySequence(key));

    SETUP_ACTION(filterDock,    "CTRL+2");

    ui->menuToolbars->addActions(actions);
#undef SETUP_ACTION
    dataBind();


    // Reset the contents of both canvas widgets (make a new 500x500 image for the 2D one)
    fileNew();
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings)
        delete b;
    foreach (QButtonGroup *bg, m_buttonGroups)
        delete bg;
    delete ui;
}

void MainWindow::dataBind() {
#define BIND(b) { \
    DataBinding *_b = (b); \
    m_bindings.push_back(_b); \
    assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); \
}
    QButtonGroup *filterButtonGroup = new QButtonGroup;
    m_buttonGroups.push_back(filterButtonGroup);

    // Filter dock
    BIND(ChoiceBinding::bindRadioButtons(
            filterButtonGroup,
            NUM_FILTER_TYPES,
            settings.filterType,
            ui->filterTypeInvert,
            ui->filterTypeGrayscale,
            ui->filterTypeIdentity,
            ui->filterTypeShiftLeft,
            ui->filterTypeShiftRight))
#undef BIND
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e); // allow the superclass to handle this for the most part...

    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
}

void MainWindow::settingsChanged() {
    ui->canvas2D->settingsChanged();
}

void MainWindow::fileNew() {
    ui->canvas2D->newImage();
}

void MainWindow::fileOpen() {
    QString file = QFileDialog::getOpenFileName(this, QString(), "/course/cs123/data/");
    if (!file.isNull()) {
        if (file.endsWith(".xml")) {

        }
        else if(file.endsWith(".obj")) {

        }
        else {
            if (!ui->canvas2D->loadImage(file)) {
                QMessageBox::critical(this, "Error", "Could not load image \"" + file + "\"");
            } else {
                //activateCanvas2D();
            }
        }
    }
}

void MainWindow::fileSave() {
    ui->canvas2D->saveImage();
}

void MainWindow::filterImage() {
    // Disable the UI so the user can't interfere with the filtering
    setAllEnabled(false);

    // Actually do the filter.
    ui->canvas2D->filterImage();

    // Enable the UI again
    setAllEnabled(true);
}

void MainWindow::setAllEnabled(bool enabled) {
    QList<QWidget *> widgets;
    widgets += ui->filterDock;

    QList<QAction *> actions;
    actions += ui->actionNew;
    actions += ui->actionOpen;
    actions += ui->actionSave;
    actions += ui->actionRevert;
    actions += ui->actionQuit;

    foreach (QWidget *widget, widgets)
        widget->setEnabled(enabled);
    foreach (QAction *action, actions)
        action->setEnabled(enabled);
}

void MainWindow::revertImage()
{
    ui->canvas2D->revertImage();
}
