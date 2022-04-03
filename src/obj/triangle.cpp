#include "triangle.h"

const Vertex2D Triangle::s_triangleVertices[3] = {
    // Front
    {-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
};

Triangle::~Triangle()
{
    delete m_decl;
    wolf::BufferManager::DestroyBuffer(m_vb);
    wolf::TextureManager::DestroyTexture(m_texture);
    wolf::MaterialManager::DestroyMaterial(m_material);
}

Triangle::Triangle(const std::string &texturePath)
{
    m_vb = wolf::BufferManager::CreateVertexBuffer(s_triangleVertices, sizeof(Vertex) * 3);

    m_texture = wolf::TextureManager::CreateTexture(texturePath);

    // Material should be unique to a texture
    m_material = wolf::MaterialManager::CreateMaterial(texturePath);
    m_material->SetProgram("data/shaders/2d_screen.vsh", "data/shaders/2d_screen.fsh");
    m_material->SetTexture("u_texture", m_texture);

    m_decl = new wolf::VertexDeclaration();
    m_decl->Begin();
    m_decl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_decl->SetVertexBuffer(m_vb);
    m_decl->End();
}

void Triangle::Update(float dt)
{
}

void Triangle::setPosition(const glm::vec2 &vPosition)
{
    m_vPosition = vPosition;
}

void Triangle::setScale(const glm::vec2 &vScale)
{
    m_vScale = vScale;
}

void Triangle::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    // m_material->SetUniform("projection", mProj);
    // m_material->SetUniform("view", mView);
    glm::mat4 mWorld = glm::mat4(1.0f);
    mWorld = glm::scale(mWorld, glm::vec3(m_vScale.x, m_vScale.y, 0.0f));
    mWorld = glm::translate(mWorld, glm::vec3(m_vPosition.x, m_vPosition.y, 0.0f));
    m_material->SetUniform("world", mWorld);
    m_material->Apply();

    m_texture->Bind();

    m_decl->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}