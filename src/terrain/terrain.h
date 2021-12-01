#pragma once
#include "../../wolf/wolf.h"
#include "../obj/shape.h"
#include "terraingenerator.h"

class Terrain
{
public:
    Terrain(int x, int z, TerrainGenerator* terrainGenerator);
    ~Terrain();

    void Render(const glm::mat4 &mProj, const glm::mat4 &mView);
private:
    int m_x, m_z;

    TerrainGenerator *m_terrainGenerator = nullptr;

    wolf::Texture *m_texture = nullptr;
    wolf::Program *m_program = nullptr;
    wolf::VertexBuffer * m_vb;
    wolf::VertexDeclaration * m_decl;
};