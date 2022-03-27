#include "Quad.h"

const Vertex Quad::s_quadVertices[6] = {
    // Front
    {-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}};

Quad::~Quad()
{
    delete m_decl;
    wolf::BufferManager::DestroyBuffer(m_vb);
    wolf::TextureManager::DestroyTexture(m_texture);
    wolf::MaterialManager::DestroyMaterial(m_material);
}

Quad::Quad(const std::string &texturePath)
    : Node(BoundingBox())
{
    m_vb = wolf::BufferManager::CreateVertexBuffer(s_quadVertices, sizeof(Vertex) * 6);

    m_texture = wolf::TextureManager::CreateTexture(texturePath);

    // Material should be unique to a texture
    m_material = wolf::MaterialManager::CreateMaterial(texturePath);
    m_material->SetProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
    m_material->SetTexture("u_texture", m_texture);

    m_decl = new wolf::VertexDeclaration();
    m_decl->Begin();
    m_decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
    m_decl->SetVertexBuffer(m_vb);
    m_decl->End();
}

void Quad::Update(float dt)
{
    if (m_dirty)
    {
        glm::vec3 dimensions = GetScale();
        glm::vec3 min = GetWorldPos() - dimensions / 2.0f;
        glm::vec3 max = min + dimensions;
        GetBoundingBox().SetMin(min);
        GetBoundingBox().SetMax(max);
        m_dirty = false;
    }
}

void Quad::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    m_material->SetUniform("projection", mProj);
    m_material->SetUniform("view", mView);
    m_material->SetUniform("world", GetWorldTransform());
    m_material->Apply();

    m_texture->Bind();

    m_decl->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}