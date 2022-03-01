#pragma once
#include "../../wolf/wolf.h"
#include "../../thirdparty/bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

class RigidBody
{
public:
    enum Shape
    {
        Capsule,
        Box,
        Plane,
        Sphere,
        Terrain,
        ConvexHull
    };

    RigidBody(const std::string &pConfiguration);
    RigidBody(int heightStickWidth, int heightStickLength, void *heightfieldData, btScalar heightScale, btScalar minHeight, btScalar maxHeight, int upAxis, PHY_ScalarType hdt, bool flipQuadEdges);
    RigidBody(btConvexHullShape *pMesh);

    ~RigidBody();

    glm::vec3 Update(float p_fDelta, const glm::vec3 &p_vPosition);

    //------------------------------------------------------------------------------
    // Public methods for "GOC_RigidBody" family of components
    //------------------------------------------------------------------------------
    void Init(const glm::vec3 &p_vPosition, btCollisionShape *p_pCollisionShape, const std::string &p_strMaterial, float p_fMass, const glm::vec3 &p_vOffset, bool p_bIsKinematic = false);
    void InitValues(std::string p_sMaterial, bool p_bKinematic, float p_fMass,
                    const glm::vec3 &p_vOffset, float p_fRadius = 0.0f, float p_fHeight = 0.0f,
                    const btVector3 &p_vExtents = btVector3(0.0f, 0.0f, 0.0f), const btVector3 &p_vPlaneNormal = btVector3(0.0f, 0.0f, 0.0f),
                    float p_fPlaneConstant = 0.0f);
    btRigidBody *getBody() const
    {
        return m_pBody;
    };

    bool isKinematic() const
    {
        return m_bKinematic;
    }

    void setPosition(const glm::vec3 &vPosition)
    {
        if (m_pBody == nullptr)
            return;
        btTransform trans;
        m_pBody->getMotionState()->getWorldTransform(trans);

        m_vPosition = btVector3(vPosition.x, vPosition.y, vPosition.z) - btVector3(m_vOffset.x, m_vOffset.y, m_vOffset.z);
        trans.setOrigin(m_vPosition);
        m_pBody->getMotionState()->setWorldTransform(trans);
    }

private:
    // Bullet physics rigid body
    btRigidBody *m_pBody = nullptr;

    btVector3 m_vPosition;

    // Collision shape
    btCollisionShape *m_pCollisionShape = nullptr;

    // Is Physics controlled or manually controlled
    bool m_bReadyToInit = true;

    Shape m_eShape;

    // Values
    std::string m_sMaterial;
    bool m_bKinematic;
    float m_fMass;
    glm::vec3 m_vOffset;
    float m_fRadius;
    float m_fHeight;
    btVector3 m_vExtents;
    btVector3 m_vPlaneNormal;
    float m_fPlaneConstant;

    // Terrain Shape
    int m_heightStickWidth;
    int m_heightStickLength;
    void *m_heightfieldData;
    btScalar m_heightScale;
    btScalar m_minHeight;
    btScalar m_maxHeight;
    int m_upAxis;
    PHY_ScalarType m_hdt;
    bool m_flipQuadEdges;

    // Convex Hull
    btConvexHullShape *m_convexHull;
};