#pragma once
#include "../../wolf/wolf.h"
#include "../../tinyxml2/tinyxml2.h"
#include "../misc/util.h"
#include <mutex>

enum EmissionMode
{
    BURST,
    CONTINUOUS
};
enum EmissionType
{
    POINT_EMITTER,
    BOX_EMITTER,
    SPHERE_EMITTER
};

class Affector;
class Emitter
{
public:
    Emitter(const std::string &p_sFilePath);
    ~Emitter();

    struct Particle
    {
        bool m_bInitialized;

        Particle *m_pPrev;
        Particle *m_pNext;

        glm::mat3 m_mWorldMatrix;
        glm::vec3 m_vPosition;
        glm::vec4 m_vColor;
        glm::vec3 m_vVelocity;
        float m_fSize;
        float m_fLifetime;
        float m_fFade;

        Emitter *m_pEmitter;
        std::vector<Affector *> m_vAffectors;

        Particle()
            : m_bInitialized(false),
              m_pPrev(nullptr),
              m_pNext(nullptr),
              m_mWorldMatrix(glm::mat4(1.0f)),
              m_vPosition(glm::vec3(0.0f)),
              m_vColor(glm::vec4(0.0f)),
              m_vVelocity(glm::vec3(0.0f)),
              m_fSize(0.0f),
              m_fLifetime(1.0f),
              m_fFade(1.0f),
              m_pEmitter(nullptr)
        {
        }
    };

    struct Vertex
    {
        GLfloat x, y, z;    // Position
        GLfloat r, g, b, a; // Color
        GLfloat s;          // Size
    };

    glm::mat4 getTransform() const;
    glm::vec3 getOffset() const;
    void setTransform(const glm::mat4 &p_mTransform);
    void setOffset(const glm::vec3 &p_vOffset);
    void update(float p_fDeltaTime);
    void render(const glm::mat4 &p_mProj, const glm::mat4 &p_mView);

    void setRadius(float pRadius);

    void killParticle(Particle *p_pParticle);

private:
    void _calcBurstTime();
    void _addToActive(Particle *p_pParticle);
    void _addToFree(Particle *p_pParticle);
    Particle *_getFreeParticle();
    void _spawnParticle();
    void _removeFromActive(Particle *p_pParticle);
    void _deleteLinkedList(Particle *p_pHead);

    glm::mat4 m_mTransform = glm::mat4(1.0f);
    glm::vec3 m_vPosition = glm::vec3(0.0f);
    glm::vec3 m_vOffset = glm::vec3(0.0f, 0.0f, 0.0f);

    // Particle Linked List
    Particle *m_pFreeList = nullptr;
    Particle *m_pActiveHead = nullptr;
    Particle *m_pActiveTail = nullptr;

    std::string m_sName = "";
    int m_iNumOfParticles = 0;

    // Lifetime of Emitter
    float m_fDuration = 0.0f;
    float m_fEmitterLifetime = 0.0f;

    // Emission Mode
    EmissionMode m_eMode = CONTINUOUS;

    // Burst Mode
    bool m_bRandomBurstTime = false;
    bool m_bRandomBurstSpawns = false;
    bool m_bRepeatBurst = false;
    int m_iBurstSpawns = 0;
    int m_iBurstSpawnsMin = 0;
    int m_iBurstSpawnsMax = 0;
    float m_fBurstTime = 0.0f;
    float m_fBurstTimeMin = 0.0f;
    float m_fBurstTimeMax = 0.0f;
    float m_fTimeToBurst = 0.0f;

    // Emission Type
    EmissionType m_eType = POINT_EMITTER;
    glm::vec3 m_vBoxEmitterMin = glm::vec3(1.0f);
    glm::vec3 m_vBoxEmitterMax = glm::vec3(1.0f);
    float m_fSphereRadius = 1.0f;

    // Delimeter
    char const m_cDelimeter = ',';

    // Affectors
    std::vector<Affector *> m_vAffectors;

    //- - - - - - - - - Properties - - - - - - - - -//

    // Birth Rate
    bool m_bRandomBirthRate = false;
    int m_iBirthRate = 0;
    int m_iBirthRateMin = 0;
    int m_iBirthRateMax = 0;
    float m_fToSpawn = 0.0f;

    // Velocity
    bool m_bRandomVelocity = false;
    glm::vec3 m_vVelocity = glm::vec3(0.0f);
    glm::vec3 m_vVelocityMin = glm::vec3(0.0f);
    glm::vec3 m_vVelocityMax = glm::vec3(0.0f);

    // Color
    bool m_bRandomColor = false;
    glm::vec4 m_vColor = glm::vec4(0.0f);
    glm::vec4 m_vColorMin = glm::vec4(0.0f);
    glm::vec4 m_vColorMax = glm::vec4(0.0f);

    // Size
    bool m_bRandomSize = false;
    float m_fSize = 0.0f;
    float m_fSizeMin = 0.0f;
    float m_fSizeMax = 0.0f;

    // Lifetime
    bool m_bRandomLifetime = false;
    float m_fLifetime = 0.0f;
    float m_fLifetimeMin = 0.0f;
    float m_fLifetimeMax = 0.0f;

    // Fade
    bool m_bRandomFade = false;
    float m_fFade = 1.0f;
    float m_fFadeMin = 0.0f;
    float m_fFadeMax = 0.0f;

    // - - - - - - - wolf - - - - - - - //
    wolf::Material *m_pMaterial = nullptr;
    wolf::VertexBuffer *m_pVertexBuffer = nullptr;
    wolf::VertexDeclaration *m_pDeclaration = nullptr;
    std::vector<Vertex> m_vVerts;
    std::mutex m_mutex;
};