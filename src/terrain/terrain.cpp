#include "terrain.h"

Terrain::Terrain(int x, int z, TerrainGenerator *terrainGenerator)
{
    m_x = x * terrainGenerator->GetSize();
    m_z = z * terrainGenerator->GetSize();

    m_texture = wolf::TextureManager::CreateTexture("data/textures/ground/ground.png");

    m_texture->SetWrapMode(wolf::Texture::WM_Repeat);

    m_program = wolf::ProgramManager::CreateProgram("data/shaders/terrain.vsh", "data/shaders/terrain.fsh");

    m_decl = terrainGenerator->GenerateVertices(x, z);

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

    m_program->SetUniform("projection", mProj);
    m_program->SetUniform("view", mView);
    m_program->SetUniform("world", mWorld);
    m_program->SetUniform("worldIT", glm::transpose(glm::inverse(mWorld)));

    wolf::Texture *dirtTexture = wolf::TextureManager::CreateTexture("data/textures/ground/dirt.png");
    wolf::Texture *grassTexture = wolf::TextureManager::CreateTexture("data/textures/ground/grass.png");
    wolf::Texture *rockTexture = wolf::TextureManager::CreateTexture("data/textures/ground/rock.png");
    wolf::Texture *snowTexture = wolf::TextureManager::CreateTexture("data/textures/ground/snow.png");

    m_program->SetTexture("region1ColorMap", dirtTexture);
    m_program->SetTexture("region2ColorMap", grassTexture);
    m_program->SetTexture("region3ColorMap", rockTexture);
    m_program->SetTexture("region4ColorMap", snowTexture);

    m_program->SetUniform("region1.min", 2 * -140.0f);
    m_program->SetUniform("region1.max", 2 * -80.0f);
    m_program->SetUniform("region2.min", 2 * -79.0f);
    m_program->SetUniform("region2.max", 2 * -29.0f);
    m_program->SetUniform("region3.min", 2 * -28.0f);
    m_program->SetUniform("region3.max", 2 * 22.0f);
    m_program->SetUniform("region4.min", 2 * 23.0f);
    m_program->SetUniform("region4.max", 2 * 73.0f);

    m_program->SetUniform("u_lightPos", glm::vec3(-4000, 3000, 4000));
    // 158, 108, 56
    m_program->SetUniform("u_ambientLight", glm::vec3(0.3f, 0.21f, 0.11f));
    m_program->SetTexture("u_texture", m_texture);

    m_program->Bind();
    m_decl->Bind();

    glDrawElements(GL_TRIANGLES,
                   6 * ((m_terrainGenerator->GetVertexCount() - 1) * (m_terrainGenerator->GetVertexCount() - 1)),
                   GL_UNSIGNED_INT, 0);
}