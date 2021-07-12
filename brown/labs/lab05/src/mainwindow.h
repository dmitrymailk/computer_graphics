#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "glwidget.h"

namespace Ui {
    class MainWindow;
}

class DataBinding;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // Overridden from QWidget
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *m_ui;
    GLWidget *m_glWidget;

};

#endif // MAINWINDOW_H
