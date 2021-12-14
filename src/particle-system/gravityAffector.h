#pragma once
#include "affector.h"

class GravityAffector : public Affector
{
    public:
        GravityAffector() : Affector() {}
        ~GravityAffector() override {}
        void apply(float p_fDelta, Emitter::Particle* p_pParticle) override { 
            p_pParticle->m_vPosition -= glm::vec3(0.0f, m_fGravity, 0.0f);
            Affector::apply(p_fDelta, p_pParticle);
        }

    private:
        float m_fGravity = 9.81f;
};