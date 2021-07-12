#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "GL/glew.h"

#include <QFile>
#include <QGLShaderProgram>

/**
   A resource loader with code to handle loading models, skyboxes, and shader programs.

  @author: Justin Ardini (jardini)
   **/
namespace ResourceLoader
{
    // Returns the skybox ID
    GLuint loadSkybox();

    // These return a new QGLShaderProgram.  THIS MUST BE DELETED BY THE CALLER.
    QGLShaderProgram * newVertShaderProgram(const QGLContext *context, QString vertShader);
    QGLShaderProgram * newFragShaderProgram(const QGLContext *context, QString fragShader);
    QGLShaderProgram * newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader, QString *errors = 0);

    // Returns the cubeMap ID
    GLuint loadCubeMap(QList<QFile *> files);

    void initializeGlew();
}

#endif // RESOURCELOADER_H
