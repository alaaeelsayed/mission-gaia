#include "effect.h"

using namespace tinyxml2;
using namespace std;

Effect::Effect(string p_sFilePath)
{
    XMLDocument doc = new XMLDocument();

    if (doc.LoadFile(p_sFilePath.c_str()) != XML_SUCCESS)
        throw XML_ERROR_FILE_COULD_NOT_BE_OPENED;

    XMLElement *effect = doc.FirstChildElement("effect");
    if (effect == nullptr)
        throw XML_ERROR_FILE_READ_ERROR;

    m_sName = effect->Attribute("name");

    for (XMLElement *element = effect->FirstChildElement("emitter"); element != NULL; element = element->NextSiblingElement("emitter"))
    {
        string sEmitterName = element->Attribute("file");
        string sEmitterPath = "data/effects/emitters/" + sEmitterName;
        string sEmitterOffset = element->Attribute("offset");
        vector<string> offset = Util::splitString(sEmitterOffset, m_cDelimeter);
        glm::vec3 vEmitterOffset = glm::vec3(stof(offset[0]), stof(offset[1]), stof(offset[2]));

        // Create and offset Emitters
        Emitter *emitter = new Emitter(sEmitterPath);
        emitter->setOffset(vEmitterOffset);
        m_vEmitters.push_back(emitter);
    }
}

Effect::~Effect()
{
    for (int i = 0; i < m_vEmitters.size(); i++)
        delete m_vEmitters[i];
}

void Effect::setTransform(const glm::mat4 &p_mTransform)
{
    for (int i = 0; i < m_vEmitters.size(); i++)
    {
        m_vEmitters[i]->setTransform(p_mTransform);
    }
    m_vPosition = p_mTransform[3]; // Position vector
}

void Effect::setPos(const glm::vec3 &p_vPosition)
{
    m_vPosition = p_vPosition;
    m_mTransform = glm::translate(glm::mat4(1.0f), m_vPosition);
    for (int i = 0; i < m_vEmitters.size(); i++)
    {
        m_vEmitters[i]->setTransform(m_mTransform);
    }
}

void Effect::start()
{
    m_bPause = false;
}

void Effect::pause()
{
    m_bPause = true;
}

bool Effect::isPaused() const
{
    return m_bPause;
}

void Effect::stop()
{
    m_bStopped = true;
}

void Effect::update(float p_fDeltaTime)
{
    if (!m_bPause && !m_bStopped)
    {
        for (Emitter *emitter : m_vEmitters)
        {
            emitter->update(p_fDeltaTime);
        }
    }
}

void Effect::render(const glm::mat4 &p_mProj, const glm::mat4 &p_mView)
{
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    if (!m_bStopped)
    {
        for (Emitter *emitter : m_vEmitters)
        {
            emitter->render(p_mProj, p_mView);
        }
    }
}
