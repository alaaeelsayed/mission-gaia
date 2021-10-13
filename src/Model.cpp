#include "Model.h"

Model::~Model()
{
    delete m_pModel;
    wolf::MaterialManager::DestroyMaterial(m_pMat);
}

Model::Model(const char *modelPath, const char *modelName)
{

    const std::string MATNAME = modelName;
    m_pMat = wolf::MaterialManager::CreateMaterial(MATNAME);
    m_pMat->SetProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthWrite(true);

    m_pMat->SetUniform("u_lightDir", glm::vec3(0.0f, 0.0f, 1.0f));
    m_pMat->SetUniform("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_pMat->SetUniform("u_ambientLight", glm::vec3(0.5f, 0.5f, 0.5f));
    m_pMat->SetUniform("u_diffuseTex", 0);
    m_pMat->SetUniform("u_specularColor", glm::vec3(1.0f, 1.0f, 1.0f));

    SingleMaterialProvider matProvider(MATNAME);
    m_pModel = new wolf::Model(modelPath, matProvider);
}

void Model::setTexture(const char *texPath)
{
    m_pTexture = wolf::TextureManager::CreateTexture(texPath);
}

void Model::setPosition(const glm::mat4 &mPosition)
{
    m_mPosition = mPosition;
}

void Model::setOffset(const glm::vec3 &vOffset)
{
    m_vOffset = vOffset;
}

wolf::Model *Model::getModel()
{
    return m_pModel;
}

void Model::translate(const glm::vec3 &vPosition)
{
    if (m_vOffset == glm::vec3(0.0f, 0.0f, 0.0f))
        m_mPosition = glm::translate(m_mPosition, vPosition);
    else
        m_mPosition = glm::translate(m_mPosition, vPosition * -m_vOffset);
}

void Model::rotate(float fAngle)
{
    m_mPosition = glm::rotate(m_mPosition, glm::radians(fAngle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::setScale(const glm::vec3 &vScale)
{
    m_mPosition = glm::scale(m_mPosition, vScale);
    m_mPosition = glm::translate(m_mPosition, -m_vOffset);
}

void Model::render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    if (m_pTexture)
        m_pTexture->Bind();
    m_pModel->Render(m_mPosition, mView, mProj);
}
