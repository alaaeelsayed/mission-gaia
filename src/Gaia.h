#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/FreeRoamCamera.h"
#include "../samplefw/Skybox.h"
#include "../samplefw/plane.h"
#include "./util/singleMaterialProvider.h"

class Gaia : public Sample
{
public:
    Gaia(wolf::App *pApp) : Sample(pApp, "Generating Gaia") {}
    ~Gaia();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    const std::string m_skyboxPath = "data/textures/skybox/skybox.png";
    const std::string m_groundTexPath = "data/textures/ground/ground.png";

    wolf::Program *m_pWorldProgram = 0;
    wolf::Model *m_pModel = nullptr;
    wolf::Material *m_pMat = nullptr;
    wolf::Texture *m_pTex = nullptr;

    const int m_gridSize = 20;

    float m_currShininess = 200.0f;

    float m_sunSpeed = 0.01f;
    GLfloat m_sunAngle = -100;
    glm::vec3 m_sunDirection = glm::vec3(6000.0f, 6000.0f, 500.0f);

    Skybox *m_pSkybox = 0;
    FreeRoamCamera *m_pFreeRoamCamera = 0;
    Plane *m_pPlane = 0;
};