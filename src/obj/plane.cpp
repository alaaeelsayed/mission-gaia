#include "plane.h"

const Vertex Plane::s_planeVertices[6] = {
    {-0.5f, 0.0f, 0.5f, 0, 1, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.0f, 0.5f, 1, 1, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.0f, -0.5f, 1, 0, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.0f, -0.5f, 1, 0, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.0f, -0.5f, 0, 0, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.0f, 0.5f, 0, 1, 0.0f, 1.0f, 0.0f},
};

Plane::~Plane()
{
    delete m_decl;
    wolf::BufferManager::DestroyBuffer(m_vb);
    wolf::TextureManager::DestroyTexture(m_texture);
    wolf::ProgramManager::DestroyProgram(m_program);
}

Plane::Plane(const std::string &texturePath)
    : Node(BoundingBox())
{
    m_vb = wolf::BufferManager::CreateVertexBuffer(s_planeVertices, sizeof(Vertex) * 6);
    m_texture = wolf::TextureManager::CreateTexture(texturePath);

    // Material should be unique to a texture
    // m_material = wolf::MaterialManager::CreateMaterial(texturePath);
    // m_material->SetProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
    // m_material->SetTexture("u_texture", m_texture);
    m_program = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
    m_program->SetTexture("u_texture", m_texture);

    m_decl = new wolf::VertexDeclaration();
    m_decl->Begin();
    m_decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
    m_decl->SetVertexBuffer(m_vb);
    m_decl->End();
}

Plane::Plane(wolf::Program *program, int subdivisions) : Node(BoundingBox())
{
    m_numSegments = subdivisions * subdivisions;
    Vertex *pAllVerts = new Vertex[m_numSegments * 6];
    float curx = -0.5f;
    float y = 0;
    float curz = -0.5f;
    float quadSize = 1.0f / subdivisions;
    int vertIdx = 0;

    int count = 0;

    for (int z = 0; z < subdivisions; ++z)
    {
        for (int x = 0; x < subdivisions; ++x)
        {

            // m_fTextureCoords[count] = currX;
            // m_fTextureCoords[count + 1] = currY + quadSize;
            // m_fTextureCoords[count + 2] = currX;
            // m_fTextureCoords[count + 3] = currY;

            for (int i = 0; i < 6; ++i)
            {
                pAllVerts[vertIdx].x = (glm::vec3(curx, 0.0f, curz) + glm::vec3(s_planeVertices[i].x, s_planeVertices[i].y, s_planeVertices[i].z) * quadSize).x;
                pAllVerts[vertIdx].y = (glm::vec3(curx, 0.0f, curz) + glm::vec3(s_planeVertices[i].x, s_planeVertices[i].y, s_planeVertices[i].z) * quadSize).y;
                pAllVerts[vertIdx].z = (glm::vec3(curx, 0.0f, curz) + glm::vec3(s_planeVertices[i].x, s_planeVertices[i].y, s_planeVertices[i].z) * quadSize).z;

                if (i == 0 || i == 5)
                {
                    pAllVerts[vertIdx].u = x * quadSize;
                    pAllVerts[vertIdx].v = z * quadSize + quadSize;
                }
                else if (i == 1)
                {
                    pAllVerts[vertIdx].u = x * quadSize + quadSize;
                    pAllVerts[vertIdx].v = z * quadSize + quadSize;
                }
                else if (i == 2 || i == 3)
                {
                    pAllVerts[vertIdx].u = x * quadSize + quadSize;
                    pAllVerts[vertIdx].v = z * quadSize;
                }
                else if (i == 4)
                {
                    pAllVerts[vertIdx].u = x * quadSize;
                    pAllVerts[vertIdx].v = z * quadSize;
                }

                pAllVerts[vertIdx].nX = s_planeVertices[i].nX;
                pAllVerts[vertIdx].nY = s_planeVertices[i].nY;
                pAllVerts[vertIdx].nZ = s_planeVertices[i].nZ;
                vertIdx++;
            }
            curx += quadSize;
        }
        curz += quadSize;
        curx = -0.5f;
    }

    m_vb = wolf::BufferManager::CreateVertexBuffer(pAllVerts, sizeof(Vertex) * 6 * m_numSegments);

    // Material should be unique to a texture
    // m_material = wolf::MaterialManager::CreateMaterial("water");
    // m_material->SetProgram("data/shaders/water/water.vsh", "data/shaders/water/water.fsh");
    // m_material->SetTexture("u_texture", m_texture);
    m_program = program;

    m_decl = new wolf::VertexDeclaration();
    m_decl->Begin();
    m_decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
    m_decl->SetVertexBuffer(m_vb);
    m_decl->End();

    delete[] pAllVerts;
}

void Plane::Update(float dt)
{
}

void Plane::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    m_program->SetUniform("projection", mProj);
    m_program->SetUniform("view", mView);
    m_program->SetUniform("world", GetWorldTransform());
    m_program->SetUniform("worldIT", glm::transpose(glm::inverse(GetWorldTransform())));

    m_program->Bind();

    if (m_texture)
        m_texture->Bind();

    m_decl->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 6 * m_numSegments);
}
