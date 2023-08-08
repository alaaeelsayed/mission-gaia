#pragma once

#include "../wolf/wolf.h"
#include "../src/physics/rigidBody.h"

class Sphere
{
public:
    Sphere(float radius);
    ~Sphere();

    void SetPosition(const glm::vec3 &pos) { m_pos = pos; }
    glm::vec3 GetPosition() { return m_pos; }
    void SetRadius(float radius)
    {
        m_radius = radius;
        m_scale = glm::vec3(radius / BASE_RADIUS, radius / BASE_RADIUS, radius / BASE_RADIUS);
    }

    void setTag(const std::string &p_sTag)
    {
        m_sTag = p_sTag;
    }

    const std::string &getTag()
    {
        return m_sTag;
    }

    void attachRigidBody(const std::string &p_sConfiguration);

    void SetColor(const glm::vec4 &color) { m_color = color; }

    void Render(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix);

    void Update(float p_fDelta);

private:
    void _genVerts(int sectorCount, int stackCount);

    static wolf::VertexBuffer *s_pVB;
    static wolf::IndexBuffer *s_pIB;
    static wolf::VertexDeclaration *s_pDecl;
    static int s_numIndices;

    float m_radius;
    glm::vec3 m_pos;
    glm::vec3 m_scale;
    glm::vec4 m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    wolf::Material *m_pMat;
    RigidBody *m_pRigidBody = nullptr;

    std::string m_sTag = "";

    const float BASE_RADIUS = 0.5f;
};