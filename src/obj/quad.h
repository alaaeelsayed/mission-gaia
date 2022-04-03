#pragma once
#include "../../wolf/wolf.h"
#include "node.h"

class Quad
{
public:
    Quad(const std::string &texturePath);
    virtual ~Quad();

    void Update(float dt);
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView);

    void setPosition(const glm::vec2 &vPosition);
    void setScale(const glm::vec2 &vScale);

private:
    static const Vertex2D s_quadVertices[6];

    bool m_dirty;
    glm::vec2 m_vPosition = glm::vec2(0.0f, 0.0f);
    glm::vec2 m_vScale = glm::vec2(1.0f, 1.0f);

    wolf::VertexBuffer *m_vb = 0;
    wolf::VertexDeclaration *m_decl = 0;
    wolf::Texture *m_texture = 0;
    wolf::Material *m_material = 0;
};
