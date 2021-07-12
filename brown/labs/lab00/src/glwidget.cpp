#include "glwidget.h"
#include "settings.h"
#include <math.h>

#include "cs123_lib/resourceloader.h"
#include "openglshape.h"

#include "gl/shaders/ShaderAttribLocations.h"

using namespace CS123::GL;

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_program(0)
{

}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL() {
    ResourceLoader::initializeGlew();
    m_program = ResourceLoader::createShaderProgram(":/shaders/shader.vert", ":/shaders/shader.frag");
    glViewport(0, 0, width(), height());
    glEnable(GL_CULL_FACE); // Hides the back faces of triangles.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    std::vector<GLfloat> quadData;
        quadData = {-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 1.0f};
        m_quad = std::make_unique<OpenGLShape>();
        m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
        m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
        m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
        m_quad->buildVAO();
}

void GLWidget::paintGL() {
    glUseProgram(m_program);       // Installs the shader program. You'll learn about this later.
    glClear(GL_COLOR_BUFFER_BIT);  // Clears the color buffer. (i.e. Sets the screen to black.)

    // Gets the uniform location from the program.
    GLint uniformLoc = glGetUniformLocation(m_program, "color");

    printf("color: %i", (int) settings.color);

    switch (settings.color) {
        case COLOR_RED:
            glUniform3f(uniformLoc, 1.0, 0.0, 0.0);
            break;
        case COLOR_GREEN:
            glUniform3f(uniformLoc, 0.0, 1.0, 0.0);
            break;
        case COLOR_BLUE:
            glUniform3f(uniformLoc, 0.0, 0.0, 1.0);
            break;
    }

    m_quad->draw();

    glUseProgram(0); // Uninstalls the shader program.
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}
