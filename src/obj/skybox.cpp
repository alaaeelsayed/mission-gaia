#include "Skybox.h"

Skybox::~Skybox()
{
    delete m_pSkyboxCube;
}

Skybox::Skybox(wolf::Program *m_pProgram, Camera *m_pCamera, std::string filePath)
{
    this->m_pProgram = m_pProgram;
    this->m_pCamera = m_pCamera;
    m_pSkyboxCube = new Cube(m_pProgram, filePath, true);
    m_pSkyboxCube->setScale(12000.0f);
}

void Skybox::update(float dt)
{
    this->m_pCamPosition = m_pCamera->getPosition();
    this->m_pSkyboxCube->setPosition(m_pCamPosition);
}

void Skybox::render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height)
{
    m_pSkyboxCube->render(mProj, mView, width, height);
}
