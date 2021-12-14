#pragma once
#include "../../wolf/wolf.h"
#include "emitter.h"

class Affector
{
    public:
        Affector() {};
        virtual ~Affector() {};
        virtual void apply(float p_fDelta, Emitter::Particle* p_pParticle);
};