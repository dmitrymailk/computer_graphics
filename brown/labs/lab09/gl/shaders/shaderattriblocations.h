#ifndef SHADERATTRIBLOCATIONS_H
#define SHADERATTRIBLOCATIONS_H

/**
 *
 * ***IMPORTANT FOR WRITING SHADERS***
 *
 *
 * All shaders (CS123's default and any of your custom ones) must specify the layout locations
 * to match the layout locations in that file.
 */
namespace CS123 { namespace GL { namespace ShaderAttrib {

    // Standard attributes
    const GLuint POSITION = 0;
    const GLuint NORMAL = 1;
    const GLuint TEXCOORD = 2;

}}}

#endif // SHADERATTRIBLOCATIONS_H
