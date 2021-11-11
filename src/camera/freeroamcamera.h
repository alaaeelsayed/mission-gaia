#pragma once

#include "../../wolf/wolf.h"
#include "camera.h"

class FreeRoamCamera : public Camera
{
public:
    FreeRoamCamera(wolf::App *pApp);
    ~FreeRoamCamera() override;

    void update(float dt) override;
    glm::mat4 getViewMatrix() override;
    glm::mat4 getProjMatrix(int width, int height) override;
    glm::vec3 getViewDirection() const override;
    glm::vec3 getPosition() const override;
    void setPosition(const glm::vec3 &m_position) override;
    void reset();

private:
    glm::vec3 _getCameraUp() override;
    glm::vec3 _getCameraSide() override;
    glm::vec3 _getCameraTarget() const;
    void _rotate(const glm::vec2 &mouseMovement);
    void _processMouseScroll(const glm::vec2 &mouseScroll);
    float m_sensitivity = 0.1f;
    float m_xOffset = 0.0f;
    float m_yOffset = 0.0f;
    glm::vec3 m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
};