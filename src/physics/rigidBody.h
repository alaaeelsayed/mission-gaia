#pragma once
#include "../../wolf/wolf.h"

class RigidBody
{
public:
    enum Shape
    {
        Capsule,
        Box,
        Plane,
        Sphere
    };

    RigidBody(const std::string &pConfiguration);
    ~RigidBody();

    void Update(float p_fDelta, const glm::vec3 &p_vPosition, void* p_pObject);

    //------------------------------------------------------------------------------
    // Public methods for "GOC_RigidBody" family of components
    //------------------------------------------------------------------------------
    void Init(btCollisionShape *p_pCollisionShape, const std::string &p_strMaterial, float p_fMass, const glm::vec3 &p_vOffset, bool p_bIsKinematic = false);
    void InitValues(std::string p_sMaterial, bool p_bKinematic, float p_fMass,
                    const glm::vec3 &p_vOffset, float p_fRadius = 0.0f, float p_fHeight = 0.0f,
                    const btVector3 &p_vExtents = btVector3(0.0f, 0.0f, 0.0f), const btVector3 &p_vPlaneNormal = btVector3(0.0f, 0.0f, 0.0f),
                    float p_fPlaneConstant = 0.0f);
    btRigidBody *getBody() const
    {
        return m_pBody;
    };

private:
    // Bullet physics rigid body
    btRigidBody *m_pBody = nullptr;

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
};