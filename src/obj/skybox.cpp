#include "skybox.h"

const Vertex Skybox::s_skyboxVertices[36] = {
    // Front
    {-0.5f, -0.5f, 0.5f, 0.25f, 1 / 3.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 0.25f, 2 / 3.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.5f, 2 / 3.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.5f, 2 / 3.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.5f, 0.5f, 1 / 3.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 0.25f, 1 / 3.0f, 0.0f, 0.0f, 1.0f},

    // Back
    {0.5f, 0.5f, -0.5f, 0.75f, 2 / 3.0f, 0.0f, 0.0f, -1.0f},
    {-0.5f, 0.5f, -0.5f, 1.0f, 2 / 3.0f, 0.0f, 0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f, 1.0f, 1 / 3.0f, 0.0f, 0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f, 1.0f, 1 / 3.0f, 0.0f, 0.0f, -1.0f},
    {0.5f, -0.5f, -0.5f, 0.75f, 1 / 3.0f, 0.0f, 0.0f, -1.0f},
    {0.5f, 0.5f, -0.5f, 0.75f, 2 / 3.0f, 0.0f, 0.0f, -1.0f},

    // Left
    {-0.5f, 0.5f, -0.5f, 0.0f, 2 / 3.0f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, 0.25f, 2 / 3.0f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.25f, 1 / 3.0f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.25f, 1 / 3.0f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 1 / 3.0f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 2 / 3.0f, -1.0f, 0.0f, 0.0f},

    // Right
    {0.5f, 0.5f, 0.5f, 0.5f, 2 / 3.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 0.75f, 2 / 3.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 0.75f, 1 / 3.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 0.75f, 1 / 3.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 0.5f, 1 / 3.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.5f, 2 / 3.0f, 1.0f, 0.0f, 0.0f},

    // Top
    {-0.5f, 0.5f, 0.5f, 0.25f, 2 / 3.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, 0.25f, 1.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.5f, 2 / 3.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, 0.25f, 2 / 3.0f, 0.0f, 1.0f, 0.0f},

    // Bottom
    {-0.5f, -0.5f, 0.5f, 0.25f, 1 / 3.0f, 0.0f, -1.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 0.5f, 1 / 3.0f, 0.0f, -1.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, 0.25f, 0.0f, 0.0f, -1.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.25f, 1 / 3.0f, 0.0f, -1.0f, 0.0f},
};

Skybox::~Skybox()
{
    delete m_decl;
    wolf::BufferManager::DestroyBuffer(m_vb);
    wolf::TextureManager::DestroyTexture(m_texture);
    wolf::MaterialManager::DestroyMaterial(m_material);
}

Skybox::Skybox(const std::string &texturePath) : Node(BoundingBox())
{
    m_vb = wolf::BufferManager::CreateVertexBuffer(s_skyboxVertices, sizeof(Vertex) * 6 * 3 * 2);

    m_texture = wolf::TextureManager::CreateTexture(texturePath);

    // Material should be unique to a texture
    m_material = wolf::MaterialManager::CreateMaterial(texturePath);
    m_material->SetProgram("data/shaders/skybox.vsh", "data/shaders/skybox.fsh");
    m_material->SetTexture("u_texture", m_texture);

    m_decl = new wolf::VertexDeclaration();
    m_decl->Begin();
    m_decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
    m_decl->SetVertexBuffer(m_vb);
    m_decl->End();
}

void Skybox::Update(float dt)
{
}

void Skybox::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    m_material->SetUniform("projection", mProj);
    m_material->SetUniform("view", mView);
    m_material->SetUniform("world", GetWorldTransform());
    m_material->Apply();

    m_texture->Bind();

    m_decl->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);
}
