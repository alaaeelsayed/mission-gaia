#pragma once
#include "../../wolf/wolf.h"
#include "../../tinyxml2/tinyxml2.h"
#include "emitter.h"
#include "../misc/util.h"

class Effect
{
public:
    Effect(std::string p_sFilePath);
    ~Effect();

    void setTransform(const glm::mat4 &p_mTransform);
    void setPos(const glm::vec3 &p_vPosition);

    void start();
    void pause();
    bool isPaused() const;
    void stop();

    void update(float p_fDeltaTime);
    void render(const glm::mat4 &p_mProj, const glm::mat4 &p_mView);

private:
    std::string m_sName;
    std::vector<Emitter *> m_vEmitters;
    glm::mat4 m_mTransform = glm::mat4(1.0f);
    glm::vec3 m_vPosition = glm::vec3(0.0f);

    bool m_bPause = false;
    bool m_bStopped = false;

    const char m_cDelimeter = ',';
};