#pragma once

#include "../wolf/wolf.h"
#include "Camera.h"

class OrbitCamera : public Camera
{
public:
    OrbitCamera(wolf::App *pApp);
    ~OrbitCamera() override;

    void update(float dt) override;
    glm::mat4 getViewMatrix() override;
    glm::mat4 getProjMatrix(int width, int height) override;
    glm::vec3 getViewDirection() const override;
    void setPosition(const glm::vec3 &m_position) override;

    void focusOn(const glm::vec3 &min, const glm::vec3 &max);

private:
    void _rotate(const glm::vec2 &mouseMovement);
    glm::vec3 _getCameraUp() override;
    glm::vec3 _getCameraSide() override;
    void _pan(const glm::vec2 &mouseMovement);
    float _calculateRequiredDistance();
    glm::vec3 m_offset = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_focusMin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_focusMax = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_distance = 0.0f;
};