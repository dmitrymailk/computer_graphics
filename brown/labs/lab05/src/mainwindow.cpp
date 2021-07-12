#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <assert.h>
#include <QGridLayout>
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
    m_glWidget->setMinimumSize(100, 100);
    gridLayout->addWidget(m_glWidget, 0, 1);

    // Restore the UI settings
    QSettings qtSettings("CS123", "Lab05");
    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_glWidget;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings qtSettings("CS123", "Lab05");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}
