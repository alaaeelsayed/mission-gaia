#pragma once
#include "../../wolf/wolf.h"
#include "node.h"

class Plane : public Node
{
public:
    Plane(const std::string &texturePath);
    Plane(int subdivisions = 0);

    virtual ~Plane();

    void Update(float dt) override;
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView) override;

private:
    static const Vertex s_planeVertices[6];

    int m_numSegments = 1;
    float *m_textureCoords = nullptr;
    wolf::VertexBuffer *m_vb = 0;
    wolf::VertexDeclaration *m_decl = 0;
    wolf::Texture *m_texture = 0;
    wolf::Material *m_material = 0;
};