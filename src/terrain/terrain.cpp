#include "terrain.h"

Terrain::Terrain(int x, int z, TerrainGenerator *terrainGenerator) : Node(BoundingBox())
{
    m_x = x * terrainGenerator->GetSize();
    m_z = z * terrainGenerator->GetSize();

    SetPos(glm::vec3(m_x, 0.0f, m_z));

    m_texture = wolf::TextureManager::CreateTexture("data/textures/ground/ground.png");

    m_texture->SetWrapMode(wolf::Texture::WM_Repeat);

    m_program = wolf::ProgramManager::CreateProgram("data/shaders/terrain.vsh", "data/shaders/terrain.fsh");

    m_decl = terrainGenerator->GenerateVertices(x, z);
    // float *data = &terrainGenerator->getHeights()[0];
    // int length = terrainGenerator->getHeights().size();
    // m_rigidBody = new RigidBody(terrainGenerator->GetVertexCount(), terrainGenerator->GetVertexCount(), data, 1, -3, 3, 1, PHY_FLOAT, false);
    m_terrainGenerator = terrainGenerator;

    GetBoundingBox().Extend(terrainGenerator->GetBounds(x, z));
}

Terrain::~Terrain()
{
    wolf::ProgramManager::DestroyProgram(m_program);
    m_decl->Clean();
    delete m_decl;
    // delete m_rigidBody;
}

void Terrain::Update(float dt)
{
    // m_rigidBody->Update(dt, glm::vec3(m_x, 0.0f, m_z));
}

void Terrain::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    glm::mat4 mWorld = glm::mat4(1.0f);
    mWorld = glm::translate(mWorld, glm::vec3(m_x, 0.0f, m_z));

    m_program->SetUniform("projection", mProj);
    m_program->SetUniform("view", mView);
    m_program->SetUniform("world", mWorld);
    m_program->SetUniform("worldIT", glm::transpose(glm::inverse(mWorld)));

    wolf::Texture *dirtTexture = wolf::TextureManager::CreateTexture("data/textures/ground/dirt1.png");
    wolf::Texture *grassTexture = wolf::TextureManager::CreateTexture("data/textures/ground/grass4.png");
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

    m_program->SetUniform("u_lightPos", Scene::Instance()->GetLightDirection());
    m_program->SetUniform("u_viewPos", glm::vec3(0.0f, 0.0f, 0.0f));
    // 158, 108, 56
    m_program->SetUniform("u_ambientLight", glm::vec3(0.3f, 0.21f, 0.11f));
    m_program->SetTexture("u_texture", m_texture);

    m_program->Bind();
    m_decl->Bind();

    glDrawElements(GL_TRIANGLES,
                   6 * ((m_terrainGenerator->GetVertexCount() - 1) * (m_terrainGenerator->GetVertexCount() - 1)),
                   GL_UNSIGNED_INT, 0);
}
