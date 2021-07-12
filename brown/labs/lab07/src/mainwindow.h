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

public slots:
    // Used internally to keep data bindings and settings in sync.
    void settingsChanged();

protected:
    // Overridden from QWidget
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *m_ui;
    QList<DataBinding *> m_bindings;
    GLWidget *m_glWidget;

    void dataBind();
};

#endif // MAINWINDOW_H
