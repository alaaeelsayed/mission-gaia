#include "gaia.h"

Gaia::~Gaia()
{
    printf("Destroying Gaia\n");
    delete m_pSkybox;
    delete m_pPlane;
    delete m_pFreeRoamCamera;
    wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    wolf::TextureManager::DestroyTexture(m_pTex);
    delete m_pModel;
}

void Gaia::init()
{
    // Only init if not already done
    if (!m_pWorldProgram && !m_pModel)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glEnable(GL_DEPTH_TEST);
        srand((unsigned int)time(NULL));

        m_pTex = wolf::TextureManager::CreateTexture("data/textures/diffuseGray.png");
        m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);

        const std::string MATNAME = "ship";
        m_pMat = wolf::MaterialManager::CreateMaterial(MATNAME);
        m_pMat->SetProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
        m_pMat->SetDepthTest(true);
        m_pMat->SetDepthWrite(true);

        m_pMat->SetUniform("u_lightDir", glm::vec3(0.0f, 0.0f, 1.0f));
        m_pMat->SetUniform("u_lightColor", glm::vec3(1.0f, 1.0f, 0.0f));
        m_pMat->SetUniform("u_ambientLight", glm::vec3(0.3f, 0.1f, 0.1f));
        m_pMat->SetUniform("u_diffuseTex", 0);
        m_pMat->SetUniform("u_specularColor", glm::vec3(1.0f, 1.0f, 1.0f));

        SingleMaterialProvider matProvider(MATNAME);
        m_pModel = new wolf::Model("data/models/bird.fbx", matProvider);

        m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
        m_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
        m_pFreeRoamCamera->setPosition(glm::vec3(100, 1000, 100));
        m_pSkybox = new Skybox(m_pWorldProgram, m_pFreeRoamCamera, m_skyboxPath);

        m_pPlane = new Plane(m_pWorldProgram, m_groundTexPath);

        // reduce scale of whole project
        m_pPlane->setScale(12000.0f);
    }

    printf("Successfully initialized Gaia\n");
}

void Gaia::update(float dt)
{
    m_pFreeRoamCamera->update(dt);
    m_pSkybox->update(dt);
}

void Gaia::render(int width, int height)
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mProj = m_pFreeRoamCamera->getProjMatrix(width, height);
    glm::mat4 mView = m_pFreeRoamCamera->getViewMatrix();
    glm::mat4 mWorld = glm::mat4(1.0f);

    m_pWorldProgram->SetUniform("u_lightDir", m_sunDirection);

    // Render all objects
    m_pSkybox->render(mProj, mView, width, height);
    m_pPlane->render(mProj, mView, width, height);
    m_pModel->Render(mWorld, mView, mProj);
}
