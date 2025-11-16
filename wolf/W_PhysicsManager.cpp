//------------------------------------------------------------------------
// PhysicsManager
//
// Created:	2023/09/13
// Author:	Alaa Emad
//	
// Manager to handle integration and initialization with PhysX.
//------------------------------------------------------------------------

#include "W_PhysicsManager.h"
#include <cassert>

using namespace wolf;

// Static singleton instance
PhysicsManager* PhysicsManager::s_pPhysicsManagerInstance = NULL;

//------------------------------------------------------------------------------
// Method:    CreateInstance
// Returns:   void
// 
// Creates the singleton instance.
//------------------------------------------------------------------------------
void PhysicsManager::CreateInstance(PxTolerancesScale scale, PxVec3 gravity)
{
	assert(s_pPhysicsManagerInstance == NULL);
	s_pPhysicsManagerInstance = new PhysicsManager(scale, gravity);
}

//------------------------------------------------------------------------------
// Method:    DestroyInstance
// Returns:   void
// 
// Destroys the singleton instance.
//------------------------------------------------------------------------------
void PhysicsManager::DestroyInstance()
{
	assert(s_pPhysicsManagerInstance != NULL);
	delete s_pPhysicsManagerInstance;
	s_pPhysicsManagerInstance = NULL;
}

//------------------------------------------------------------------------------
// Method:    Instance
// Returns:   BulletPhysicsManager::BulletPhysicsManager*
// 
// Access to singleton instance.
//------------------------------------------------------------------------------
PhysicsManager* PhysicsManager::Instance()
{
	assert(s_pPhysicsManagerInstance);
	return s_pPhysicsManagerInstance;
}

//------------------------------------------------------------------------------
// Method:    BulletPhysicsManager
// Parameter: const char * p_strMaterialsPath
// Parameter: const char * p_strDebugLinesVertexShader
// Parameter: const char * p_strDebugLinesFragShader
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
PhysicsManager::PhysicsManager(PxTolerancesScale scale, PxVec3 gravity)
	:
	m_pDispatcher(NULL),
	m_pFoundation(NULL),
	m_pPhysics(NULL),
	m_pPvd(NULL),
	m_pScene(NULL)
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	m_pPvd = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, scale, true, m_pPvd);

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = gravity;
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_pScene = m_pPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pControllerManager = PxCreateControllerManager(*m_pScene);
}

//------------------------------------------------------------------------------
// Method:    ~BulletPhysicsManager
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
PhysicsManager::~PhysicsManager()
{
	PX_RELEASE(m_pScene);
	PX_RELEASE(m_pDispatcher);
	PX_RELEASE(m_pPhysics);
	if (m_pPvd)
	{
		PxPvdTransport* transport = m_pPvd->getTransport();
		m_pPvd->release();	m_pPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_pFoundation);
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Updates PhysX SDK and runs physics simulation
//------------------------------------------------------------------------------
void PhysicsManager::Update(float p_fDelta)
{
	m_pScene->simulate(p_fDelta);
	m_pScene->fetchResults(true);
}

PxScene* wolf::PhysicsManager::GetScene()
{
	return m_pScene;
}

PxPhysics* wolf::PhysicsManager::GetPhysics()
{
	return m_pPhysics;
}

PxControllerManager* wolf::PhysicsManager::GetControllerManager()
{
	return m_pControllerManager;
}
