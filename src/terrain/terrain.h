#pragma once
#include "../../wolf/wolf.h"
#include "../obj/node.h"
#include "terraingenerator.h"
#include "../text/textbox.h"

class Terrain : public Node
{
public:
    Terrain(int x, int z, TerrainGenerator *terrainGenerator);
    ~Terrain();

    void Update(float dt) override;
    void Render(const glm::mat4 &mProj, const glm::mat4 &mView) override;

private:
    int m_x, m_z;

    TerrainGenerator *m_terrainGenerator = nullptr;

    wolf::Texture *m_texture = nullptr;
    wolf::Program *m_program = nullptr;
    wolf::VertexBuffer *m_vb;
    wolf::VertexDeclaration *m_decl;
};