//------------------------------------------------------------------------
// BulletPhysicsManager
//
// Created:	2013/02/17
// Author:	Carel Boers
//	
// Manager to handle integration and initialization with Bullet.
//------------------------------------------------------------------------

#ifndef W_PHYSICSMANAGER_H
#define W_PHYSICSMANAGER_H

#include "PxPhysicsAPI.h"

using namespace physx;

namespace wolf
{
	class PhysicsManager
	{
	public:
		//---------------------------------------------------------------------
		// Public Methods.
		//---------------------------------------------------------------------

		static void CreateInstance(PxTolerancesScale scale = PxTolerancesScale(), PxVec3 gravity = PxVec3(0.0f, -9.81f, 0.0f));
		static void DestroyInstance();
		static PhysicsManager* Instance();

		void Update(float p_fDelta);

		PxScene* GetScene();
		PxPhysics* GetPhysics();
		PxControllerManager* GetControllerManager();

	private:
		//--------------------------------------------------------------------
		// Private methods.
		//--------------------------------------------------------------------

		// Private construction; we're a singleton
		PhysicsManager(PxTolerancesScale scale, PxVec3 gravity);
		~PhysicsManager();

	private:
		//---------------------------------------------------------------------
		// Private members.
		//---------------------------------------------------------------------

		// Static singleton instance
		static PhysicsManager* s_pPhysicsManagerInstance;

		// PhysX SDK members
		PxDefaultAllocator m_allocator;
		PxDefaultErrorCallback m_errorCallback;
		PxFoundation* m_pFoundation;
		PxPhysics* m_pPhysics;
		PxDefaultCpuDispatcher* m_pDispatcher;
		PxScene* m_pScene;
		PxPvd* m_pPvd;
		PxControllerManager* m_pControllerManager;
	};
}

#endif