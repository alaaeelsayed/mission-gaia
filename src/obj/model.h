#pragma once
#include "../../wolf/wolf.h"
#include "../camera/camera.h"
#include "../states/stategameplay.h"

class SingleMaterialProvider : public wolf::Model::MaterialProvider
{
public:
    SingleMaterialProvider(const std::string &matName) : m_matName(matName) {}

    wolf::Material *getMaterial(const std::string &nodeName, int subMeshIndex, const std::string &name) const override
    {
        // Regardless of what mesh index or mat name the model wants, we just
        // use the mat we were seeded with. Note that we create a new one each
        // time so the DestroyMaterial calls line up. This could be improved,
        // but they do share shaders.
        return wolf::MaterialManager::CreateMaterial(m_matName);
    }

private:
    std::string m_matName;
};

class Model
{
public:
    Model(const std::string &modelPath, const std::string &matName);
    virtual ~Model();

    void setTexture(const char *texPath);
    void setNormal(const char *normalPath);
    void setPosition(const glm::vec3 &vPosition);
    void translate(const glm::vec3 &vPosition);
    void rotate(float fAngle, const glm::vec3 &vAxis);
    void setRotation(float fAngle, const glm::vec3 &vAxis);
    void setScale(const glm::vec3 &vScale);
    glm::vec3 getScale();
    void setOffset(const glm::vec3 &vOffset);
    wolf::Model *getModel();
    void attachLight(StateGameplay::Light *pLight);
    void update(float fDelta);

    wolf::Material *getMaterial();

    glm::vec3 getPosition();
    void render(const glm::mat4 &mProj, const glm::mat4 &mView, const glm::vec3 &mViewPos = glm::vec3(0.0f, 0.0f, 0.0f));

private:
    glm::vec3 m_vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 m_vRotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // xyz for axis w for angle
    glm::vec3 m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;
    float m_fRotation = 0.0f;

    wolf::Model *m_pModel = 0;
    wolf::Texture *m_pTexture = 0;
    wolf::Texture *m_pNormal = 0;
    wolf::Material *m_pMat = 0;
    int m_iNumLights = 0;
    glm::vec3 m_vOffset = glm::vec3(0.0f, 0.0f, 0.0f);

    StateGameplay::Light *m_pLight = nullptr;
};