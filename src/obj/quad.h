#pragma once
#include "../../wolf/wolf.h"
#include "node.h"

class Quad : public Node
{
public:
    Quad(const std::string &texturePath);
    virtual ~Quad();

    void Update(float dt) override;
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView) override;

    void SetWidth(float width);
    void SetHeight(float height);
    void SetDepth(float depth);

    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;

private:
    static const Vertex s_quadVertices[6];

    bool m_dirty;

    wolf::VertexBuffer *m_vb = 0;
    wolf::VertexDeclaration *m_decl = 0;
    wolf::Texture *m_texture = 0;
    wolf::Material *m_material = 0;
};
