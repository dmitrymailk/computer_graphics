/*!

   @file   OrbitingCamera.cpp
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief
     See the header file, really. You don't need to be poking around
   in this file unless you're interested in how an orbiting camera
   works.

   The way we have implemented this class is NOT how you should be
   implementing your Camtrans. This camera is a DIFFERENT TYPE of
   camera which we're providing so you can easily view your Shapes
   and to make sure your scene graph is working if your camera isn't.

   In the Camtrans assignment, you'll be implementing your own
   perspective camera from scratch! This one uses OpenGL.

*/

#include "orbitingcamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "lib/common.h"

OrbitingCamera::OrbitingCamera() : Camera()
{
    m_aspectRatio = 1;
    m_angleX = m_angleY = 0;
    m_zoomZ = -5;
    updateMatrices();
}

OrbitingCamera::~OrbitingCamera()
{

}

void OrbitingCamera::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;

    updateProjectionMatrix();
}

glm::mat4 OrbitingCamera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

glm::mat4 OrbitingCamera::getModelviewMatrix() const
{
    return m_modelviewMatrix;
}

void OrbitingCamera::mouseDown(int x, int y)
{
    m_oldX = x;
    m_oldY = y;
}

void OrbitingCamera::mouseDragged(int x, int y)
{
    m_angleY += x - m_oldX;
    m_angleX -= y - m_oldY;
    m_oldX = x;
    m_oldY = y;
    if (m_angleX < -90) m_angleX = -90;
    if (m_angleX > 90) m_angleX = 90;

    updateModelviewMatrix();
}

void OrbitingCamera::mouseScrolled(int delta)
{
    // Use an exponential factor so the zoom increments are small when we are
    // close to the object and large when we are far away from the object
    float newVal = m_zoomZ *  powf(0.999f, delta);
    if (newVal >= -.001 || newVal <= -100) return;
    m_zoomZ = newVal;

    updateModelviewMatrix();
}

void OrbitingCamera::updateMatrices()
{
    updateProjectionMatrix();
    updateModelviewMatrix();
}

void OrbitingCamera::updateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(75.0f, m_aspectRatio, 0.1f, 10000.0f);
    emit projectionChanged(m_projectionMatrix);
    emit modelviewProjectionChanged(m_projectionMatrix * m_modelviewMatrix);

}

void OrbitingCamera::updateModelviewMatrix()
{
    m_modelviewMatrix = glm::mat4();
    // Move the object forward by m_zoomZ units before we rotate, so it will rotate about a point in front of us
    m_modelviewMatrix = glm::translate(m_modelviewMatrix, glm::vec3(0, 0, m_zoomZ));

    // Now rotate the object, pivoting it about the new origin in front of us
    m_modelviewMatrix = glm::rotate(m_modelviewMatrix, degreesToRadians(180), glm::vec3(1,0,0));
    m_modelviewMatrix = glm::rotate(m_modelviewMatrix, degreesToRadians(m_angleX), glm::vec3(1, 0, 0));
    m_modelviewMatrix = glm::rotate(m_modelviewMatrix, degreesToRadians(m_angleY), glm::vec3(0, 1, 0));

    emit viewChanged(m_modelviewMatrix);
    emit modelviewProjectionChanged(m_projectionMatrix * m_modelviewMatrix);

}

glm::vec4 OrbitingCamera::getPosition() const {
    return glm::vec4(0, 0, 0, 1);
}

void OrbitingCamera::updateMats()
{
    emit viewChanged(m_modelviewMatrix);
    emit projectionChanged(m_projectionMatrix);
    emit modelviewProjectionChanged(m_projectionMatrix * m_modelviewMatrix);
}

glm::mat4 OrbitingCamera::getViewingMatrix() const {
    return glm::mat4();
}

