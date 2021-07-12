#include "glwidget.h"
#include <QMouseEvent>
#include <sstream>

#include "shapes/sphere.h"
#include "shapes/cube.h"
#include "camera/orbitingcamera.h"
#include "lib/resourceloader.h"
#include "uniforms/varsfile.h"
#include "gl/shaders/shaderattriblocations.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

UniformVariable *GLWidget::s_skybox = NULL;
UniformVariable *GLWidget::s_projection = NULL;
UniformVariable *GLWidget::s_model = NULL;
UniformVariable *GLWidget::s_view = NULL;
UniformVariable *GLWidget::s_mvp = NULL;
UniformVariable *GLWidget::s_time = NULL;
UniformVariable *GLWidget::s_size = NULL;
UniformVariable *GLWidget::s_mouse = NULL;
std::vector<UniformVariable*> *GLWidget::s_staticVars = NULL;

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent), m_sphere(nullptr), m_cube(nullptr), m_shape(nullptr), skybox_cube(nullptr)
{
    camera = new OrbitingCamera();
    QObject::connect(camera, SIGNAL(viewChanged(glm::mat4)), this, SLOT(viewChanged(glm::mat4)));
    QObject::connect(camera, SIGNAL(projectionChanged(glm::mat4)), this, SLOT(projectionChanged(glm::mat4)));
    QObject::connect(camera, SIGNAL(modelviewProjectionChanged(glm::mat4)), this, SLOT(modelviewProjectionChanged(glm::mat4)));

    activeUniforms = new QList<const UniformVariable *>();
    current_shader = NULL;
    wireframe_shader2 = NULL;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000.0f/60.0f);

    s_staticVars = new std::vector<UniformVariable*>();

    changeAnimMode(ANIM_NONE);

    drawWireframe = true;
    wireframeMode = WIREFRAME_NORMAL;
    mouseDown = false;
    setMouseTracking(true);
}

GLWidget::~GLWidget() {
    delete camera;

    delete activeUniforms;
    delete timer;

    delete skybox_shader;
    delete wireframe_shader;

    foreach (const UniformVariable *v, permUniforms) {
        delete v;
    }
}

bool GLWidget::saveUniforms(QString path)
{
    QList<const UniformVariable *> toSave;
    foreach (const UniformVariable *v, *activeUniforms) {
        toSave += v;
    }
    foreach (const UniformVariable *v, permUniforms) {
        toSave += v;
    }

    return VarsFile::save(path, &toSave);
}

bool GLWidget::loadUniforms(QString path)
{
    QList<const UniformVariable*> fromFile;

    foreach (const UniformVariable *v, permUniforms) {
        delete v;
    }
    permUniforms.clear();

    if (!VarsFile::load(path, &fromFile, context()->contextHandle())) return false;

    bool match;
    foreach (const UniformVariable *v, fromFile) {
        match = false;
        foreach (const UniformVariable *u, *activeUniforms) {
            if (!v->getName().compare(u->getName()) && v->getType() == u->getType()) {
                // Really really really bad, but the best option I can think of for now
                // sskybox_shaderetPermanent doesn't really modify the object much, sort of a small
                // flag for how to save it
                UniformVariable *utemp = const_cast<UniformVariable *>(u);
                utemp->setPermanent(v->getPermanent());
                emit(changeUniform(u, v->toString()));
                match = true;
            }
        }

        if (!match && v->getPermanent()) {
            permUniforms += v;
        } else {
            delete v;
        }
    }
    return true;
}

void GLWidget::initializeGL() {
    ResourceLoader::initializeGlew();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);

    glDisable(GL_COLOR_MATERIAL);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    skybox_shader = ResourceLoader::newShaderProgram(context(), ":/shaders/skybox.vert", ":/shaders/skybox.frag");
    wireframe_shader = ResourceLoader::newShaderProgram(context(), ":/shaders/standard.vert", ":/shaders/color.frag");

    s_skybox = new UniformVariable(this->context()->contextHandle());
    s_skybox->setName("skybox");
    s_skybox->setType(UniformVariable::TYPE_TEXCUBE);
    //top, bottom, left, right, front, back
    s_skybox->parse(":/skybox/posy.jpg,:/skybox/negy.jpg,:/skybox/negx.jpg,:/skybox/posx.jpg,:/skybox/posz.jpg,:/skybox/negz.jpg");

    s_model = new UniformVariable(this->context()->contextHandle());
    s_model->setName("model");
    s_model->setType(UniformVariable::TYPE_MAT4);
    s_model->parse("1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1");

    s_projection = new UniformVariable(this->context()->contextHandle());
    s_projection->setName("projection");
    s_projection->setType(UniformVariable::TYPE_MAT4);

    s_view = new UniformVariable(this->context()->contextHandle());
    s_view->setName("view");
    s_view->setType(UniformVariable::TYPE_MAT4);

    s_mvp = new UniformVariable(this->context()->contextHandle());
    s_mvp->setName("mvp");
    s_mvp->setType(UniformVariable::TYPE_MAT4);

    s_time = new UniformVariable(this->context()->contextHandle());
    s_time->setName("time");
    s_time->setType(UniformVariable::TYPE_TIME);

    s_size = new UniformVariable(this->context()->contextHandle());
    s_size->setName("size");
    s_size->setType(UniformVariable::TYPE_FLOAT2);

    s_mouse = new UniformVariable(this->context()->contextHandle());
    s_mouse->setName("mouse");
    s_mouse->setType(UniformVariable::TYPE_FLOAT3);

    s_staticVars->push_back(s_skybox);
    s_staticVars->push_back(s_model);
    s_staticVars->push_back(s_projection);
    s_staticVars->push_back(s_view);
    s_staticVars->push_back(s_mvp);
    s_staticVars->push_back(s_time);
    s_staticVars->push_back(s_size);
    s_staticVars->push_back(s_mouse);

    gl = QOpenGLFunctions(context()->contextHandle());

    std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;
    m_sphere = std::make_unique<OpenGLShape>();
    m_sphere->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->setAttribute(ShaderAttrib::NORMAL, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->buildVAO();

    std::vector<GLfloat> cubeData = CUBE_DATA_POSITIONS;
    m_cube = std::make_unique<OpenGLShape>();
    m_cube->setVertexData(&cubeData[0], cubeData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_CUBE_VERTICES);
    m_cube->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_cube->setAttribute(ShaderAttrib::NORMAL, 3, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_cube->buildVAO();

    skybox_cube = std::make_unique<OpenGLShape>();
    skybox_cube->setVertexData(&cubeData[0], cubeData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_CUBE_VERTICES);
    skybox_cube->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    skybox_cube->setAttribute(ShaderAttrib::NORMAL, 3, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    skybox_cube->buildVAO();

    m_shape = m_sphere.get();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    s_size->parse(QString("%1,%2").arg(QString::number(w), QString::number(h)));
    camera->setAspectRatio(((float) w) / ((float) h));
    update();
}

void GLWidget::handleAnimation() {
    model = glm::mat4();
    switch (animMode) {
    case ANIM_NONE:
        break;
    case ANIM_MOVE_AND_SCALE:
    case ANIM_SCALE:
        if (scale > 2) dscale *= -1;
        if (scale < .5) dscale *= -1;
        scale += scale * dscale;
        model = glm::scale(model, glm::vec3(scale));
        if (animMode == ANIM_SCALE)
            break;
    case ANIM_MOVE:
        if (pos.y > 2) {
            dir *= -1;
        }
        if (pos.y < -2) {
            dir *= -1;
        }
        pos += dir;
        model = glm::translate(model, pos);
        break;
    case ANIM_ROTATE:
        angle += dangle;
        model = glm::rotate(model, degreesToRadians(angle), glm::vec3(0,1,0));
        break;
    case ANIM_ROTATE_2:
        angle += dangle;
        model = glm::rotate(model, degreesToRadians(angle), glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(0, 2, 0));
        break;
    default:
        break;
    }
    modelChanged(model);
    modelviewProjectionChanged(camera->getProjectionMatrix() * camera->getModelviewMatrix());
}

void GLWidget::paintGL() {
    handleAnimation();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox_shader->bind();
    s_skybox->setValue(skybox_shader);
    s_projection->setValue(skybox_shader);
    s_view->setValue(skybox_shader);
    glCullFace(GL_FRONT);
    skybox_cube->draw();
    glCullFace(GL_BACK);
    skybox_shader->release();

    // Geometry
    if (m_shape) {

        if (current_shader) {
            current_shader->bind();

            foreach (const UniformVariable *var, *activeUniforms) {
                var->setValue(current_shader);
            }
        }

        m_shape->draw();
        if (current_shader) {
            current_shader->release();
        }

        if (drawWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            switch(wireframeMode) {
            case WIREFRAME_NORMAL:
                wireframe_shader->bind();
                s_mvp->setValue(wireframe_shader);
                wireframe_shader->setUniformValue("color", 0, 0, 0, 1);
                m_shape->draw();
                wireframe_shader->release();
                break;
            case WIREFRAME_VERT:
                wireframe_shader2->bind();
                foreach (const UniformVariable *var, *activeUniforms) {
                    var->setValue(wireframe_shader2);
                }
                wireframe_shader2->setUniformValue("color", 0, 0, 0, 1);
                m_shape->draw();
                wireframe_shader2->release();
                break;
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void GLWidget::changeRenderMode(RenderType mode)
{
    m_renderMode = mode;
    switch(m_renderMode) {
    case SHAPE_SPHERE:
        m_shape = m_sphere.get();
        break;
    case SHAPE_CUBE:
        m_shape = m_cube.get();
        break;
    default:
        break;
    }
}

void GLWidget::changeAnimMode(AnimType mode)
{
    model = glm::mat4();
    animMode = mode;
    pos = glm::vec3(0);
    dir = glm::vec3(0,.03,0);
    scale = 1;
    dscale = .017;
    angle = 0;
    dangle = 2;
}

void GLWidget::toggleDrawWireframe(bool draw)
{
    drawWireframe = draw;
}

void GLWidget::setWireframeMode(WireframeType mode)
{
    wireframeMode = mode;
}

bool GLWidget::loadShader(QString vert, QString frag, QString *errors)
{
    QGLShaderProgram *new_shader = ResourceLoader::newShaderProgram(context(), vert, frag, errors);
    if (new_shader == NULL) {
        return false;
    }

    delete wireframe_shader2;
    wireframe_shader2 = ResourceLoader::newShaderProgram(context(), vert, ":/shaders/color.frag", errors);

    UniformVariable::s_numTextures = 2;

    UniformVariable::resetTimer();

    // http://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade

    std::vector<GLchar> nameData(256);
    GLint numActiveUniforms = 0;
    gl.glGetProgramiv(new_shader->programId(), GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    for (int unif = 0; unif < numActiveUniforms; unif++) {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        gl.glGetActiveUniform(new_shader->programId(), unif, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
        std::string name((char*)&nameData[0], actualLength);

        UniformVariable::Type uniformType = UniformVariable::typeFromGLEnum(type);

        QString qname = QString::fromStdString(name);
        if (qname.startsWith("gl_")) continue;
        emit(addUniform(uniformType, qname, true, arraySize));
    }

    delete current_shader;
    current_shader = new_shader;
    camera->mouseScrolled(0);
    camera->updateMats();
    update();
    return true;
}

void GLWidget::uniformDeleted(const UniformVariable *uniform)
{
    foreach (UniformVariable *sv, *s_staticVars) {
        if (uniform == sv) return;
    }

    foreach (const UniformVariable *v, *activeUniforms) {
        if (uniform == v)
            delete v;
    }
    activeUniforms->removeAll(uniform);
}

void GLWidget::uniformAdded(const UniformVariable *uniform)
{
    activeUniforms->append(uniform);
}

void GLWidget::viewChanged(const glm::mat4 &modelview)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(modelview));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_view->parse(QString::fromStdString(s.str()));
}

void GLWidget::projectionChanged(const glm::mat4 &projection)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(projection));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_projection->parse(QString::fromStdString(s.str()));
}

void GLWidget::modelviewProjectionChanged(const glm::mat4 &modelviewProjection)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(modelviewProjection * model));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_mvp->parse(QString::fromStdString(s.str()));
}

void GLWidget::modelChanged(const glm::mat4 &modelview)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(modelview));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_model->parse(QString::fromStdString(s.str()));
}

void GLWidget::setPaused(bool paused)
{
    if (paused) {
        timer->stop();
    } else {
        timer->start(1000.0f/60.0f);
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        camera->mouseDragged(event->x(), event->y());
    }
    s_mouse->parse(QString("%1,%2,%3").arg(
                       QString::number(event->x()),
                       QString::number(event->y()),
                       QString::number(mouseDown)));
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    camera->mouseScrolled(event->delta());
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    camera->mouseDown(event->x(), event->y());
    mouseDown = true;
    s_mouse->parse(QString("%1,%2,%3").arg(
                       QString::number(event->x()),
                       QString::number(event->y()),
                       QString::number(mouseDown)));
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    mouseDown = false;
    s_mouse->parse(QString("%1,%2,%3").arg(
                       QString::number(event->x()),
                       QString::number(event->y()),
                       QString::number(mouseDown)));
}
