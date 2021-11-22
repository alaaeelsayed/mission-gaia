#pragma once

#include "../../wolf/wolf.h"
#include "./plane.h"
#include "../misc/imgui/imgui.h"
#include "../misc/imgui/imgui_impl_glfw.h"
#include "../misc/imgui/imgui_impl_opengl3.h"

class Water
{
public:
    Water();
    virtual ~Water();
    void update(float dt);
    void render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height);
    void setPosition(const glm::vec3 &vPosition)
    {
        m_vPosition = vPosition;
        m_pSurface->setPosition(vPosition);
    };
    void setScale(const glm::vec3 &vScale)
    {
        m_vScale = vScale;
        m_pSurface->setWidth(vScale.x);
        m_pSurface->setHeight(vScale.y);
        m_pSurface->setDepth(vScale.z);
    }

private:
    void _calculateH0K();
    void _calculateH0T();
    void _generateTwiddleFactors();
    void _butterflyOperation(wolf::Texture *axisTexture, wolf::Texture *dest);
    void _inversionOperation(wolf::Texture *axisTexture, wolf::Texture *dest, int pingpong);
    void _generateIndices();
    unsigned long _reverseBits(unsigned long word, unsigned char maxLength);

    glm::vec3 m_vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);
    wolf::ShaderStorageBuffer *m_pIndices = 0;

    wolf::Program *m_pRenderProgram = 0;

    // Compute Programs
    wolf::Program *m_pAmplitudeProgram = 0;
    wolf::Program *m_pAmplitudeByTimeProgram = 0;
    wolf::Program *m_pTwiddleFactorsProgram = 0;
    wolf::Program *m_pInversionProgram = 0;
    wolf::Program *m_pButterflyProgram = 0;
    bool m_bInitialized = false;

    static const unsigned char gs_bitRevTable[];
    static const unsigned short gs_bitRevMasks[];

    // Phillips Spectrum Variables (Wind)
    int N = 256;
    int L = 1000;
    float A = 2.0f;
    glm::vec3 W = glm::vec3(1.0f, 1.0f, 0.0f);
    float S = 80.0f;
    float suppression = 0.1f;
    glm::vec3 m_vWaterColor = glm::vec3(0.0f, 0.369668034f, 0.839894319f);

    // Textures
    wolf::Texture *m_pH0Texture = nullptr;
    wolf::Texture *m_pH0MinuskTexture = nullptr;
    wolf::Texture *m_pH0Tdy = nullptr;
    wolf::Texture *m_pH0Tdx = nullptr;
    wolf::Texture *m_pH0Tdz = nullptr;
    wolf::Texture *m_pDy = nullptr;
    wolf::Texture *m_pDx = nullptr;
    wolf::Texture *m_pDz = nullptr;
    wolf::Texture *m_pTwiddleFactors = nullptr;
    wolf::Texture *m_pPingPong = nullptr;
    wolf::Texture *m_pNoise00 = nullptr;
    wolf::Texture *m_pNoise01 = nullptr;
    wolf::Texture *m_pNoise02 = nullptr;
    wolf::Texture *m_pNoise03 = nullptr;

    bool m_bUpdate = false;

    Plane *m_pSurface = nullptr;
    int m_iSegments = 32;
};