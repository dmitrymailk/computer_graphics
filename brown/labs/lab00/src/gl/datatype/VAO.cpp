#include "VAO.h"

#include "VBO.h"
#include "IBO.h"

namespace CS123 { namespace GL {

VAO::VAO(const VBO &vbo, int numberOfVerticesToRender) :
    m_drawMethod(DRAW_ARRAYS),
    m_handle(0),
    m_numVertices(numberOfVerticesToRender),
    m_size(0),
    m_triangleLayout(vbo.triangleLayout())
{
    // TODO [Task 3]
    // begin ta code
    glGenVertexArrays(1, &m_handle);
    // end ta code

    // TODO [Task 4]
    // begin ta code
    bind();
    vbo.bindAndEnable();
    unbind();
    vbo.unbind();
    // end ta code
}

VAO::VAO(const VBO &vbo, const IBO &ibo, int numberOfVerticesToRender) :
    m_drawMethod(DRAW_INDEXED),
    m_handle(0),
    m_numVertices(numberOfVerticesToRender),
    m_size(0),
    m_triangleLayout(vbo.triangleLayout())
{
    // TODO [OPTIONAL]
    // There's another way of drawing with OpenGL that uses IBOs,
    // or Index Buffer Objects.  Feel free to look them up or ask us
    // about them if you're curious!
    // This constructor should be almost identical to the one above,
    // just also bind the IBO after binding the vbo (and unbind it)
    // begin ta code
    glGenVertexArrays(1, &m_handle);
    bind();
    vbo.bindAndEnable();
    ibo.bind();
    unbind();
    ibo.unbind();
    vbo.unbind();
    // end ta code
}

VAO::VAO(VAO &&that) :
    m_drawMethod(that.m_drawMethod),
    m_numVertices(that.m_numVertices),
    m_size(that.m_size),
    m_triangleLayout(that.m_triangleLayout)
{
    that.m_handle = 0;
}

VAO& VAO::operator=(VAO &&that) {
    this->~VAO();

    m_drawMethod = that.m_drawMethod;
    m_handle = that.m_handle;
    m_numVertices = that.m_numVertices;
    m_size = that.m_size;
    m_triangleLayout = that.m_triangleLayout;

    that.m_handle = 0;

    return *this;
}

VAO::~VAO()
{
    // TODO [Task 6]
    // begin ta code
    glDeleteVertexArrays(1, &m_handle);
    // end ta code
}

void VAO::draw() {
    draw(m_numVertices);
}

void VAO::draw(int count) {
    switch(m_drawMethod) {
        case VAO::DRAW_ARRAYS:
            // TODO [Task 5]
            // begin ta code
            glDrawArrays(m_triangleLayout, 0, count);
            // end ta code
            break;
        case VAO::DRAW_INDEXED:
            // TODO [OPTIONAL]
            // If you want to use IBO's, you'll need to call glDrawElements here
            // begin ta code
            glDrawElements(m_triangleLayout, count, GL_UNSIGNED_INT, 0);
            // end ta code
            break;
    }
}

void VAO::bind() {
    // TODO [Task 4]
    // begin ta code
    glBindVertexArray(m_handle);
    // end ta code
}

void VAO::unbind() {
    // TODO [Task 4]
    // begin ta code
    glBindVertexArray(0);
    // end ta code
}

}}
