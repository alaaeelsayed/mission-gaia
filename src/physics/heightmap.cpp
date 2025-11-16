#include "heightmap.h"
#include "../../wolf/W_PhysicsManager.h"

Heightmap::Heightmap(std::vector<TerrainVertex> verts, int vertCount, int terrainSize, PxVec3 position)
{
	PxReal minHeight = PX_MAX_F32;
	PxReal maxHeight = -PX_MAX_F32;
	for (PxU32 s = 0; s < vertCount * vertCount; s++)
	{
		minHeight = PxMin(minHeight, verts[s].y);
		maxHeight = PxMax(maxHeight, verts[s].y);
	}
	PxReal deltaHeight = maxHeight - minHeight;
	PxReal quantization = (PxReal)0x7fff;
	PxReal heightScale = PxMax(deltaHeight / quantization, PX_MIN_HEIGHTFIELD_Y_SCALE);

	m_heightFieldSamples = new PxHeightFieldSample[vertCount * vertCount];

	PxU32 index = 0;
	for (PxU32 col = 0; col < vertCount; col++)
	{
		for (PxU32 row = 0; row < vertCount; row++)
		{
			PxI16 height;
			height = PxI16(quantization * ((verts[(col * vertCount) + row].y - minHeight) /
				deltaHeight));

			PxHeightFieldSample& smp = m_heightFieldSamples[(row * vertCount) + col];
			smp.height = height;
			smp.materialIndex0 = 0;
			smp.materialIndex1 = 0;
			if (false)
				smp.setTessFlag();
		}
	}

	PxHeightFieldDesc terrainDesc;
	terrainDesc.format = PxHeightFieldFormat::eS16_TM;
	terrainDesc.nbColumns = vertCount;
	terrainDesc.nbRows = vertCount;
	terrainDesc.samples.data = m_heightFieldSamples;
	terrainDesc.samples.stride = sizeof(PxHeightFieldSample);
	terrainDesc.flags = PxHeightFieldFlags(PxHeightFieldFlag::eNO_BOUNDARY_EDGES);

	PxHeightFieldGeometry hfGeom;
	hfGeom.columnScale = terrainSize / (vertCount - 1.0f);
	hfGeom.rowScale = terrainSize / (vertCount - 1.0f);
	hfGeom.heightScale = heightScale;
	hfGeom.heightField = PxCreateHeightField(terrainDesc, wolf::PhysicsManager::Instance()->GetPhysics()->getPhysicsInsertionCallback());
	printf("Height Scale: %f\n", heightScale);
	PxTransform localPose;
	localPose.p = PxVec3(0,
		minHeight, 0);
	localPose.q = PxQuat(PxIdentity);

	PxMaterial* material[] = { wolf::PhysicsManager::Instance()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f) };

	m_actor = wolf::PhysicsManager::Instance()->GetPhysics()->createRigidStatic(PxTransform(position));

	PxShape* shape = PxRigidActorExt::createExclusiveShape(*m_actor, hfGeom, material, 1);
	shape->setLocalPose(localPose);

	wolf::PhysicsManager::Instance()->GetScene()->addActor(*m_actor);

}

Heightmap::~Heightmap()
{
	wolf::PhysicsManager::Instance()->GetScene()->removeActor(*m_actor);
	PX_RELEASE(m_actor);
}
