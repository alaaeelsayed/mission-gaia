#pragma once

#include "../../wolf/wolf.h"
#include "./plane.h"

class Water
{
public:
    Water();
    virtual ~Water();
    void update(float dt);
    void render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height);
    void setPosition(const glm::vec3 &vPosition) { m_vPosition = vPosition; };
    void setScale(const glm::vec3 &vScale) { m_vScale = vScale; }

private:
    void _calculateH0K();
    void _calculateH0T(float dt);
    void _generateTwiddleFactors();
    void _butterflyOperation(wolf::Texture *axisTexture, wolf::Texture *dest);
    void _inversionOperation(wolf::Texture *axisTexture, wolf::Texture *dest, int pingpong);
    void _generateIndices();
    unsigned long _reverseBits(unsigned long word, unsigned char maxLength);

    glm::vec3 m_vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);

    wolf::VertexBuffer *m_pVB = 0;
    wolf::VertexDeclaration *m_pDecl = 0;
    wolf::ShaderStorageBuffer *m_pIndices = 0;

    wolf::Program *m_pRenderProgram = 0;

    // Compute Programs
    wolf::Program *m_pAmplitudeProgram = 0;
    wolf::Program *m_pAmplitudeByTimeProgram = 0;
    wolf::Program *m_pTwiddleFactorsProgram = 0;
    wolf::Program *m_pInversionProgram = 0;
    wolf::Program *m_pButterflyProgram = 0;
    bool m_bInitialized = false;

    static const Vertex gs_surfaceVertices[6];
    static const unsigned char gs_bitRevTable[];
    static const unsigned short gs_bitRevMasks[];

    // Phillips Spectrum Variables (Wind)
    int N = 256;
    int L = 1000;
    float A = 4.0f;
    glm::vec3 W = glm::vec3(1.0f, 1.0f, 0.0f);
    float S = 40.0f;

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
};