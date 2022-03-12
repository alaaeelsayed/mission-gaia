#pragma once

#include "../../wolf/wolf.h"
#include "camera.h"

class OrthoCamera : public Camera
{
public:
    OrthoCamera(wolf::App *pApp);
    ~OrthoCamera() override;

    void Update(float dt) override;
    glm::mat4 GetViewMatrix() override;
    glm::mat4 GetProjMatrix(int width, int height) override;
    glm::vec3 GetViewDirection() const override;
    glm::vec3 GetPosition() const override;
    void SetPosition(const glm::vec3 &m_position) override;
    void Reset();
    void SetRotation(const glm::vec2 &rotation);

private:
    glm::vec3 _getCameraUp() override;
    glm::vec3 _getCameraSide() override;
    glm::vec3 _getCameraTarget() const;
    void _processMouseScroll(const glm::vec2 &mouseScroll);
    float m_sensitivity = 0.1f;
    float m_xOffset = 0.0f;
    float m_yOffset = 0.0f;
    glm::vec3 m_eye = glm::vec3(0.0f, 0.0f, -1.0f);
};