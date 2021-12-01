#include "terraingenerator.h"

TerrainGenerator::TerrainGenerator()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 1000000000);

	m_seed = dist(mt);
}

TerrainGenerator::~TerrainGenerator()
{
}

float TerrainGenerator::GenerateHeight(int x, int y)
{
	return _generateHeight(x, y);
}

void TerrainGenerator::SetSize(int size)
{
	m_size = size;
}

void TerrainGenerator::SetVertexCount(int count)
{
	m_countVerts = count;
}

void TerrainGenerator::SetAmplitude(float amplitude)
{
	m_amplitude = amplitude;
}

void TerrainGenerator::SetOctaves(int octaves)
{
	m_octaves = octaves;
}

void TerrainGenerator::SetRoughness(float roughness)
{
	m_roughness = roughness;
}

int TerrainGenerator::GetSize() const
{
	return m_size;
}

int TerrainGenerator::GetVertexCount() const
{
	return m_countVerts;
}

float TerrainGenerator::GetAmplitude() const
{
	return m_amplitude;
}

int TerrainGenerator::GetOctaves() const
{
	return m_octaves;
}

float TerrainGenerator::GetRoughness() const
{
	return m_roughness;
}

wolf::VertexDeclaration *TerrainGenerator::GenerateVertices()
{
	int count = m_countVerts * m_countVerts;

	std::vector<Vertex> vertices(count);

	int vertexOffset = 0;
	for (int i = 0; i < m_countVerts; i++)
	{
		for (int j = 0; j < m_countVerts; j++)
		{
			float height = _generateHeight(i, j);
			glm::vec3 normal = _calculateNormal(i, j);
			vertices[vertexOffset] = {
				(float)j / ((float)m_countVerts - 1) * m_size,
				height,
				(float)i / ((float)m_countVerts - 1) * m_size,
				(float)j / ((float)m_countVerts - 1),
				(float)i / ((float)m_countVerts - 1),
				normal.x,
				normal.y,
				normal.z};
			vertexOffset++;
		}
	}

	std::vector<GLuint> indices(6 * (m_countVerts - 1) * (m_countVerts - 1));

	int counter = 0;
	for (int i = 0; i < m_countVerts - 1; i++)
	{
		for (int j = 0; j < m_countVerts - 1; j++)
		{
			int topLeft = (i * m_countVerts) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * m_countVerts) + j;
			int bottomRight = bottomLeft + 1;
			indices[counter++] = topLeft;
			indices[counter++] = bottomLeft;
			indices[counter++] = topRight;
			indices[counter++] = topRight;
			indices[counter++] = bottomLeft;
			indices[counter++] = bottomRight;
		}
	}

	wolf::VertexBuffer *vertexBuffer = wolf::BufferManager::CreateVertexBuffer(&vertices[0], sizeof(Vertex) * vertices.size());
	wolf::IndexBuffer *indexBuffer = wolf::BufferManager::CreateIndexBuffer(&indices[0], sizeof(GLuint) * counter);

	wolf::VertexDeclaration *vertexDeclaration = new wolf::VertexDeclaration();

	vertexDeclaration->Begin();
	vertexDeclaration->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	vertexDeclaration->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	vertexDeclaration->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
	vertexDeclaration->SetVertexBuffer(vertexBuffer);
	vertexDeclaration->SetIndexBuffer(indexBuffer);
	vertexDeclaration->End();

	return vertexDeclaration;
}

glm::vec3 TerrainGenerator::_calculateNormal(int x, int z)
{
	float heightLeft = _generateHeight(x - 1, z);
	float heightRight = _generateHeight(x + 1, z);
	float heightFront = _generateHeight(x, z - 1);
	float heightBack = _generateHeight(x, z + 1);
	glm::vec3 normal = glm::vec3(heightLeft - heightRight, 2.0f, heightBack - heightFront);
	return glm::normalize(normal);
}

float TerrainGenerator::_generateHeight(int x, int z)
{
	float total = 0;
	float d = (float)pow(2, m_octaves - 1);
	for (int i = 0; i < m_octaves; i++)
	{
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(m_roughness, i) * m_amplitude;
		total += _getInterpolatedNoise(x * freq, z * freq) * amp;
	}
	return total;
}

float TerrainGenerator::_getInterpolatedNoise(float x, float z)
{
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = _getSmoothNoise(intX, intZ);
	float v2 = _getSmoothNoise(intX + 1, intZ);
	float v3 = _getSmoothNoise(intX, intZ + 1);
	float v4 = _getSmoothNoise(intX + 1, intZ + 1);
	float i1 = _interpolate(v1, v2, fracX);
	float i2 = _interpolate(v3, v4, fracX);
	return _interpolate(i1, i2, fracZ);
}

// Cosine interpolation
float TerrainGenerator::_interpolate(float a, float b, float blend)
{
	double theta = blend * PI;
	float f = (float)(1.0f - cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float TerrainGenerator::_getSmoothNoise(int x, int z)
{
	float corners = (_getNoise(x - 1, z - 1) + _getNoise(x + 1, z - 1) + _getNoise(x - 1, z + 1) + _getNoise(x + 1, z + 1)) / 16.0f;
	float sides = (_getNoise(x - 1, z) + _getNoise(x + 1, z) + _getNoise(x, z - 1) + _getNoise(x, z + 1)) / 8.0f;
	float center = _getNoise(x, z) / 4.0f;
	return corners + sides + center;
}

float TerrainGenerator::_getNoise(int x, int z)
{
	int n = x + z * m_seed;
	n = (n << 13) ^ n;
	return (1.0f - ((n * (n * n * 15371 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
