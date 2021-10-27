#pragma once
#include "../wolf/wolf.h"
#include "./singleMaterialProvider.h"
#include "../../samplefw/Camera.h"

class Model
{
public:
    Model(const char *modelPath, const char *modelName);
    virtual ~Model();

    void setTexture(const char *texPath);
    void setNormal(const char *normalPath);
    void setPosition(const glm::mat4 &mPosition);
    void translate(const glm::vec3 &vPosition);
    void rotate(float fAngle);
    void setScale(const glm::vec3 &vScale);
    void setOffset(const glm::vec3 &vOffset);
    wolf::Model *getModel();

    void render(const glm::mat4 &mProj, const glm::mat4 &mView, const glm::vec3 &mViewPos = glm::vec3(0.0f, 0.0f, 0.0f));

private:
    glm::mat4 m_mPosition = glm::mat4(1.0f);
    wolf::Model *m_pModel = 0;
    wolf::Texture *m_pTexture = 0;
    wolf::Texture *m_pNormal = 0;
    wolf::Material *m_pMat = 0;
    glm::vec3 m_vOffset = glm::vec3(0.0f, 0.0f, 0.0f);
};