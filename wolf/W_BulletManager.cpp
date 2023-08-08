//------------------------------------------------------------------------
// BulletPhysicsManager
//
// Created:	2013/02/17
// Author:	Carel Boers
//
// Manager to handle integration and initialization with Bullet.
//------------------------------------------------------------------------

#include "W_BulletManager.h"
#include <cassert>

using namespace wolf;

// Static singleton instance
BulletPhysicsManager *BulletPhysicsManager::s_pPhysicsManagerInstance = NULL;

//------------------------------------------------------------------------------
// Method:    CreateInstance
// Returns:   void
//
// Creates the singleton instance.
//------------------------------------------------------------------------------
void BulletPhysicsManager::CreateInstance(const char *p_strMaterialsPath, const char *p_strDebugLinesVertexShader, const char *p_strDebugLinesFragShader)
{
    assert(s_pPhysicsManagerInstance == NULL);
    s_pPhysicsManagerInstance = new BulletPhysicsManager(p_strMaterialsPath, p_strDebugLinesVertexShader, p_strDebugLinesFragShader);
}

//------------------------------------------------------------------------------
// Method:    DestroyInstance
// Returns:   void
//
// Destroys the singleton instance.
//------------------------------------------------------------------------------
void BulletPhysicsManager::DestroyInstance()
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
BulletPhysicsManager *BulletPhysicsManager::Instance()
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
BulletPhysicsManager::BulletPhysicsManager(const char *p_strMaterialsPath, const char *p_strDebugLinesVertexShader, const char *p_strDebugLinesFragShader)
    : m_pCollisionConfiguration(NULL),
      m_pDispatcher(NULL),
      m_pOverlappingPairCache(NULL),
      m_pSolver(NULL),
      m_pDynamicsWorld(NULL),
      m_pMaterialManager(NULL),
      m_bRenderDebug(false),
      m_pLineDrawer(NULL),
      m_pDebugDrawer(NULL)
{
    // Initialize Bullet Physics members.
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    m_pOverlappingPairCache = new btDbvtBroadphase();
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pOverlappingPairCache, m_pSolver, m_pCollisionConfiguration);

    // Set world gravity.
    m_pDynamicsWorld->setGravity(btVector3(0, -10, 0));

    // Load materials
    m_pMaterialManager = new BulletPhysicsMaterialManager();
    m_pMaterialManager->LoadMaterials(p_strMaterialsPath);

    // Debug drawer for physics
    if (p_strDebugLinesVertexShader && p_strDebugLinesFragShader)
    {
        m_pLineDrawer = new wolf::LineDrawer();
        m_pLineDrawer->Init(p_strDebugLinesVertexShader, p_strDebugLinesFragShader);
        m_pDebugDrawer = new BulletPhysicsDebugDrawer(m_pLineDrawer);
        m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);
    }

    // Set ourselves as the world user info in order to cast back inside the TickCallback().
    m_pDynamicsWorld->setWorldUserInfo(this);

    // Register for tick callbacks
    m_pDynamicsWorld->setInternalTickCallback(TickCallback);
}

//------------------------------------------------------------------------------
// Method:    ~BulletPhysicsManager
// Parameter: void
// Returns:
//
// Destructor.
//------------------------------------------------------------------------------
BulletPhysicsManager::~BulletPhysicsManager()
{
    // Destroy debug drawer
    delete m_pDebugDrawer;
    m_pDebugDrawer = NULL;
    delete m_pLineDrawer;
    m_pLineDrawer = NULL;
    m_pDynamicsWorld->setDebugDrawer(NULL);

    // Cleanup rigid bodies
    for (int i = m_pDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i)
    {
        btCollisionObject *pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *pBody = btRigidBody::upcast(pObj);
        if (pBody && pBody->getMotionState())
        {
            delete pBody->getMotionState();
        }
        m_pDynamicsWorld->removeCollisionObject(pObj);
        delete pObj;
    }

    // Delete Bullet Physics members
    delete m_pDynamicsWorld;
    m_pDynamicsWorld = NULL;
    delete m_pSolver;
    m_pSolver = NULL;
    delete m_pOverlappingPairCache;
    m_pOverlappingPairCache = NULL;
    delete m_pDispatcher;
    m_pDispatcher = NULL;
    delete m_pCollisionConfiguration;
    m_pCollisionConfiguration = NULL;

    // Delete material manager
    delete m_pMaterialManager;
    m_pMaterialManager = NULL;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
//
// Updates Bullet physics SDK and runs physics simulation
//------------------------------------------------------------------------------
void BulletPhysicsManager::Update(float p_fDelta)
{
    m_pDynamicsWorld->stepSimulation(p_fDelta, 10);
}

//------------------------------------------------------------------------------
// Method:    Render
// Parameter: const glm::mat4 & p_mProj
// Parameter: const glm::mat4 & p_mView
// Returns:   void
//
// Render physics debugging info.
//------------------------------------------------------------------------------
void BulletPhysicsManager::Render(const glm::mat4 &p_mProj, const glm::mat4 &p_mView)
{
    m_pDynamicsWorld->debugDrawWorld();
    m_pLineDrawer->Render(p_mProj, p_mView);
}

//------------------------------------------------------------------------------
// Method:    ToggleDebugRendering
// Returns:   void
//
// Toggle debug rendering.
//------------------------------------------------------------------------------
void BulletPhysicsManager::ToggleDebugRendering()
{
    m_bRenderDebug = !m_bRenderDebug;
    m_pDebugDrawer->setDebugMode(m_bRenderDebug ? 1 : 0);
}

//------------------------------------------------------------------------------
// Method:    GetMaterial
// Parameter: const std::string & p_strMaterial
// Returns:   BulletPhysicsMaterialManager::PhysicsMaterial*
//
// Returns the physics material from the given material name.
//------------------------------------------------------------------------------
BulletPhysicsMaterialManager::PhysicsMaterial *BulletPhysicsManager::GetMaterial(const std::string &p_strMaterial)
{
    return m_pMaterialManager->GetMaterial(p_strMaterial);
}

//------------------------------------------------------------------------------
// Method:    TickCallback
// Parameter: btDynamicsWorld * p_pWorld
// Parameter: btScalar p_fTimeStep
// Returns:   void
//
// Callback received for each physics substep. We can do collision checking
// here in order to send collision events.
//------------------------------------------------------------------------------
void BulletPhysicsManager::TickCallback(btDynamicsWorld *p_pWorld, btScalar p_fTimeStep)
{
}