#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif
#include "terrain.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget(QGLFormat format, QWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:
    void rebuildMatrices();

    /** ID for the shader program. */
    GLuint m_program;

    Terrain m_terrain;

    glm::mat4 m_model, m_view, m_projection;

    /** For mouse interaction. */
    float m_angleX, m_angleY, m_zoom;
    QPoint m_prevMousePos;
};

#endif // GLWIDGET_H
