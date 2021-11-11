#pragma once

#include "../../wolf/wolf.h"
#include "../camera/camera.h"
#include "cube.h"

class Skybox
{
public:
    Skybox(wolf::Program *m_pProgram, Camera *m_pCamera, std::string filePath);
    virtual ~Skybox();
    void update(float dt);
    void render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height);

private:
    glm::vec3 m_pCamPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    wolf::Program *m_pProgram = 0;
    Cube *m_pSkyboxCube = 0;
    Camera *m_pCamera = 0;
};