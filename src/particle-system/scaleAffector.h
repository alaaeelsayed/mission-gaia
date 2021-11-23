#pragma once
#include "../../wolf/wolf.h"
#include "affector.h"

enum Mode
{
    SCALE_CONSTANT,
    SCALE_OVER_LIFETIME
};

class ScaleAffector : public Affector
{
public:
    ScaleAffector(float p_fStartSize, float p_fEndSize, std::string p_sMode) : Affector()
    {
        m_fStartSize = p_fStartSize;
        m_fEndSize = p_fEndSize;
        m_eMode = p_sMode == "over_life" ? SCALE_OVER_LIFETIME : SCALE_CONSTANT;
    }
    ~ScaleAffector() override {}
    void apply(float p_fDelta, Emitter::Particle *p_pParticle) override
    {
        if (m_eMode == SCALE_OVER_LIFETIME)
        {
            p_pParticle->m_fSize = m_fStartSize + p_pParticle->m_fLifetime * (m_fEndSize - m_fStartSize);
        }
        Affector::apply(p_fDelta, p_pParticle);
    }

private:
    float m_fStartSize = 0.0f;
    float m_fEndSize = 0.0f;
    float m_fSpeed = 0.1f;
    bool m_bRandomSize = false;
    Mode m_eMode = SCALE_OVER_LIFETIME;
};