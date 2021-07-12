#include "TextureParameters.h"

#include "gl/GLDebug.h"
#include "gl/textures/Texture.h"
#include "gl/textures/Texture2D.h"

namespace CS123 { namespace GL {


TextureParameters::TextureParameters(FILTER_METHOD filterMethod, WRAP_METHOD wrapMethod) :
    m_filterMethod(filterMethod),
    m_wrapMethod(wrapMethod)
{
}

void TextureParameters::applyTo(const Texture2D &texture) {
    texture.bind();
    GLenum filterEnum = (GLenum)m_filterMethod;
    GLenum wrapEnum = (GLenum)m_wrapMethod;
    // TODO [Task 2] call glTexParameteri to set:
    // - the min filter and mag filter to filterEnum
    // - the s wrap and t wrap to wrapEnum

    texture.unbind();
}

}}
