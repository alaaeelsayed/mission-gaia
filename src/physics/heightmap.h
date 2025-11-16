#pragma once

#include "../terrain/terraingenerator.h"
#include "PxPhysicsAPI.h"
#include <vector>

using namespace physx;

class Heightmap
{
public:
	Heightmap(std::vector<TerrainVertex> verts, int vertCount, int terrainSize, PxVec3 position);
	~Heightmap();

private:
	PxHeightFieldSample* m_heightFieldSamples;
	PxRigidStatic* m_actor;
};