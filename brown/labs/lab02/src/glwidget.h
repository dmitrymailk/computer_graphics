#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "GL/glew.h"
#ifdef __APPLE__
#include <glu.h>
#else
#include "GL/glu.h"
#endif

#include <memory>
#include <QGLWidget>
#include <QTimer>

class OpenGLShape;

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget(QGLFormat format, QWidget *parent = 0, bool tab0 = true);
    ~GLWidget();

protected:
    /** This is called once, before any calls to paintGL. */
    void initializeGL();

    /** This is called every time the window needs to be repainted. */
    void paintGL();

    void resizeGL(int w, int h);

protected slots:
    /** Repaints the canvas. Called 60 times per second by m_timer. */
    void tick();

private:

    void initializeGLTransformationsVertexShaders();
    void initializeGLFragmentShaders();
    void paintGLTransformationsVertexShaders();
    void paintGLFragmentShaders();

    std::unique_ptr<OpenGLShape> m_square;

    GLuint m_solidProgramID;
    GLuint m_gradientProgramID;
    GLuint m_textureProgramID;

    GLuint m_textureID;
    bool m_tab0;

    /** ID for the shader program. */
    GLuint m_program;

    std::unique_ptr<OpenGLShape> m_sphere;

    /** Timer calls tick() 60 times per second. */
    QTimer m_timer;
    float m_fps;

    float m_tick;
    float m_angle;

    /** Incremented on every call to paintGL. */
    int m_increment;
};

#endif // GLWIDGET_H
