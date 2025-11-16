#pragma once
#define _USE_MATH_DEFINES
#include "../obj/node.h"
#include "../../wolf/wolf.h"
#include "../misc/util.h"
#include <random>
#include <cmath>
#include <array>   // Include for using std::array
#include <unordered_map>

// Use constexpr for values that don't change
constexpr float AMPLITUDE = 300.0f;
constexpr int SIZE = 400;
constexpr int NUM_VERTICES = 128;
constexpr int OCTAVES = 7;
constexpr float ROUGHNESS = 0.5f;
constexpr int GRID_OFFSET = 0;
constexpr float SCALE = 120.0f;

struct TerrainVertex
{
	GLfloat x, y, z;    // Position
	GLfloat u, v;       // Texture Coordinates
	GLfloat nX, nY, nZ; // Normals
	GLfloat tX, tY, tZ;
	GLfloat btX, btY, btZ;
};

constexpr size_t hashCombine(size_t t, size_t u)
{
	return t ^ (0x517cc1b727220a95 + ((t << 2) ^ (u >> 3)));
}
namespace std
{
	template <>
	struct hash<std::pair<int, int>>
	{
		size_t operator()(const std::pair<int, int>& object) const
		{
			return hashCombine(std::hash<int>()(object.first), std::hash<int>()(object.second));
		}
	};
}

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator() = default;

	wolf::VertexDeclaration* GenerateVertices(int gridX, int gridZ);

	// Setters
	void SetSize(int size);
	void SetVertexCount(int count);
	void SetAmplitude(float amplitude);
	void SetOctaves(int octaves);
	void SetRoughness(float roughness);

	void SetBaseMult(float val) { m_baseMult = val; }
	void SetValleyMult(float val) { m_valleyMult = val; }
	void SetLowAreaSmoothFactor(float val) { m_lowAreaSmoothFactor = val; }
	void SetLowAreaThreshold(float val) { m_lowAreaThreshold = val; }
	void SetValleyThreshold(float val) { m_valleyThreshold = val; }
	void SetValleyDepth(float val) { m_valleyDepth = val; }

	void SetSmoothEdge0(float val) { m_smoothEdge0 = val; }
	void SetSmoothEdge1(float val) { m_smoothEdge1 = val; }

	void ClearHeightCache() { m_heightCache.clear(); }

	// Getters
	float GetHeight(int x, int z) const { return m_heightCache[std::pair<int, int>(x, z)]; }
	int GetSize() const;
	int GetVertexCount() const;
	float GetAmplitude() const;
	int GetOctaves() const;
	float GetRoughness() const;
	const std::vector<TerrainVertex>& getVertices() const;
	const std::vector<float>& getRawVertices() const;
	BoundingBox GetBounds(int gridX, int gridZ) const;
	float _fBM(float x, float z) const;
	float _generateHeight(float x, float z, int xOff, int zOff) const;

private:
	glm::vec3 _calculateNormal(float x, float z, int xOff, int zOff) const;
	double _getNoise(double x, double z) const;

	// Move utility functions to header for inlining
	inline double _fade(double t) const
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	inline double _lerp(double t, double a, double b) const
	{
		return a + t * (b - a);
	}

	inline double _grad(int hash, double x, double z) const;

	inline float _smootherstep(float edge0, float edge1, float x) const
	{
		// Scale, bias and saturate x to 0..1 range
		x = wolf::max(0.0f, wolf::min((x - edge0) / (edge1 - edge0), 1.0f));
		// Evaluate polynomial
		return x * x * (3 - 2 * x);
	}

	int m_size = SIZE, m_countVerts = NUM_VERTICES;
	int m_octaves = OCTAVES;
	float m_amplitude = AMPLITUDE, m_roughness = ROUGHNESS;

	float m_baseMult = 2.7f;
	float m_valleyMult = 3.5f, m_valleyThreshold = -0.325f, m_valleyDepth = 0.445f;
	float m_lowAreaSmoothFactor = 0.098f, m_lowAreaThreshold = 0.0f;

	float m_smoothEdge0 = 0.4f, m_smoothEdge1 = 0.6f;

	int m_seed;
	float m_seedOffset;

	std::vector<TerrainVertex> m_vertices;
	std::vector<float> m_rawVertices;

	mutable std::unordered_map<std::pair<int, int>, float> m_heightCache;
};