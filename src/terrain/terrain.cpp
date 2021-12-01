#include "terrain.h"

Terrain::Terrain(int x, int z, TerrainGenerator *terrainGenerator)
{
    m_x = x * terrainGenerator->GetSize();
    m_z = z * terrainGenerator->GetSize();

    m_texture = wolf::TextureManager::CreateTexture("data/textures/ground/ground.png");

    m_texture->SetWrapMode(wolf::Texture::WM_Repeat);

    m_program = wolf::ProgramManager::CreateProgram("data/shaders/terrain.vsh", "data/shaders/terrain.fsh");

    m_decl = terrainGenerator->GenerateVertices();

    m_terrainGenerator = terrainGenerator;
}

Terrain::~Terrain()
{
    wolf::ProgramManager::DestroyProgram(m_program);
    m_decl->Clean();
    delete m_decl;
}

void Terrain::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    glm::mat4 mWorld = glm::mat4(1.0f);
    mWorld = glm::translate(mWorld, glm::vec3(m_x, 0.0f, m_z));
    mWorld = glm::rotate(mWorld, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_program->SetUniform("projection", mProj);
    m_program->SetUniform("view", mView);
    m_program->SetUniform("world", mWorld);

    // m_program->SetUniform("u_color", glm::vec4(0.1f, 0.4f, 0.1f, 1.0f));
    m_program->SetTexture("u_texture", m_texture);

    m_program->Bind();
    m_decl->Bind();

    glDrawElements(GL_TRIANGLES,
                   6 * ((m_terrainGenerator->GetVertexCount() - 1) * (m_terrainGenerator->GetVertexCount() - 1)),
                   GL_UNSIGNED_INT, 0);
}
