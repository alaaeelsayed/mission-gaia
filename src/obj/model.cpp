#include "Model.h"

Model::~Model()
{
    delete m_pModel;
    delete m_pLight;
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pRigidBody;
}

Model::Model(const std::string &modelPath, const std::string &matName)
{

    const std::string MATNAME = modelPath + matName;
    m_pMat = wolf::MaterialManager::CreateMaterial(MATNAME);
    m_pMat->SetProgram("data/shaders/" + matName + ".vsh", "data/shaders/" + matName + ".fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthWrite(true);

    m_pMat->SetUniform("u_lightDir", glm::vec3(10.0f, 500.0f, 500.0f));
    m_pMat->SetUniform("u_ambientLight", glm::vec3(0.5f, 0.5f, 0.5f));
    m_pMat->SetUniform("u_specularColor", glm::vec3(0.3f, 0.3f, 0.3f));
    m_pMat->SetUniform("u_shininess", 0.4f);

    SingleMaterialProvider matProvider(MATNAME);
    m_pModel = new wolf::Model(modelPath, matProvider);
}

void Model::attachRigidBody(const std::string &p_sConfiguration)
{
    m_pRigidBody = new RigidBody(p_sConfiguration);
}

void Model::attachLight(StateGameplay::Light *pLight)
{
    m_pLight = pLight;
}

void Model::setTexture(const char *texPath)
{
    m_pTexture = wolf::TextureManager::CreateTexture(texPath);
    m_pTexture->SetFilterMode(wolf::Texture::FM_LinearMipmap, wolf::Texture::FM_Linear);
    m_pTexture->SetWrapMode(wolf::Texture::WM_Repeat);
    m_pMat->SetTexture("u_diffuseTex", m_pTexture);
}

wolf::Material *Model::getMaterial()
{
    return m_pMat;
}

void Model::setNormal(const char *texPath)
{
    m_pNormal = wolf::TextureManager::CreateTexture(texPath);
    m_pNormal->SetFilterMode(wolf::Texture::FM_LinearMipmap, wolf::Texture::FM_Linear);
    m_pNormal->SetWrapMode(wolf::Texture::WM_Repeat);
    m_pMat->SetTexture("u_normalMap", m_pNormal);
}

void Model::setPosition(const glm::vec3 &vPosition)
{
    m_vPosition = vPosition;
}

glm::vec3 Model::getPosition()
{
    return m_vPosition;
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
    m_vPosition += vPosition;
}

void Model::setRotation(const glm::vec3 &vRotation)
{
    m_vRotation = vRotation;
}

void Model::rotate(const glm::vec3 &vRotation)
{
    m_vRotation += vRotation;
}

void Model::setScale(const glm::vec3 &vScale)
{
    m_vScale = vScale;
}

glm::vec3 Model::getScale()
{
    return m_vScale;
}

void Model::update(float fDelta)
{
    if (m_pRigidBody)
    {
        m_pRigidBody->Update(fDelta, m_vPosition);
    }

    if (m_pLight)
    {
        m_pLight->posRange = glm::vec4(m_vPosition.x, m_vPosition.y, m_vPosition.z, m_pLight->posRange.w);
    }
}

void Model::render(const glm::mat4 &mProj, const glm::mat4 &mView, const glm::vec3 &mViewPos)
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), m_vPosition - m_vOffset) *
                       glm::mat4_cast(glm::quat(m_vRotation)) * glm::scale(m_vScale);

    m_pMat->SetUniform("u_viewPos", mViewPos);
    m_pModel->Render(mWorld, mView, mProj);
}
