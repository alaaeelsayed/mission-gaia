#include "rigidBody.h"
#include "../../tinyxml2/tinyxml2.h"

RigidBody::~RigidBody()
{
}

RigidBody::RigidBody(const std::string &pConfiguration)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(pConfiguration.c_str()) == tinyxml2::XML_SUCCESS)
    {
        // Look for the root "rigidbody" node
        tinyxml2::XMLNode *pNode = doc.FirstChild();
        if (pNode != NULL)
        {

            const char *sShape = pNode->ToElement()->Attribute("Shape");

            // Values for InitValues()
            std::string sMaterial;
            bool bKinematic = true;
            float fMass = 0.0f;
            glm::vec3 vOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            float fRadius = 0.0f;
            float fHeight = 0.0f;
            btVector3 vExtents = btVector3(0.0f, 0.0f, 0.0f);
            btVector3 vPlaneNormal = btVector3(0.0f, 0.0f, 0.0f);
            float fPlaneConstant = 0.0f;

            tinyxml2::XMLNode *pChildNode = pNode->FirstChildElement("Material");
            if (pChildNode)
            {
                tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                sMaterial = pElement->Attribute("name");
            }

            pChildNode = pNode->FirstChildElement("Mass");
            if (pChildNode)
            {
                tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                pElement->QueryFloatAttribute("value", &fMass);
            }

            pChildNode = pNode->FirstChildElement("Offset");
            if (pChildNode)
            {
                float x, y, z;
                tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                pElement->QueryFloatAttribute("x", &x);
                pElement->QueryFloatAttribute("y", &y);
                pElement->QueryFloatAttribute("z", &z);
                vOffset = glm::vec3(x, y, z);
            }

            pChildNode = pNode->FirstChildElement("Kinematic");
            if (pChildNode)
            {
                tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                pElement->QueryBoolAttribute("value", &bKinematic);
            }

            if (!strcmp(sShape, "Capsule"))
            {

                pChildNode = pNode->FirstChildElement("Radius");
                if (pChildNode)
                {
                    tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                    pElement->QueryFloatAttribute("value", &fRadius);
                }

                pChildNode = pNode->FirstChildElement("Height");
                if (pChildNode)
                {
                    tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                    pElement->QueryFloatAttribute("value", &fHeight);
                }

                m_eShape = Capsule;
            }
            else if (!strcmp(sShape, "Box"))
            {
                pChildNode = pNode->FirstChildElement("BoxHalfExtents");
                if (pChildNode)
                {
                    float x, y, z;
                    tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                    pElement->QueryFloatAttribute("x", &x);
                    pElement->QueryFloatAttribute("y", &y);
                    pElement->QueryFloatAttribute("z", &z);
                    vExtents = btVector3(x, y, z);
                }

                m_eShape = Box;
            }
            else if (!strcmp(sShape, "StaticPlane"))
            {

                pChildNode = pNode->FirstChildElement("PlaneNormal");
                if (pChildNode)
                {
                    float x, y, z;
                    tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                    pElement->QueryFloatAttribute("x", &x);
                    pElement->QueryFloatAttribute("y", &y);
                    pElement->QueryFloatAttribute("z", &z);
                    vPlaneNormal = btVector3(x, y, z);
                }

                pChildNode = pNode->FirstChildElement("PlaneConstant");
                if (pChildNode)
                {
                    tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                    pElement->QueryFloatAttribute("value", &fPlaneConstant);
                }

                m_eShape = Plane;
            }
            else if (!strcmp(sShape, "Sphere"))
            {
                pChildNode = pNode->FirstChildElement("Radius");
                if (pChildNode)
                {
                    tinyxml2::XMLElement *pElement = pChildNode->ToElement();
                    pElement->QueryFloatAttribute("value", &fRadius);
                }

                m_eShape = Sphere;
            }
            InitValues(sMaterial, bKinematic, fMass, vOffset, fRadius,
                       fHeight, vExtents, vPlaneNormal, fPlaneConstant);
        }
    }
}

void RigidBody::Update(float p_fDelta, const glm::vec3 &p_vPosition)
{
    if (m_bReadyToInit)
    {
        switch (m_eShape)
        {
        case Capsule:
            Init(new btCapsuleShape(m_fRadius, m_fHeight), m_sMaterial, m_fMass, m_vOffset, m_bKinematic);
            break;
        case Box:
            Init(new btBoxShape(m_vExtents), m_sMaterial, m_fMass, m_vOffset, m_bKinematic);
            break;
        case Plane:
            Init(new btStaticPlaneShape(m_vPlaneNormal, m_fPlaneConstant), m_sMaterial, m_fMass, m_vOffset, m_bKinematic);
            break;
        case Sphere:
            Init(new btSphereShape(m_fRadius), m_sMaterial, m_fMass, m_vOffset, m_bKinematic);
            break;
        }
        m_bReadyToInit = false;
    }
    if (m_bKinematic)
    {
        // Get the rigid body transform
        btTransform trans;
        m_pBody->getMotionState()->getWorldTransform(trans);

        // Apply to the rigid body
        // const glm::quat &qRotation = this->GetGameObject()->GetTransform().GetRotation();
        trans.setOrigin(btVector3(p_vPosition.x, p_vPosition.y, p_vPosition.z) - btVector3(m_vOffset.x, m_vOffset.y, m_vOffset.z));
        // trans.setRotation(btQuaternion(qRotation.x, qRotation.y, qRotation.z, qRotation.w));
        m_pBody->getMotionState()->setWorldTransform(trans);
    }
    else
    {
        // Get the rigid body transform
        btTransform trans;
        m_pBody->getMotionState()->getWorldTransform(trans);

        // Rotation and translation
        glm::quat qRot = glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
        glm::vec3 vPos = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
        glm::vec3 vOffset = glm::mat3_cast(qRot) * m_vOffset;

        // Apply to the game object
        // Transform &transform = this->GetGameObject()->GetTransform();
        // transform.SetTranslation(vPos + vOffset);
        // transform.SetRotation(qRot);
    }
}

void RigidBody::Init(btCollisionShape *p_pCollisionShape, const std::string &p_strMaterial, float p_fMass, const glm::vec3 &p_vOffset, bool p_bIsKinematic)
{
    m_pCollisionShape = p_pCollisionShape;
    m_bKinematic = p_bIsKinematic;
    m_vOffset = p_vOffset;

    // Set mass
    btScalar mass(p_fMass);
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
    {
        m_pCollisionShape->calculateLocalInertia(mass, localInertia);
    }

    // Set initial transform
    // Transform &transform = this->GetGameObject()->GetTransform();
    // btTransform startTransform;
    // startTransform.setIdentity();
    // startTransform.setOrigin(btVector3(transform.GetTranslation().x, transform.GetTranslation().y, transform.GetTranslation().z));

    // // Setup the motion state
    // btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
    // btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_pCollisionShape, localInertia);

    // // Material specific properties
    // BulletPhysicsMaterialManager::PhysicsMaterial *pMaterial = BulletPhysicsManager::Instance()->GetMaterial(p_strMaterial);
    // if (pMaterial)
    // {
    //     rbInfo.m_restitution = pMaterial->restitution;
    //     rbInfo.m_friction = pMaterial->friction;
    // }

    // m_pBody = new btRigidBody(rbInfo);
    // m_pBody->setUserPointer(this->GetGameObject());

    // // Set kinematic flags if this rigid body is manually positioned
    // if (m_bKinematic)
    // {
    //     m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    //     m_pBody->setActivationState(DISABLE_DEACTIVATION);
    // }
    // // Add rigid body to the world
    // BulletPhysicsManager::Instance()->GetWorld()->addRigidBody(m_pBody);
}

void RigidBody::InitValues(std::string p_sMaterial, bool p_bKinematic, float p_fMass,
                           const glm::vec3 &p_vOffset, float p_fRadius, float p_fHeight,
                           const btVector3 &p_vExtents, const btVector3 &p_vPlaneNormal,
                           float p_fPlaneConstant)
{
    m_sMaterial = p_sMaterial;
    m_bKinematic = p_bKinematic;
    m_fMass = p_fMass;
    m_vOffset = p_vOffset;
    m_fRadius = p_fRadius;
    m_fHeight = p_fHeight;
    m_vExtents = p_vExtents;
    m_vPlaneNormal = p_vPlaneNormal;
    m_fPlaneConstant = p_fPlaneConstant;
}
