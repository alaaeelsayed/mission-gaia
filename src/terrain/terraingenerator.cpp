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

float TerrainGenerator::GetHeight(int x, int z)
{
	int terrainX = x % m_size;
	int terrainZ = z % m_size;

	float gridSize = m_size / ((float)m_countVerts - 1);

	int gridX = floor(terrainX / gridSize);
	int gridZ = floor(terrainZ / gridSize);

	float accX = (fmod(terrainX, gridSize)) / gridSize;
	float accZ = (fmod(terrainZ, gridSize)) / gridSize;

	int xOff = x / m_size;
	int zOff = z / m_size;

	xOff *= (m_countVerts - 1);
	zOff *= (m_countVerts - 1);

	float height;
	if (accX <= (1 - accZ))
	{
		height = Util::barryCentric(glm::vec3(0, _generateHeight(gridX, gridZ, xOff, zOff), 0),
									glm::vec3(1, _generateHeight(gridX + 1, gridZ, xOff, zOff), 0),
									glm::vec3(0, _generateHeight(gridX, gridZ + 1, xOff, zOff), 1),
									glm::vec2(accX, accZ));
	}
	else
	{
		height = Util::barryCentric(glm::vec3(1, _generateHeight(gridX + 1, gridZ, xOff, zOff), 0),
									glm::vec3(1, _generateHeight(gridX + 1, gridZ + 1, xOff, zOff), 1),
									glm::vec3(0, _generateHeight(gridX, gridZ + 1, xOff, zOff), 1),
									glm::vec2(accX, accZ));
	}
	return height;
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

wolf::VertexDeclaration *TerrainGenerator::GenerateVertices(int gridX, int gridZ)
{
	int xOffset = gridX * (m_countVerts - 1);
	int zOffset = gridZ * (m_countVerts - 1);

	int count = m_countVerts * m_countVerts;

	std::vector<Vertex> vertices(count);

	int vertexOffset = 0;
	for (int i = 0; i < m_countVerts; i++)
	{
		for (int j = 0; j < m_countVerts; j++)
		{
			float height = _generateHeight(j, i, xOffset, zOffset);
			glm::vec3 normal = _calculateNormal(j, i, xOffset, zOffset);
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

glm::vec3 TerrainGenerator::_calculateNormal(int x, int z, int xOff, int zOff)
{
	float heightLeft = _generateHeight(x - 1, z, xOff, zOff);
	float heightRight = _generateHeight(x + 1, z, xOff, zOff);
	float heightFront = _generateHeight(x, z - 1, xOff, zOff);
	float heightBack = _generateHeight(x, z + 1, xOff, zOff);
	glm::vec3 normal = glm::vec3(heightLeft - heightRight, 2.0f, heightBack - heightFront);
	return glm::normalize(normal);
}

float TerrainGenerator::_generateHeight(int x, int z, int xOff, int zOff)
{
	float total = 0;
	float d = (float)pow(2, m_octaves - 1);
	for (int i = 0; i < m_octaves; i++)
	{
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(m_roughness, i) * m_amplitude;
		total += _getNormalNoise((x + xOff) * freq, (z + zOff) * freq) * amp;
	}
	return total;
}

// float TerrainGenerator::_getInterpolatedNoise(float x, float z)
// {
// 	int intX = (int)x;
// 	int intZ = (int)z;
// 	float fracX = x - intX;
// 	float fracZ = z - intZ;

// 	float v1 = _getSmoothNoise(intX, intZ);
// 	float v2 = _getSmoothNoise(intX + 1, intZ);
// 	float v3 = _getSmoothNoise(intX, intZ + 1);
// 	float v4 = _getSmoothNoise(intX + 1, intZ + 1);
// 	float i1 = _interpolate(v1, v2, fracX);
// 	float i2 = _interpolate(v3, v4, fracX);
// 	return _interpolate(i1, i2, fracZ);
// }

// Cosine interpolation
// float TerrainGenerator::_interpolate(float a, float b, float blend)
// {
// 	double theta = blend * PI;
// 	float f = (float)(1.0f - cos(theta)) * 0.5f;
// 	return a * (1.0f - f) + b * f;
// }

// float TerrainGenerator::_getSmoothNoise(int x, int z)
// {
// 	float corners = (_getNoise(x - 1, z - 1) + _getNoise(x + 1, z - 1) + _getNoise(x - 1, z + 1) + _getNoise(x + 1, z + 1)) / 16.0f;
// 	float sides = (_getNoise(x - 1, z) + _getNoise(x + 1, z) + _getNoise(x, z - 1) + _getNoise(x, z + 1)) / 8.0f;
// 	float center = _getNoise(x, z) / 4.0f;
// 	return corners + sides + center;
// }

float TerrainGenerator::_getNormalNoise(float x, float z)
{
	return _getNoise(x, z) * M_SQRT2;
}

float TerrainGenerator::_getNoise(float x, float z)
{
	int xFloor = static_cast<int>(x);
	int zFloor = static_cast<int>(z);

	float xFrac = x - xFloor;
	float zFrac = z - zFloor;

	float grad1 = _grad(xFloor, zFloor, xFrac, zFrac);
	float grad2 = _grad(xFloor + 1, zFloor, xFrac - 1.0, zFrac);
	float grad3 = _grad(xFloor, zFloor + 1, xFrac, zFrac - 1.0);
	float grad4 = _grad(xFloor + 1, zFloor + 1, xFrac - 1.0, zFrac - 1.0);

	float xFade = _fade(xFrac);
	float zFade = _fade(zFrac);

	return _lerp(_lerp(grad1, grad2, xFade),
				 _lerp(grad3, grad4, xFade), zFade);
}

float TerrainGenerator::_fade(float t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float TerrainGenerator::_lerp(float a, float b, float t)
{
	return (1.0 - t) * a + t * b;
}

float TerrainGenerator::_grad(int xCorner, int zCorner, float xDist, float zDist)
{
	double cosPi4 = std::cos(M_PI / 4.0);
	double cosPi8 = std::cos(M_PI / 8.0);
	double sinPi8 = std::sin(M_PI / 8.0);

	switch (_hash(xCorner, zCorner) & 0xf)
	{
	case 0x0:
		return +xDist;
	case 0x1:
		return +zDist;
	case 0x2:
		return -xDist;
	case 0x3:
		return -zDist;
	case 0x4:
		return +xDist * cosPi4 + zDist * cosPi4;
	case 0x5:
		return -xDist * cosPi4 + zDist * cosPi4;
	case 0x6:
		return -xDist * cosPi4 - zDist * cosPi4;
	case 0x7:
		return +xDist * cosPi4 - zDist * cosPi4;
	case 0x8:
		return +xDist * cosPi8 + zDist * sinPi8;
	case 0x9:
		return +xDist * sinPi8 + zDist * cosPi8;
	case 0xa:
		return -xDist * sinPi8 + zDist * cosPi8;
	case 0xb:
		return -xDist * cosPi8 + zDist * sinPi8;
	case 0xc:
		return -xDist * cosPi8 - zDist * sinPi8;
	case 0xd:
		return -xDist * sinPi8 - zDist * cosPi8;
	case 0xe:
		return +xDist * sinPi8 - zDist * cosPi8;
	case 0xf:
		return +xDist * cosPi8 - zDist * sinPi8;
	default:
		return 0.0;
	}
}

int TerrainGenerator::_hash(int x, int z)
{
	x = (x << 16) | (z & 0xffff);

	x ^= x >> 16;
	x *= m_seed;
	x ^= x >> 16;
	x *= m_seed;
	x ^= x >> 16;
	return x;
}
