#pragma once

#include "../../wolf/wolf.h"

class Camera
{
public:
    Camera(wolf::App *pApp) : m_pApp(pApp) {}
    virtual ~Camera(){};

    virtual void update(float dt) = 0;
    virtual glm::mat4 getViewMatrix() = 0;
    virtual glm::mat4 getProjMatrix(int width, int height) = 0;
    virtual glm::vec3 getViewDirection() const = 0;
    virtual glm::vec3 getPosition() const = 0;
    virtual void setPosition(const glm::vec3 &m_position) = 0;
    glm::vec3 getRotation() { return glm::vec3(m_rotX, m_rotY, 0.0f); };

protected:
    virtual glm::vec3 _getCameraUp() = 0;
    virtual glm::vec3 _getCameraSide() = 0;

    float m_speed = 20.0f;
    float m_rotX = -90.0f;
    float m_rotY = 0.0f;
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_target = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float m_near = 0.1f;
    float m_far = 60000.0f;
    float m_fov = 45.0f;

    glm::vec2 m_lastMousePos = glm::vec2(0.0f, 0.0f);
    wolf::App *m_pApp = nullptr;
};