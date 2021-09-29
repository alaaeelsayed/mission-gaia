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
    for (wolf::Model *pModel : m_lModels)
    {
        delete pModel;
    }
}

void Gaia::init()
{
    // Only init if not already done
    if (!m_pWorldProgram)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glEnable(GL_DEPTH_TEST);
        srand((unsigned int)time(NULL));

        m_pTex = wolf::TextureManager::CreateTexture("data/textures/gimpy_diffuse.tga");
        // m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);

        const std::string MATNAME = "spitter";
        m_pMat = wolf::MaterialManager::CreateMaterial(MATNAME);
        m_pMat->SetProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
        m_pMat->SetDepthTest(true);
        m_pMat->SetDepthWrite(true);

        m_pMat->SetUniform("u_lightDir", glm::vec3(0.0f, 0.0f, 1.0f));
        m_pMat->SetUniform("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        m_pMat->SetUniform("u_ambientLight", glm::vec3(0.3f, 0.1f, 0.1f));
        m_pMat->SetUniform("u_diffuseTex", 0);
        m_pMat->SetUniform("u_specularColor", glm::vec3(1.0f, 1.0f, 1.0f));

        SingleMaterialProvider matProvider(MATNAME);

        for (int i = 0; i < 5; i++)
        {
            m_lModels.push_back(new wolf::Model("data/models/low_poly_spitter.fbx", matProvider));
            m_lModels.push_back(new wolf::Model("data/models/cyborg_weapon.fbx", matProvider));

            int x = rand() % 20 - 10;
            int z = rand() % 20 - 10;

            m_lPositions.push_back(glm::vec3(x, 0.0f, z));
            x = rand() % 20 - 10;
            z = rand() % 20 - 10;
            m_lPositions.push_back(glm::vec3(x, 0.0f, z));
        }
        m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
        m_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
        m_pFreeRoamCamera->setPosition(glm::vec3(10, 5, 10));
        m_pSkybox = new Skybox(m_pWorldProgram, m_pFreeRoamCamera, m_skyboxPath);

        m_pPlane = new Plane(m_pWorldProgram, m_groundTexPath);

        m_pPlane->setScale(30.0f);
        m_pPlane->setY(-1.5f);
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
    m_pTex->Bind();

    m_pSkybox->render(mProj, mView, width, height);
    m_pPlane->render(mProj, mView, width, height);

    int x = 0, z = 0;
    int i = 0;
    for (wolf::Model *pModel : m_lModels)
    {
        mWorld = glm::mat4(1.0f);
        mWorld = glm::translate(mWorld, m_lPositions[i]);
        pModel->Render(mWorld, mView, mProj);

        i++;
    }
}

int Gaia::_randomNum(int lowerBound, int upperBound)
{
    // Generates random number in range. Includes lower bound and upper bound
    return rand() % (upperBound - lowerBound + 1) + lowerBound;
}