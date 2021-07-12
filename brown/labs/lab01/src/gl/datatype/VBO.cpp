#include "VBO.h"

#include "gl/datatype/VBOAttribMarker.h"

namespace CS123 { namespace GL {

// This will count up the total size of each vertex, based on the maximum offset + numElements
unsigned int calculateFloatsPerVertex(const std::vector<VBOAttribMarker> &markers) {
    unsigned int max = 0;
    for (auto it = markers.begin(); it!= markers.end(); it++) {
        max = std::max(max, static_cast<unsigned int>(it->numElements + it->offset / sizeof(GLfloat)));
    }
    return max;
}

VBO::VBO(const float *data, int sizeInFloats, std::vector<VBOAttribMarker> markers, GEOMETRY_LAYOUT layout) :
    m_handle(-1),
    m_markers(markers),
    m_bufferSizeInFloats(sizeInFloats),
    m_numberOfFloatsPerVertex(calculateFloatsPerVertex(markers)),
    m_stride(m_numberOfFloatsPerVertex * sizeof(GLfloat)),
    m_triangleLayout(layout)
{
    // TODO [Task 1]

    // TODO [Task 2]
}

// This is called a copy constructor, you don't have to worry about it
VBO::VBO(VBO &&that) :
    m_handle(that.m_handle),
    m_markers(std::move(that.m_markers)),
    m_bufferSizeInFloats(that.m_bufferSizeInFloats),
    m_numberOfFloatsPerVertex(that.m_numberOfFloatsPerVertex),
    m_stride(that.m_stride),
    m_triangleLayout(that.m_triangleLayout)
{
    that.m_handle = 0;
}

// This will also deep copy a VBO
VBO& VBO::operator=(VBO &&that) {
    this->~VBO();

    m_handle = that.m_handle;
    m_markers = std::move(that.m_markers);
    m_bufferSizeInFloats = that.m_bufferSizeInFloats;
    m_numberOfFloatsPerVertex = that.m_numberOfFloatsPerVertex;
    m_stride = that.m_stride;
    m_triangleLayout = that.m_triangleLayout;

    that.m_handle = 0;

    return *this;
}

VBO::~VBO()
{
    // TODO [Task 6]
}

void VBO::bind() const {
    // TODO [Task 4]
}

void VBO::bindAndEnable() const {
    bind();
    for (unsigned int i = 0; i < m_markers.size(); i++) {
        VBOAttribMarker am = m_markers[i];

        // TODO [Task 4]
    }
}

void VBO::unbind() const {
    // TODO [Task 4]
}

int VBO::numberOfFloatsPerVertex() const {
    return m_numberOfFloatsPerVertex;
}

int VBO::numberOfVertices() const {
    return m_bufferSizeInFloats / m_numberOfFloatsPerVertex;
}

VBO::GEOMETRY_LAYOUT VBO::triangleLayout() const {
    return m_triangleLayout;
}

}}
