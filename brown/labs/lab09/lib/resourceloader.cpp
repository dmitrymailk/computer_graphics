#include "resourceloader.h"

/**
  Loads the cube map into video memory.

  @param files: a list of files containing the cube map images (should be length
  six) in order
  @return the assigned OpenGL id to the cube map
**/
GLuint ResourceLoader::loadCubeMap(QList<QFile *> files)
{
    Q_ASSERT(files.length() == 6);

    // Generate an ID
    GLuint id;
    glGenTextures(1, &id);

    // Bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    // Load and build mipmaps for each image
    for (int i = 0; i < 6; ++i)
    {
        QImage image, texture;
        image.load(files[i]->fileName());
        image = image.mirrored(false, true);
        texture = QGLWidget::convertToGLFormat(image);
        texture = texture.scaledToWidth(2048, Qt::SmoothTransformation);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA,GL_UNSIGNED_BYTE, texture.bits());
    }

    // Set filter when pixel occupies more than one texture element
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Set filter when pixel smaller than one texture element
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return id;
}

/**
    Creates a call list for a skybox
  **/
GLuint ResourceLoader::loadSkybox()
{
    GLuint id = glGenLists(1);
    glNewList(id, GL_COMPILE);

    glBegin(GL_QUADS);
    float extent = 1000.f;
    glTexCoord3f( 1.0f, -1.0f, -1.0f); glVertex3f( extent, -extent, -extent);
    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-extent, -extent, -extent);
    glTexCoord3f(-1.0f,  1.0f, -1.0f); glVertex3f(-extent,  extent, -extent);
    glTexCoord3f( 1.0f,  1.0f, -1.0f); glVertex3f( extent,  extent, -extent);

    glTexCoord3f( 1.0f, -1.0f,  1.0f); glVertex3f( extent, -extent,  extent);
    glTexCoord3f( 1.0f, -1.0f, -1.0f); glVertex3f( extent, -extent, -extent);
    glTexCoord3f( 1.0f,  1.0f, -1.0f); glVertex3f( extent,  extent, -extent);
    glTexCoord3f( 1.0f,  1.0f,  1.0f); glVertex3f( extent,  extent,  extent);

    glTexCoord3f(-1.0f, -1.0f,  1.0f); glVertex3f(-extent, -extent,  extent);
    glTexCoord3f( 1.0f, -1.0f,  1.0f); glVertex3f( extent, -extent,  extent);
    glTexCoord3f( 1.0f,  1.0f,  1.0f); glVertex3f( extent,  extent,  extent);
    glTexCoord3f(-1.0f,  1.0f,  1.0f); glVertex3f(-extent,  extent,  extent);

    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-extent, -extent, -extent);
    glTexCoord3f(-1.0f, -1.0f,  1.0f); glVertex3f(-extent, -extent,  extent);
    glTexCoord3f(-1.0f,  1.0f,  1.0f); glVertex3f(-extent,  extent,  extent);
    glTexCoord3f(-1.0f,  1.0f, -1.0f); glVertex3f(-extent,  extent, -extent);

    glTexCoord3f(-1.0f,  1.0f, -1.0f); glVertex3f(-extent,  extent, -extent);
    glTexCoord3f(-1.0f,  1.0f,  1.0f); glVertex3f(-extent,  extent,  extent);
    glTexCoord3f( 1.0f,  1.0f,  1.0f); glVertex3f( extent,  extent,  extent);
    glTexCoord3f( 1.0f,  1.0f, -1.0f); glVertex3f( extent,  extent, -extent);

    glTexCoord3f(-1.0f, -1.0f, -1.0f); glVertex3f(-extent, -extent, -extent);
    glTexCoord3f( 1.0f, -1.0f, -1.0f); glVertex3f( extent, -extent, -extent);
    glTexCoord3f( 1.0f, -1.0f,  1.0f); glVertex3f( extent, -extent,  extent);
    glTexCoord3f(-1.0f, -1.0f,  1.0f); glVertex3f(-extent, -extent,  extent);
    glEnd();
    glEndList();
    return id;
}

/**
    Creates a new shader program from a vert shader only
  **/
QGLShaderProgram * ResourceLoader::newVertShaderProgram(const QGLContext *context, QString vertShader)
{
    QGLShaderProgram *program = new QGLShaderProgram(context);
    program->addShaderFromSourceFile(QGLShader::Vertex, vertShader);
    program->link();
    return program;
}

/**
    Creates a new shader program from a frag shader only
  **/
QGLShaderProgram * ResourceLoader::newFragShaderProgram(const QGLContext *context, QString fragShader)
{
    QGLShaderProgram *program = new QGLShaderProgram(context);
    program->addShaderFromSourceFile(QGLShader::Fragment, fragShader);
    program->link();
    return program;
}

/**
    Creates a shader program from both vert and frag shaders
  **/
QGLShaderProgram * ResourceLoader::newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader, QString *errors)
{
    QGLShaderProgram *program = new QGLShaderProgram(context);
    if (!program->addShaderFromSourceFile(QGLShader::Vertex, vertShader)) {
        if (errors) {
            *errors = program->log();
        }
        delete program;
        return NULL;
    }
    if (!program->addShaderFromSourceFile(QGLShader::Fragment, fragShader)) {
        if (errors) {
            *errors = program->log();
        }
        delete program;
        return NULL;
    }
    program->bindAttributeLocation("position", 0);
    program->bindAttributeLocation("normal", 1);
    program->bindAttributeLocation("texCoord", 2);
    if (!program->link()) {
        if (errors) {
            *errors = program->log();
        }
        delete program;
        return NULL;
    }
    return program;
}

void ResourceLoader::initializeGlew() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
}
