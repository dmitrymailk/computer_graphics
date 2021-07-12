#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>

namespace Ui {
class MainWindow;
}

class UniformWidget; class QBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadShader(QString vert, QString frag);

signals:
    void removeUniforms();

private slots:
    void init();

    void on_sphereButton_clicked();

    void on_cubeButton_clicked();

    void on_defaultShaderButton_clicked();

    void on_animMoveButton_clicked();

    void on_animNoneButton_clicked();

    void on_animScaleButton_clicked();

    void on_animScaleMoveButton_clicked();

    void on_animRotate1Button_clicked();

    void on_animRotate2Button_clicked();

    void on_wireframeCheckbox_toggled(bool checked);

    void on_tipsButton_clicked();

    void on_shader1Button_clicked();

    void on_shader2Button_clicked();

    void on_checkBox_toggled(bool checked);

public slots:
    void handleUniformDeleted(UniformWidget *deleted);
    void changeUniform(const UniformVariable *uniform, const QString &newVal);
    void changeUniform(const QString &name, const QString &newVal);
    void addUniform(UniformVariable::Type type, const QString &name, bool editable = false, int size = 1);
    void addUniform(UniformVariable *uniform, bool editable = false);

protected:
    void closeEvent(QCloseEvent *e);
private:
    Ui::MainWindow *ui;
    GLWidget *m_glwidget;
    QString m_oldVert, m_oldFrag;
    bool m_hasError;

    QList<UniformWidget *> m_uniforms;
    QString m_initShader;

    QWidget *m_uniformContainer;
    QBoxLayout *m_uniformLayout;
};

#endif // MAINWINDOW_H
