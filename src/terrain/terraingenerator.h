#pragma once
#define _USE_MATH_DEFINES
#include "../obj/shape.h"
#include "../../wolf/wolf.h"
#include <random>
#include <cmath>

#ifndef AMPLITUDE
#define AMPLITUDE 200.0f
#endif

#ifndef SIZE
#define SIZE 800
#endif

#ifndef NUM_VERTICES
#define NUM_VERTICES 128
#endif

#ifndef OCTAVES
#define OCTAVES 7
#endif

#ifndef ROUGHNESS
#define ROUGHNESS 0.3f
#endif

class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    float GenerateHeight(int x, int z, int xOff, int zOff);
    wolf::VertexDeclaration *GenerateVertices(int gridX, int gridZ);

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
    float _generateHeight(int x, int z, int xOff, int zOff);
    glm::vec3 _calculateNormal(int x, int z, int xOff, int zOff);

    float _interpolate(float a, float b, float blend);

    float _getInterpolatedNoise(float x, float z);
    float _getSmoothNoise(float x, float z);
    float _getNormalNoise(float x, float z);
    float _getNoise(float x, float z);
    float _fade(float t);
    float _lerp(float t, float a, float b);
    float _grad(int xCorner, int zCorner, float xDist, float zDist);
    int _hash(int x, int z);

    int m_size = SIZE, m_countVerts = NUM_VERTICES;
    int m_octaves = OCTAVES;
    float m_amplitude = AMPLITUDE, m_roughness = ROUGHNESS;
    int m_seed;
};