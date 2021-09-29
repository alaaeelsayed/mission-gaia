#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/FreeRoamCamera.h"
#include "../samplefw/Skybox.h"
#include "../samplefw/plane.h"
#include "./singleMaterialProvider.h"

class Gaia : public Sample
{
public:
    Gaia(wolf::App *pApp) : Sample(pApp, "Generating Gaia") {}
    ~Gaia();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    int _randomNum(int lowerBound, int upperBound);

    const std::string m_skyboxPath = "data/textures/skybox/skybox.png";
    const std::string m_groundTexPath = "data/textures/ground/ground.png";

    wolf::Program *m_pWorldProgram = 0;
    std::vector<wolf::Model *> m_lModels;
    std::vector<glm::vec3> m_lPositions;
    wolf::Material *m_pMat = nullptr;
    wolf::Texture *m_pTex = nullptr;

    const int m_gridSize = 20;

    float m_currShininess = 200.0f;

    float m_sunSpeed = 0.01f;
    GLfloat m_sunAngle = -60;
    glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 20.0f);

    Skybox *m_pSkybox = 0;
    FreeRoamCamera *m_pFreeRoamCamera = 0;
    Plane *m_pPlane = 0;
};