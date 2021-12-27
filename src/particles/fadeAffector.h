#pragma once
#include "affector.h"

class FadeAffector : public Affector
{
    public:
        FadeAffector() : Affector() {}
        ~FadeAffector() override {}
        void apply(float p_fDelta, Emitter::Particle* p_pParticle) override { 
            p_pParticle->m_fFade = p_pParticle->m_fLifetime;
            Affector::apply(p_fDelta, p_pParticle);
        }

    private:
        float m_fFadeSpeed;
};