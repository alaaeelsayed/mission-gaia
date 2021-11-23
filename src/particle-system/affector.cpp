#include "affector.h"

void Affector::apply(float p_fDelta, Emitter::Particle* p_pParticle){
    if(p_pParticle->m_bInitialized){
        p_pParticle->m_fLifetime -= p_fDelta;
        if( p_pParticle->m_fLifetime <= 0.0f)
            p_pParticle->m_pEmitter->killParticle(p_pParticle);
    }
}