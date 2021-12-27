#pragma once
#include "../../wolf/wolf.h"
#include "node.h"

class Cube : public Node
{
public:
    Cube(const std::string &texturePath);
    ~Cube() override;

    void Update(float dt) override;
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView) override;

    void SetWidth(float width);
    void SetHeight(float height);
    void SetDepth(float depth);

    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;

private:
    static const Vertex s_cubeVertices[36];

    wolf::VertexBuffer *m_vb = 0;
    wolf::VertexDeclaration *m_decl = 0;
    wolf::Texture *m_texture = 0;
    wolf::Material *m_material = 0;
};
