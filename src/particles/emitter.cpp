#include "emitter.h"
#include "affectors.h"

using namespace tinyxml2;
using namespace std;

Emitter::Emitter(const string &p_sFilePath)
{

    m_pMaterial = wolf::MaterialManager::CreateMaterial("ParticleSystemMat");
    m_pMaterial->SetProgram("data/shaders/particle.vsh", "data/shaders/particle.fsh");
    m_pMaterial->SetBlend(true);
    m_pMaterial->SetBlendMode(wolf::BM_SrcAlpha, wolf::BM_OneMinusSrcAlpha);

    XMLDocument doc = new XMLDocument();
    if (doc.LoadFile(p_sFilePath.c_str()) != XML_SUCCESS)
        throw XML_ERROR_FILE_COULD_NOT_BE_OPENED;

    XMLElement *emitter = doc.FirstChildElement("emitter");
    if (emitter == nullptr)
        throw XML_ERROR_FILE_READ_ERROR;

    m_sName = emitter->Attribute("name");
    emitter->QueryIntAttribute("num_particles", &m_iNumOfParticles);
    emitter->QueryFloatAttribute("duration", &m_fDuration);

    string sMode = emitter->Attribute("mode");
    string sType = emitter->Attribute("type");

    // Emitter Mode and Type
    m_eMode = sMode == "burst" ? BURST : CONTINUOUS;

    // Burst Mode
    if (m_eMode == BURST)
    {
        if (emitter->Attribute("burst_spawns"))
        {
            emitter->QueryIntAttribute("burst_spawns", &m_iBurstSpawns);
        }
        else
        {
            m_bRandomBurstSpawns = true;
            emitter->QueryIntAttribute("burst_spawns_min", &m_iBurstSpawnsMin);
            emitter->QueryIntAttribute("burst_spawns_max", &m_iBurstSpawnsMax);
        }

        if (emitter->Attribute("burst_time"))
        {
            emitter->QueryFloatAttribute("burst_time", &m_fBurstTime);
        }
        else
        {
            m_bRandomBurstTime = true;
            emitter->QueryFloatAttribute("burst_time_min", &m_fBurstTimeMin);
            emitter->QueryFloatAttribute("burst_time_max", &m_fBurstTimeMax);
        }

        _calcBurstTime();
    }

    m_eType = sType == "box_emitter" ? BOX_EMITTER : sType == "sphere_emitter" ? SPHERE_EMITTER
                                                                               : POINT_EMITTER;

    if (m_eType == BOX_EMITTER)
    {
        vector<string> min = Util::splitString(emitter->Attribute("box_emitter_min"), m_cDelimeter);
        vector<string> max = Util::splitString(emitter->Attribute("box_emitter_max"), m_cDelimeter);
        m_vBoxEmitterMin = glm::vec3(stof(min[0]), stof(min[1]), stof(min[2]));
        m_vBoxEmitterMax = glm::vec3(stof(max[0]), stof(max[1]), stof(max[2]));
    }
    else if (m_eType == SPHERE_EMITTER)
    {
        emitter->QueryFloatAttribute("sphere_radius", &m_fSphereRadius);
    }

    // Birth Rate
    if (emitter->QueryIntAttribute("birthrate_min", &m_iBirthRateMin) == XML_SUCCESS)
    {
        m_bRandomBirthRate = true;
        emitter->QueryIntAttribute("birthrate_max", &m_iBirthRateMax);
    }
    else
    {
        emitter->QueryIntAttribute("birthrate", &m_iBirthRate);
    }

    // Spawn Properties
    for (XMLElement *element = emitter->FirstChildElement("spawn_property"); element != NULL; element = element->NextSiblingElement("spawn_property"))
    {
        string name = element->Attribute("name");
        string type = element->Attribute("type");
        vector<string> vValue, vMin, vMax;
        bool bRandom = false;

        if (type == "constant")
        {
            vValue = Util::splitString(element->Attribute("value"), m_cDelimeter);
        }
        else if (type == "random")
        {
            bRandom = true;

            vMin = Util::splitString(element->Attribute("min"), m_cDelimeter);
            vMax = Util::splitString(element->Attribute("max"), m_cDelimeter);
        }

        // Velocity
        if (name == "velocity")
        {
            if (bRandom)
            {
                m_bRandomVelocity = true;
                m_vVelocityMin = glm::vec3(stof(vMin[0]), stof(vMin[1]), stof(vMin[2]));
                m_vVelocityMax = glm::vec3(stof(vMax[0]), stof(vMax[1]), stof(vMax[2]));
            }
            else
                m_vVelocity = glm::vec3(stof(vValue[0]), stof(vValue[1]), stof(vValue[2]));
        }

        // Color
        else if (name == "color")
        {
            if (bRandom)
            {
                m_bRandomColor = true;
                m_vColorMin = glm::vec4(stof(vMin[0]), stof(vMin[1]), stof(vMin[2]), stof(vMin[3]));
                m_vColorMax = glm::vec4(stof(vMax[0]), stof(vMax[1]), stof(vMax[2]), stof(vMax[3]));
            }
            else
                m_vColor = glm::vec4(stof(vValue[0]), stof(vValue[1]), stof(vValue[2]), stof(vValue[3]));
        }

        // Size
        else if (name == "size")
        {
            if (bRandom)
            {
                m_bRandomSize = true;
                m_fSizeMin = stof(vMin[0]);
                m_fSizeMax = stof(vMax[0]);
            }
            else
                m_fSize = stof(vValue[0]);
        }

        // Lifetime
        else if (name == "lifetime")
        {
            if (bRandom)
            {
                m_bRandomLifetime = true;
                m_fLifetimeMin = stof(vMin[0]);
                m_fLifetimeMax = stof(vMax[0]);
            }
            else
                m_fLifetime = stof(vValue[0]);
        }

        // Fade
        else if (name == "fade")
        {
            if (bRandom)
            {
                m_bRandomFade = true;
                m_fFadeMin = stof(vMin[0]);
                m_fFadeMax = stof(vMax[0]);
            }
            else
                m_fFade = stof(vValue[0]);
        }
    }

    // Affectors

    for (XMLElement *affector = emitter->FirstChildElement("affector"); affector != NULL; affector = affector->NextSiblingElement("affector"))
    {
        string type = affector->Attribute("type");

        // Scale
        std::string sMode;
        int iStart, iEnd;

        for (XMLElement *property = affector->FirstChildElement("property"); property != NULL; property = property->NextSiblingElement("property"))
        {
            std::string sName = property->Attribute("name");

            // Scale Affector
            if (type == "scale")
            {
                if (sName == "mode")
                    sMode = property->Attribute("value");
                else if (sName == "start")
                    property->QueryIntAttribute("value", &iStart);
                else if (sName == "end")
                    property->QueryIntAttribute("value", &iEnd);
            }
        }

        if (type == "scale")
            m_vAffectors.push_back(new ScaleAffector((float)iStart, (float)iEnd, sMode));
        else if (type == "add_velocity")
            m_vAffectors.push_back(new AddVelAffector());
        else if (type == "fade")
            m_vAffectors.push_back(new FadeAffector());
        else if (type == "gravity")
            m_vAffectors.push_back(new GravityAffector());
    }

    // Create Particles
    for (int i = 0; i < m_iNumOfParticles; i++)
    {
        Particle *pParticle = new Particle();
        pParticle->m_pEmitter = this;
        pParticle->m_vAffectors = m_vAffectors;
        _addToFree(pParticle);
    }
}

Emitter::~Emitter()
{
    wolf::MaterialManager::DestroyMaterial(m_pMaterial);
    wolf::BufferManager::DestroyBuffer(m_pVertexBuffer);
    delete m_pDeclaration;

    // Delete Particles
    _deleteLinkedList(m_pFreeList);
    _deleteLinkedList(m_pActiveHead);
}

glm::mat4 Emitter::getTransform() const
{
    return m_mTransform;
}

glm::vec3 Emitter::getOffset() const
{
    return m_vOffset;
}

void Emitter::setTransform(const glm::mat4 &p_mTransform)
{
    m_mTransform = glm::translate(p_mTransform, m_vOffset);
    m_vPosition = m_mTransform[3]; // Position vector
}

void Emitter::setOffset(const glm::vec3 &p_vOffset)
{
    // Reset transform before setting offset
    m_mTransform = glm::translate(m_mTransform, -m_vOffset);
    m_vOffset = p_vOffset;
    m_mTransform = glm::translate(m_mTransform, m_vOffset);
    m_vPosition = m_mTransform[3]; // Position vector
}

void Emitter::update(float p_fDeltaTime)
{
    m_fEmitterLifetime += p_fDeltaTime;

    // Keep updating until emitter expires
    if (m_fDuration < 0 || m_fEmitterLifetime < m_fDuration)
    {

        int iNumSpawns = 0;
        if (m_eMode == CONTINUOUS)
        {
            int iBirthRate;
            if (m_bRandomBirthRate)
            {
                iBirthRate = Util::randNum(m_iBirthRateMin, m_iBirthRateMax);
            }
            else
            {
                iBirthRate = m_iBirthRate;
            }
            m_fToSpawn += iBirthRate * p_fDeltaTime;
            iNumSpawns = int(m_fToSpawn);
            m_fToSpawn -= iNumSpawns;
        }

        else if (m_eMode == BURST)
        {
            if (m_fTimeToBurst > 0.0f)
            {
                m_fTimeToBurst -= p_fDeltaTime;
                if (m_bRandomBurstSpawns)
                    iNumSpawns = Util::randNum(m_iBurstSpawnsMin, m_iBurstSpawnsMax);
                else
                {
                    iNumSpawns = m_iBurstSpawns;
                    if (m_bRepeatBurst)
                    {
                        _calcBurstTime();
                    }
                }
            }
        }

        // Spawn Particles
        iNumSpawns = (iNumSpawns > m_iNumOfParticles) ? m_iNumOfParticles : iNumSpawns;
        for (int i = 0; i < iNumSpawns; i++)
        {
            _spawnParticle();
        }

        // Apply affectors
        Particle *pCurrent = m_pActiveHead;
        while (pCurrent)
        {
            for (Affector *affector : pCurrent->m_vAffectors)
            {
                if (pCurrent->m_bInitialized)
                {
                    affector->apply(p_fDeltaTime, pCurrent);
                }
            }
            pCurrent = pCurrent->m_pNext;
        }
    }
}

void Emitter::render(const glm::mat4 &p_mProj, const glm::mat4 &p_mView)
{

    glm::mat4 mBillboard = glm::transpose(p_mView);
    m_vVerts.clear();
    Particle *pCurrent = m_pActiveHead;
    while (pCurrent)
    {
        // pCurrent->m_mWorldMatrix = mBillboard;
        pCurrent = pCurrent->m_pNext;
    }

    m_pMaterial->SetUniform("projection", p_mProj);
    m_pMaterial->SetUniform("view", p_mView);
    m_pMaterial->SetUniform("world", m_mTransform);

    m_mutex.lock();
    pCurrent = m_pActiveHead;
    int i = 0;
    while (pCurrent)
    {
        glm::vec3 pTransformedPosition = pCurrent->m_vPosition * pCurrent->m_mWorldMatrix;
        m_vVerts.push_back(Vertex{pTransformedPosition.x, pTransformedPosition.y, pTransformedPosition.z,
                                  pCurrent->m_vColor.r, pCurrent->m_vColor.g, pCurrent->m_vColor.b,
                                  pCurrent->m_fFade, pCurrent->m_fSize});
        i++;
        pCurrent = pCurrent->m_pNext;
    }
    m_mutex.unlock();

    if (!m_vVerts.empty())
    {
        Vertex *pVertArray = &m_vVerts[0];
        m_pVertexBuffer = wolf::BufferManager::CreateVertexBuffer(pVertArray, sizeof(Vertex) * (int)m_vVerts.size());

        // Set up source data
        m_pDeclaration = new wolf::VertexDeclaration();
        m_pDeclaration->Begin();
        m_pDeclaration->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
        m_pDeclaration->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);
        m_pDeclaration->AppendAttribute(wolf::AT_Normal, 1, wolf::CT_Float);
        m_pDeclaration->SetVertexBuffer(m_pVertexBuffer);
        m_pDeclaration->End();

        m_pMaterial->Apply();
        m_pDeclaration->Bind();
        glDrawArrays(GL_POINTS, 0, (int)m_vVerts.size());
    }
}

void Emitter::_calcBurstTime()
{
    if (m_bRandomBurstTime)
        m_fTimeToBurst = Util::randNum(m_fBurstTimeMin, m_fBurstTimeMax);
    else
        m_fTimeToBurst = m_fBurstTime;
}

void Emitter::_addToActive(Particle *p_pParticle)
{
    p_pParticle->m_bInitialized = true;
    p_pParticle->m_pPrev = 0;
    p_pParticle->m_pNext = m_pActiveHead;

    if (!m_pActiveTail)
        m_pActiveTail = p_pParticle;

    if (m_pActiveHead)
        m_pActiveHead->m_pPrev = p_pParticle;

    m_pActiveHead = p_pParticle;
}

void Emitter::_addToFree(Particle *p_pParticle)
{
    p_pParticle->m_bInitialized = false;
    p_pParticle->m_pPrev = 0;
    p_pParticle->m_pNext = m_pFreeList;

    if (m_pFreeList)
        m_pFreeList->m_pPrev = p_pParticle;

    m_pFreeList = p_pParticle;
}

Emitter::Particle *Emitter::_getFreeParticle()
{
    Particle *pFreeParticle;
    if (m_pFreeList)
    {
        pFreeParticle = m_pFreeList;
        m_pFreeList = m_pFreeList->m_pNext;
        if (m_pFreeList)
            m_pFreeList->m_pPrev = 0;
        pFreeParticle->m_pPrev = 0;
        pFreeParticle->m_pNext = 0;
    }
    else
    {
        pFreeParticle = m_pActiveTail;
        _removeFromActive(pFreeParticle);
    }

    return pFreeParticle;
}

void Emitter::_spawnParticle()
{
    Particle *p_pParticle = _getFreeParticle();

    // - - -- - - -  Properties - - - - - - - //

    // Color
    if (m_bRandomColor)
        p_pParticle->m_vColor = Util::randVector(m_vColorMin, m_vColorMax);
    else
        p_pParticle->m_vColor = m_vColor;

    // Velocity
    if (m_bRandomVelocity)
        p_pParticle->m_vVelocity = Util::randVector(m_vVelocityMin, m_vVelocityMax);
    else
        p_pParticle->m_vVelocity = m_vVelocity;

    // Size
    if (m_bRandomSize)
        p_pParticle->m_fSize = Util::randNum(m_fSizeMin, m_fSizeMax);
    else
        p_pParticle->m_fSize = m_fSize;

    // Lifetime
    if (m_bRandomLifetime)
        p_pParticle->m_fLifetime = Util::randNum(m_fLifetimeMin, m_fLifetimeMax);
    else
        p_pParticle->m_fLifetime = m_fLifetime;

    // Fade
    if (m_bRandomFade)
        p_pParticle->m_fFade = Util::randNum(m_fFadeMin, m_fFadeMax);
    else
        p_pParticle->m_fFade = m_fFade;

    // Position
    if (m_eType == POINT_EMITTER)
        p_pParticle->m_vPosition = m_vPosition;
    else if (m_eType == BOX_EMITTER)
    {
        p_pParticle->m_vPosition.x = m_vPosition.x + Util::randNum(m_vBoxEmitterMin.x, m_vBoxEmitterMax.x);
        p_pParticle->m_vPosition.y = m_vPosition.y + Util::randNum(m_vBoxEmitterMin.y, m_vBoxEmitterMax.y);
        p_pParticle->m_vPosition.z = m_vPosition.z + Util::randNum(m_vBoxEmitterMin.z, m_vBoxEmitterMax.z);
    }
    else if (m_eType == SPHERE_EMITTER)
    {

        // Random position inside sphere
        float theta = Util::randNum(0.0f, 2.0f * (float)PI);
        float v = Util::randNum(0.0f, 1.0f);
        float phi = acos((2 * v) - 1);
        p_pParticle->m_vPosition.x = m_vPosition.x + m_fSphereRadius * sin(phi) * cos(theta);
        p_pParticle->m_vPosition.y = m_vPosition.y + m_fSphereRadius * sin(phi) * sin(theta);
        p_pParticle->m_vPosition.z = m_vPosition.z + m_fSphereRadius * cos(phi);
    }

    _addToActive(p_pParticle);
}

void Emitter::killParticle(Particle *p_pParticle)
{
    _removeFromActive(p_pParticle);
    _addToFree(p_pParticle);
}

void Emitter::_removeFromActive(Particle *p_pParticle)
{
    p_pParticle->m_bInitialized = false;

    if (p_pParticle == m_pActiveTail)
    {
        m_pActiveTail = m_pActiveTail->m_pPrev;
    }

    if (p_pParticle->m_pPrev)
        p_pParticle->m_pPrev->m_pNext = p_pParticle->m_pNext;
    else
        m_pActiveHead = p_pParticle->m_pNext;

    if (p_pParticle->m_pNext)
        p_pParticle->m_pNext->m_pPrev = p_pParticle->m_pPrev;
}

void Emitter::_deleteLinkedList(Particle *p_pHead)
{
    Particle *pCurrent = p_pHead;
    Particle *next = nullptr;

    while (pCurrent != nullptr)
    {
        next = pCurrent->m_pNext;
        delete pCurrent;
        pCurrent = next;
    }

    p_pHead = nullptr;
}