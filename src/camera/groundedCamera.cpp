#include "groundedcamera.h"

GroundedCamera::GroundedCamera(wolf::App *pApp)
    : Camera(pApp)
{
    m_lastMousePos = m_pApp->getMousePos();
    m_pApp->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GroundedCamera::~GroundedCamera()
{
}

void GroundedCamera::Update(float dt)
{
    glm::vec2 mousePos = m_pApp->getMousePos();

    glm::vec2 mouseMovement = mousePos - m_lastMousePos;
    _rotate(mouseMovement);

    glm::vec2 mouseScroll = m_pApp->getMouseScroll();

    if (mouseScroll.y > 0)
    {
        m_fov -= (m_fov / 5.0f);
        m_fov = wolf::max(m_fov, 1.0f);
    }
    else if (mouseScroll.y < 0)
    {
        m_fov += (m_fov / 5.0f);
        m_fov = wolf::min(m_fov, 45.0f);
    }

    // LControl
    float cameraSpeed = m_pApp->isKeyDown(341) * 0.5f + 0.3f;

    if (m_pApp->isKeyDown('w'))
    {
        m_position += cameraSpeed * glm::vec3(m_eye.x, 0.0f, m_eye.z);
    }
    if (m_pApp->isKeyDown('s'))
    {

        m_position -= cameraSpeed * glm::vec3(m_eye.x, 0.0f, m_eye.z);
    }
    if (m_pApp->isKeyDown('a'))
    {
        m_position -= _getCameraSide() * cameraSpeed;
    }
    if (m_pApp->isKeyDown('d'))
    {
        m_position += _getCameraSide() * cameraSpeed;
    }

    m_lastMousePos = mousePos;
}

glm::mat4 GroundedCamera::GetViewMatrix()
{
    return glm::lookAt(m_position, _getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 GroundedCamera::GetProjMatrix(int width, int height)
{
    return glm::perspective(glm::radians(m_fov), (float)width / (float)height, m_near, m_far);
}

glm::vec3 GroundedCamera::_getCameraTarget() const
{
    return m_position + m_eye;
}

void GroundedCamera::_rotate(const glm::vec2 &mouseMovement)
{
    m_rotX += mouseMovement.x * 0.1f;
    m_rotY -= mouseMovement.y * 0.1f;
    m_rotY = wolf::max(wolf::min(89.0f, m_rotY), -89.0f);
    m_eye = glm::normalize(glm::vec3(cos(glm::radians(m_rotX)) * cos(glm::radians(m_rotY)), sin(glm::radians(m_rotY)), sin(glm::radians(m_rotX)) * cos(glm::radians(m_rotY))));
}

void GroundedCamera::Reset()
{
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_eye = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 GroundedCamera::_getCameraSide()
{
    glm::vec3 side = glm::cross(m_eye, glm::vec3(0.0f, 1.0f, 0.0f));
    return glm::normalize(side);
}

glm::vec3 GroundedCamera::_getCameraUp()
{
    return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 GroundedCamera::GetViewDirection() const
{
    return _getCameraTarget();
}

glm::vec3 GroundedCamera::GetPosition() const
{
    return this->m_position;
}

void GroundedCamera::SetPosition(const glm::vec3 &m_position)
{
    this->m_position = m_position;
}