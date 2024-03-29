#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(wolf::App *pApp)
    : Camera(pApp)
{
    m_lastMousePos = m_pApp->getMousePos();
    m_pApp->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::Update(float dt)
{
    // glm::vec2 mousePos = m_pApp->getMousePos();

    // glm::vec2 mouseMovement = mousePos - m_lastMousePos;
    // Rotate(mouseMovement);

    // // LControl
    // float cameraSpeed = m_pApp->isKeyDown(341) * 2.0f + 0.3f;

    // if (m_pApp->isKeyDown('w'))
    // {
    //     m_position += cameraSpeed * m_eye;
    // }
    // if (m_pApp->isKeyDown('s'))
    // {
    //     m_position -= cameraSpeed * m_eye;
    // }
    // if (m_pApp->isKeyDown('a'))
    // {
    //     m_position -= _getCameraSide() * cameraSpeed;
    // }
    // if (m_pApp->isKeyDown('d'))
    // {
    //     m_position += _getCameraSide() * cameraSpeed;
    // }
    // // LShift
    // if (m_pApp->isKeyDown(340))
    // {
    //     m_position -= _getCameraUp() * cameraSpeed;
    // }
    // m_lastMousePos = mousePos;
}

glm::mat4 OrthoCamera::GetViewMatrix()
{
    return glm::lookAt(m_position, _getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 OrthoCamera::GetProjMatrix(int width, int height)
{
    return glm::perspective(glm::radians(m_fov), (float)width / (float)height, m_near, m_far);
}

void OrthoCamera::Reset()
{
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_eye = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 OrthoCamera::GetViewDirection() const
{
    return m_eye;
}

glm::vec3 OrthoCamera::GetPosition() const
{
    return this->m_position;
}

void OrthoCamera::SetPosition(const glm::vec3 &m_position)
{
    this->m_position = m_position;
}

glm::vec3 OrthoCamera::_getCameraTarget() const
{
    return m_position + m_eye;
}

void OrthoCamera::SetRotation(const glm::vec2 &rotation)
{
    m_rotX = rotation.x;
    m_rotY = rotation.y;
    m_rotY = wolf::max(wolf::min(89.0f, m_rotY), -89.0f);

    m_eye = glm::normalize(glm::vec3(cos(glm::radians(m_rotX)) * cos(glm::radians(m_rotY)), sin(glm::radians(m_rotY)), sin(glm::radians(m_rotX)) * cos(glm::radians(m_rotY))));
}

glm::vec3 OrthoCamera::_getCameraSide()
{
    glm::vec3 side = glm::cross(m_eye, glm::vec3(0.0f, 1.0f, 0.0f));
    return glm::normalize(side);
}

glm::vec3 OrthoCamera::_getCameraUp()
{
    return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
}