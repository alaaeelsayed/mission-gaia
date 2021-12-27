#pragma once
#include "../../wolf/wolf.h"
#include "affector.h"

class AddVelAffector : public Affector
{
public:
    AddVelAffector() : Affector() {}
    ~AddVelAffector() override {}
    void apply(float p_fDelta, Emitter::Particle *p_pParticle) override
    {
        p_pParticle->m_vPosition += p_pParticle->m_vVelocity;
        Affector::apply(p_fDelta, p_pParticle);
    }
};