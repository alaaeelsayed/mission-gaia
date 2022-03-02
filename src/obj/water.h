#pragma once

#include "../../wolf/wolf.h"
#include "./plane.h"
#include "../camera/camera.h"
#include "../misc/imgui/imgui.h"
#include "../misc/imgui/imgui_impl_glfw.h"
#include "../misc/imgui/imgui_impl_opengl3.h"
#include "../particles/effect.h"
#include "../scene/scene.h"
#include "node.h"

class Water : public Node
{
public:
    Water();
    virtual ~Water();
    void Update(float dt) override;
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView) override;
    virtual void SetScale(const glm::vec3 &scale);
    virtual void SetPos(const glm::vec3 &position);

private:
    void _calculateH0K();
    void _calculateH0T();
    void _generateTwiddleFactors();
    void _butterflyOperation(wolf::Texture *axisTexture, wolf::Texture *dest);
    void _inversionOperation(wolf::Texture *axisTexture, wolf::Texture *dest, int pingpong);
    void _generateIndices();
    unsigned long _reverseBits(unsigned long word, unsigned char maxLength);

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
    int N = 512;
    int L = 756;
    float A = 57.7f;
    glm::vec3 W = glm::vec3(0.54f, 0.84f, 0.0f);
    float S = 95.0f;
    float suppression = 0.1f;
    float choppiness = 1.7f;
    glm::vec3 m_vWaterColor = glm::vec3(0.019f, 0.537f, 0.854f);

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

    // Rain Effect for cloud configuration (fun)
    Effect *m_pEffect = nullptr;
    std::string m_sRainPath = "data/effects/rain.pfx";
    bool m_bRenderRain = false;

    Plane *m_pSurface = nullptr;
    int m_iSegments = 32;

    // Debug Menu
    bool m_bUpdate = false;
    bool m_bStop = false;
    bool m_bIsChoppy = true;
    bool m_bInvertColors = false;
    std::string m_sStopResume = "Stop Water";
};