#pragma once

#include "../../wolf/wolf.h"
#include "node.h"

class Skybox : public Node
{
public:
    Skybox(const std::string &texturePath);
    ~Skybox() override;

    void Update(float dt) override;
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView) override;

private:
    static const Vertex s_skyboxVertices[36];

    wolf::VertexBuffer *m_vb = 0;
    wolf::VertexDeclaration *m_decl = 0;
    wolf::Texture *m_texture = 0;
    wolf::Material *m_material = 0;
};