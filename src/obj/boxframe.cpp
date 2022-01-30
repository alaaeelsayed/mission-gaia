#include "boxframe.h"
#include "node.h"

BoxFrame::BoxFrame(const BoundingBox &box)
    : m_box(box)
{
    m_program = wolf::ProgramManager().CreateProgram("data/shaders/plain.vsh", "data/shaders/plain.fsh");

    glm::vec3 min = box.GetMin();
    glm::vec3 max = box.GetMax();

    /*
    MINX, MAXZ          MAXX, MAXZ
              
    MINX, MINZ          MAXX, MINZ
    */
    Vertex verts[2 * 12] = {
        // {min.x, 0.0f, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {min.x, 0.0f, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {max.x, 0.0f, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {min.x, 0.0f, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {max.x, 0.0f, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {max.x, 0.0f, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {max.x, 0.0f, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        // {min.x, 0.0f, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, min.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, min.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, min.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, min.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, max.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, max.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, max.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, max.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, min.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, min.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, max.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, max.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, min.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, min.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, max.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, max.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, min.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, max.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, min.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {min.x, max.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, min.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, max.y, min.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, min.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
        {max.x, max.y, max.z, 1.0f, 1.0f, 1.0f, 1.0f},
    };

    m_vb = wolf::BufferManager::CreateVertexBuffer(verts, sizeof(Vertex) * 24);

    m_decl = new wolf::VertexDeclaration();
    m_decl->Begin();
    m_decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_decl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);
    m_decl->SetVertexBuffer(m_vb);
    m_decl->End();
}

BoxFrame::~BoxFrame()
{
    delete m_decl;
    wolf::BufferManager::DestroyBuffer(m_vb);
}

void BoxFrame::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    glm::mat4 mWorld(1.0f);

    m_program->SetUniform("projection", mProj);
    m_program->SetUniform("view", mView);
    m_program->SetUniform("world", mWorld);

    m_program->Bind();

    m_decl->Bind();

    glDrawArrays(GL_LINES, 0, 24);
}
