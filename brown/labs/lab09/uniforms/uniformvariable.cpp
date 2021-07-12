#include "uniformvariable.h"

#include "glwidget.h"
#include <QFileInfo>

GLuint UniformVariable::s_numTextures = 2;
QList<GLuint> UniformVariable::s_faceTextures;

QString UniformVariable::typeName(UniformVariable::Type type)
{
    switch (type) {
    case TYPE_BOOL:
        return "bool";

    case TYPE_INT:
        return "int";

    case TYPE_INT2:
    case TYPE_INT3:
    case TYPE_INT4:
        return QString("ivec%1").arg(type - TYPE_INT + 1);

    case TYPE_FLOAT:
        return "float";

    case TYPE_FLOAT2:
    case TYPE_FLOAT3:
    case TYPE_FLOAT4:
        return QString("vec%1").arg(type - TYPE_FLOAT + 1);

    case TYPE_MAT2:
    case TYPE_MAT3:
    case TYPE_MAT4:
        return QString("mat%1").arg(type - TYPE_MAT2 + 2);

    case TYPE_TEX1D:
        return "sampler1D";

    case TYPE_TEX2D:
        return "sampler2D";

    case TYPE_TEXCUBE:
        return "samplerCube";

    case TYPE_TIME:
        return "time (float)";

    default:
        return "unknown";
    }
}

UniformVariable::Type UniformVariable::typeFromName(const QString &name)
{
    for (int i = TYPE_START; i <= TYPE_END; i++) {
        if (typeName(static_cast<Type>(i)) == name) return static_cast<Type>(i);
    }
    return TYPE_UNKNOWN;
}

UniformVariable::Type UniformVariable::typeFromGLEnum(GLenum type)
{
    switch (type) {
    case GL_INT:
        return TYPE_INT;
    case GL_INT_VEC2:
        return TYPE_INT2;
    case GL_INT_VEC3:
        return TYPE_INT3;
    case GL_INT_VEC4:
        return TYPE_INT4;

    case GL_FLOAT:
        return TYPE_FLOAT;
    case GL_FLOAT_VEC2:
        return TYPE_FLOAT2;
    case GL_FLOAT_VEC3:
        return TYPE_FLOAT3;
    case GL_FLOAT_VEC4:
        return TYPE_FLOAT4;

    case GL_FLOAT_MAT2:
        return TYPE_MAT2;
    case GL_FLOAT_MAT3:
        return TYPE_MAT3;
    case GL_FLOAT_MAT4:
        return TYPE_MAT4;

    case GL_SAMPLER_1D:
        return TYPE_TEX1D;
    case GL_SAMPLER_2D:
        return TYPE_TEX2D;
    case GL_SAMPLER_CUBE:
        return TYPE_TEXCUBE;

    case GL_BOOL:
        return TYPE_BOOL;
    }
    return TYPE_UNKNOWN;
}

QTime g_time;
bool g_timeInitialized = false;

GLfloat UniformVariable::timeValue()
{
    return g_time.elapsed() / 1000.f;
}

void UniformVariable::resetTimer()
{
    g_time.restart();
}

UniformVariable::UniformVariable(QOpenGLContext *ctx)
{
    if (!g_timeInitialized) {
        g_time.start();
        g_timeInitialized = true;
    }
    if (s_faceTextures.isEmpty()) {
        for (int i = 0; i < 6; i++) {
            s_faceTextures.append(0);
        }
        s_faceTextures[FACE_BACK] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        s_faceTextures[FACE_FRONT] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        s_faceTextures[FACE_TOP] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        s_faceTextures[FACE_BOTTOM] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        s_faceTextures[FACE_LEFT] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        s_faceTextures[FACE_RIGHT] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }
    name = "New Uniform";
    asciiName = name.toUtf8();
    type = TYPE_FLOAT;
    floatVal = new GLfloat[1];
    *floatVal = 0;
    intVal = 0;
    elements = 1;
    texID = 0;
    gl = new QOpenGLFunctions(ctx);
    copyFrom = this;
    permanent = false;
    arraySize = maxSizeUsed = 1;
}

UniformVariable::UniformVariable(QOpenGLContext *ctx, UniformVariable *u)
{
    if (!g_timeInitialized) {
        g_time.start();
        g_timeInitialized = true;
    }
    if (s_faceTextures.isEmpty()) {
        for (int i = 0; i < 6; i++) {
            s_faceTextures.append(0);
        }
        s_faceTextures[FACE_BACK] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        s_faceTextures[FACE_FRONT] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        s_faceTextures[FACE_TOP] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        s_faceTextures[FACE_BOTTOM] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        s_faceTextures[FACE_LEFT] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        s_faceTextures[FACE_RIGHT] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }
    name = u->name;
    asciiName = name.toUtf8();
    type = u->type;
    texID = 0;
    parse(u->toString());
    gl = new QOpenGLFunctions(ctx);
    copyFrom = u->copyFrom;
    permanent = u->permanent;
    arraySize = u->arraySize;
    maxSizeUsed = u->maxSizeUsed;
}

UniformVariable::~UniformVariable()
{
    delete[] floatVal;
    delete[] intVal;
    if (texID) {
        glDeleteTextures(1, &texID);
    }
    delete gl;
}

const QString &UniformVariable::getName() const
{
    return name;
}

const char *UniformVariable::getAsciiName() const
{
    return asciiName.constData();
}

void UniformVariable::setName(const QString &name)
{
    this->name = name;
    asciiName = name.toUtf8();
}

UniformVariable::Type UniformVariable::getType() const
{
    return type;
}

void UniformVariable::setType(Type type)
{
    if (this->type == type) return;

    this->type = type;

    delete[] floatVal;
    floatVal = 0;

    delete[] intVal;
    intVal = 0;

    switch(type) {
    case TYPE_INT:
    case TYPE_FLOAT:
    case TYPE_TEX1D:
    case TYPE_TEX2D:
    case TYPE_TIME:
    case TYPE_BOOL:
        elements = 1;
        break;

    case TYPE_INT2:
    case TYPE_FLOAT2:
        elements = 2;
        break;

    case TYPE_INT3:
    case TYPE_FLOAT3:
        elements = 3;
        break;

    case TYPE_FLOAT4:
    case TYPE_INT4:
    case TYPE_MAT2:
        elements = 4;
        break;

    case TYPE_MAT3:
        elements = 3 * 3;
        break;

    case TYPE_MAT4:
        elements = 4 * 4;
        break;

    case TYPE_TEXCUBE:
        elements = 6;

    default:
        elements = 0;
    }

    if (type > TYPE_INT_VALS_START && type < TYPE_INT_VALS_END) {
        intVal = new GLint[elements*arraySize];
        memset(intVal, 0, sizeInBytes());
        floatVal = new GLfloat[elements];
        memset(floatVal, 0, sizeInBytes());
    } else if (type > TYPE_FLOAT_VALS_START && type < TYPE_FLOAT_VALS_END) {
        floatVal = new GLfloat[elements*arraySize];
        if (type == TYPE_TIME) return;
        memset(floatVal, 0, sizeInBytes());
    }
}

QString UniformVariable::typeName() const
{
    return typeName(type);
}

bool UniformVariable::isFloatValue() const
{
    return floatVal != 0;
}

bool UniformVariable::isIntValue() const
{
    return intVal != 0;
}

bool UniformVariable::isImage() const
{
    return type > TYPE_TEXTURE_START && type < TYPE_TEXTURE_END;
}

const GLfloat *UniformVariable::floatValue() const
{
    if (type == TYPE_TIME)
        *floatVal = timeValue();

    return floatVal;
}

const GLint *UniformVariable::intValue() const
{
    return intVal;
}

bool UniformVariable::loadImage(const QString &path)
{
    if (!QFileInfo(path).exists())
        return false;
    QImage img;
    if (!img.load(path))
        return false;

    files.clear();
    files += path;

    QImage oglFormat = QGLWidget::convertToGLFormat(img);

    if (texID) {
        glDeleteTextures(1, &texID);
    }

    glGenTextures(1, &texID);
    texOffset = s_numTextures++;

    //gl->glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    gl->glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, oglFormat.width(), oglFormat.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, oglFormat.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    //gl->glActiveTexture(GL_TEXTURE0);

    return true;
}

bool UniformVariable::loadImage(UniformVariable::Face index, const QString &path)
{
    if (!QFileInfo(path).exists() && !path.startsWith(":"))
        return false;

    QImage img;
    if (!img.load(path))
        return false;

    img = img.mirrored(false, true);

    files[index] = path;
    QImage oglFormat = QGLWidget::convertToGLFormat(img);

    GLuint texTarget = s_faceTextures[index];
    if (!texID) {
        glGenTextures(1, &texID);
    }
    texOffset = s_numTextures++;
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(texTarget, 0, GL_RGBA, oglFormat.width(), oglFormat.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, oglFormat.bits());

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}

size_t UniformVariable::sizeInBytes() const
{
    if (isIntValue()) return sizeof(GLint) * sizeInElements();
    else if (isFloatValue()) return sizeof(GLfloat) * sizeInElements();
    else return 0;
}

size_t UniformVariable::sizeInElements() const
{
    return elements;
}

QString UniformVariable::toString() const
{
    if (strVal.compare("")) {
        return strVal;
    }
    if (type == TYPE_BOOL) {
        return intVal[0] ? "true" : "false";
    }
    if (type == TYPE_TIME)
        return QString::number(timeValue());

    QString str = "";


    if (isFloatValue() || isIntValue()) {
        for (int i = 0; i < elements * maxSizeUsed; i++) {
            if (isFloatValue())
                str += QString::number(floatVal[i]);
            else
                str += QString::number(intVal[i]);

            if (i + 1 < elements * maxSizeUsed)
                str += ",";
        }
    } else if (isImage()) {
        for (int i = 0; i < files.count(); i++) {
            str += files[i];

            if (i+1 < elements)
                str += ",";
        }
    }

    return str;
}

UniformVariable *checkStatics(const QString &str, UniformVariable::Type type) {
    switch (type) {
    case UniformVariable::TYPE_MAT4:
        if (qstrcmp(str, "<model>")) return GLWidget::s_model;
        if (qstrcmp(str, "<view>")) return GLWidget::s_view;
        if (qstrcmp(str, "<projection>")) return GLWidget::s_projection;
        if (qstrcmp(str, "<mvp>")) return GLWidget::s_mvp;
        break;
    case UniformVariable::TYPE_TEXCUBE:
        if (qstrcmp(str, "<skybox>")) return GLWidget::s_skybox;
        break;
    case UniformVariable::TYPE_FLOAT:
        if (qstrcmp(str, "<time>")) return GLWidget::s_time;
        break;
    case UniformVariable::TYPE_FLOAT2:
        if (qstrcmp(str, "<size>")) return GLWidget::s_size;
    case UniformVariable::TYPE_FLOAT3:
        if (qstrcmp(str, "<mouse>")) return GLWidget::s_mouse;
    default:
        return NULL;
    }
    return NULL;
}

bool UniformVariable::parse(const QString &value, bool verifyOnly)
{
    copyFrom = checkStatics(value, this->type);
    if (copyFrom) {
        strVal = value;
        return true;
    }
    strVal = "";
    copyFrom = this;

    QStringList paths;
    switch(type) {
    case TYPE_TEX1D:
    case TYPE_TEX2D:
        if (verifyOnly) {
            QImage img;
            return img.load(value);
        }
        return loadImage(value);
    case TYPE_TEXCUBE:
        paths = value.split(",");
        if (paths.count() != 6) {
            paths = value.split(":"); // Cupcake legacy support
            if (paths.count() != 6) return false;
        }

        if (texID) {
            glDeleteTextures(1, &texID);
            texID = 0;
        }

        files.clear();
        for (int i = 0; i < 6; i++) {
            QString path = paths[i].trimmed();
            if (verifyOnly) {
                QImage img;
                if (!img.load(path)) return false;
            } else {
                files.append(0);
                if (!loadImage((Face)i, path)) return false;
            }
        }
        return true;
    case TYPE_BOOL:
        if (qstrcmpl(value, "true")) {
            if (!verifyOnly) {
                intVal[0] = GL_TRUE;
            }
        } else if (qstrcmpl(value, "false")) {
            if (!verifyOnly) {
                intVal[0] = GL_FALSE;
            }
        } else
            return false;
        return true;
    default:
        break;
    }

    QStringList components = value.split(",");
    if (arraySize == 1 && components.count() != elements) {
        return false;
    }
    if (arraySize > 1 && (components.count() % elements != 0 || components.count() > elements * arraySize)) {
        return false;
    }

    maxSizeUsed = components.count()/elements;

    for (int i = 0; i < components.size(); i++) {
        if (isIntValue()) {
            bool ok; int value;
            value = components[i].toInt(&ok);

            if (!ok) {
                return false;
            }

            if (!verifyOnly) {
                intVal[i] = value;
                floatVal[i] = value;
            }
        } else if (isFloatValue()) {
            bool ok; float value;
            value = components[i].toFloat(&ok);

            if (!ok) {
                return false;
            }

            if (!verifyOnly) {
                floatVal[i] = value;
            }
        }
    }

    return true;
}

void UniformVariable::setValue(QGLShaderProgram *shader) const
{
    switch (type) {
    case TYPE_INT:
    case TYPE_BOOL:
        shader->setUniformValue(getAsciiName(), copyFrom->intVal[0]);
        break;

    case TYPE_INT2:
    case TYPE_INT3:
    case TYPE_INT4:
        shader->setUniformValueArray(getAsciiName(), copyFrom->floatValue(), arraySize, copyFrom->elements);
        break;

    case TYPE_FLOAT:
        if (copyFrom->type == TYPE_TIME)
            shader->setUniformValue(getAsciiName(), copyFrom->timeValue());
        else
            shader->setUniformValueArray(getAsciiName(), copyFrom->floatValue(), arraySize, copyFrom->elements);
        break;

    case TYPE_TIME:
        shader->setUniformValue(getAsciiName(), copyFrom->timeValue());
        break;

    case TYPE_FLOAT2:
    case TYPE_FLOAT3:
    case TYPE_FLOAT4:
        shader->setUniformValueArray(getAsciiName(), copyFrom->floatValue(), arraySize, copyFrom->elements);
        break;

    case TYPE_MAT2:
        gl->glUniformMatrix2fv(gl->glGetUniformLocation(shader->programId(), getAsciiName()), arraySize, GL_TRUE, copyFrom->floatValue());
        break;

    case TYPE_MAT3:
        gl->glUniformMatrix3fv(gl->glGetUniformLocation(shader->programId(), getAsciiName()), arraySize, GL_TRUE, copyFrom->floatValue());
        break;

    case TYPE_MAT4:
        gl->glUniformMatrix4fv(gl->glGetUniformLocation(shader->programId(), getAsciiName()), arraySize, GL_TRUE, copyFrom->floatValue());
        break;

    case TYPE_TEX1D:
    case TYPE_TEX2D:

        gl->glActiveTexture(GL_TEXTURE0 + copyFrom->texOffset);
        glBindTexture(GL_TEXTURE_2D, copyFrom->texID);
        gl->glUniform1i(gl->glGetUniformLocation(shader->programId(), copyFrom->getAsciiName()), copyFrom->texOffset);
        shader->setUniformValue(getAsciiName(), copyFrom->texOffset);

        gl->glActiveTexture(GL_TEXTURE0);
        break;
    case TYPE_TEXCUBE:
        gl->glActiveTexture(GL_TEXTURE0 + copyFrom->texOffset);
        glBindTexture(GL_TEXTURE_CUBE_MAP, copyFrom->texID);
        gl->glUniform1i(gl->glGetUniformLocation(shader->programId(), getAsciiName()), copyFrom->texOffset);

        gl->glActiveTexture(GL_TEXTURE0);
        break;
    default:break;

    }
}

void UniformVariable::setCopyFrom(UniformVariable *toCopy)
{
    copyFrom = toCopy;
}

void UniformVariable::setPermanent(bool perm)
{
    permanent = perm;
}

bool UniformVariable::getPermanent() const
{
    return permanent;
}

void UniformVariable::setArraySize(int size)
{
    arraySize = size;
    Type old = type;
    this->setType(TYPE_START);
    this->setType(old);
}

int UniformVariable::getArrySize() const {
    return arraySize;
}
