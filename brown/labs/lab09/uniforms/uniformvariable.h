#ifndef UNIFORMVARIABLE_H
#define UNIFORMVARIABLE_H
#include "GL/glew.h"

#include <QGLShaderProgram>
#include <QTime>
#include <QOpenGLFunctions>

#include "lib/common.h"


class UniformVariable
{
public:
    enum Type {
        TYPE_START,
        TYPE_UNKNOWN,

        TYPE_INT_VALS_START,
        TYPE_INT,
        TYPE_BOOL,
        TYPE_INT2,
        TYPE_INT3,
        TYPE_INT4,
        TYPE_INT_VALS_END,

        TYPE_FLOAT_VALS_START,
        TYPE_FLOAT,
        TYPE_FLOAT2,
        TYPE_FLOAT3,
        TYPE_FLOAT4,

        TYPE_MAT2,
        TYPE_MAT3,
        TYPE_MAT4,

        TYPE_TIME,
        TYPE_FLOAT_VALS_END,

        TYPE_TEXTURE_START,
        TYPE_TEX1D,
        TYPE_TEX2D,
        TYPE_TEXCUBE,
        TYPE_TEXTURE_END,


        TYPE_END
    };

    enum Face {
        FACE_TOP,
        FACE_BOTTOM,
        FACE_LEFT,
        FACE_RIGHT,
        FACE_FRONT,
        FACE_BACK
    };

    static QString typeName(Type type);
    static Type typeFromName(const QString &name);
    static Type typeFromGLEnum(GLenum type);

    static GLfloat timeValue();
    static void resetTimer();

    UniformVariable(QOpenGLContext *ctx);
    UniformVariable(QOpenGLContext *ctx, UniformVariable *u);
    virtual ~UniformVariable();

    const QString& getName() const;
    const char* getAsciiName() const;
    void setName(const QString&);

    Type getType() const;
    void setType(Type);

    QString typeName() const;

    bool isFloatValue() const;
    bool isIntValue() const;
    bool isImage() const;

    const GLfloat *floatValue() const;
    const GLint *intValue() const;
    bool loadImage(const QString &path);
    bool loadImage(Face index, const QString &path);

    size_t sizeInBytes() const;
    size_t sizeInElements() const;

    QString toString() const;
    bool parse(const QString &value, bool verifyOnly = false);

    void setValue(QGLShaderProgram *shader) const;

    static GLuint s_numTextures;

    void setCopyFrom(UniformVariable *toCopy);

    void setPermanent(bool perm);
    bool getPermanent() const;

    void setArraySize(int size);
    int getArrySize() const;

protected:
    QString name;
    QByteArray asciiName;

    Type type;
    float *floatVal;
    int *intVal;
    int elements;
    GLuint texID;
    GLuint texOffset;

    static QList<GLuint> s_faceTextures;

    QOpenGLFunctions *gl;
    QList<QString> files;

    UniformVariable *copyFrom;
    QString strVal;

    bool permanent;

    int arraySize;
    int maxSizeUsed;

};

#endif // UNIFORMVARIABLE_H
