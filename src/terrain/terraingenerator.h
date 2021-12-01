#pragma once
#include "../obj/shape.h"
#include "../../wolf/wolf.h"
#include <random>
#include <cmath>

#ifndef AMPLITUDE
#define AMPLITUDE 70.0f
#endif

#ifndef SIZE
#define SIZE 800
#endif

#ifndef NUM_VERTICES
#define NUM_VERTICES 128
#endif

#ifndef OCTAVES
#define OCTAVES 3
#endif

#ifndef ROUGHNESS
#define ROUGHNESS 0.3f
#endif

class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    float GenerateHeight(int x, int y);
    wolf::VertexDeclaration *GenerateVertices();

    void SetSize(int size);
    void SetVertexCount(int count);
    void SetAmplitude(float amplitude);
    void SetOctaves(int octaves);
    void SetRoughness(float roughness);

    int GetSize() const;
    int GetVertexCount() const;
    float GetAmplitude() const;
    int GetOctaves() const;
    float GetRoughness() const;

private:
    float _generateHeight(int x, int z);
    glm::vec3 _calculateNormal(int x, int z);

    float _interpolate(float a, float b, float blend);

    float _getNoise(int x, int z);
    float _getSmoothNoise(int x, int z);
    float _getInterpolatedNoise(float x, float z);

    int m_size = SIZE, m_countVerts = NUM_VERTICES;
    int m_octaves = OCTAVES;
    float m_amplitude = AMPLITUDE, m_roughness = ROUGHNESS;
    int m_seed;
};