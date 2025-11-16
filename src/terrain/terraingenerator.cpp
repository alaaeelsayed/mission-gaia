#include "terraingenerator.h"

TerrainGenerator::TerrainGenerator()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
	m_seed = dist(mt);
	float hashed = sin(m_seed + 1.0f) * 10000.0f;
	m_seedOffset = (hashed - floor(hashed)) * 2.0f - 1.0f;

	m_vertices.reserve(m_countVerts * m_countVerts);
	m_rawVertices.reserve((m_countVerts * m_countVerts) * 3);
}

//TerrainGenerator::~TerrainGenerator()
//{
//}

void TerrainGenerator::SetSize(int size)
{
	m_size = size;
}

void TerrainGenerator::SetVertexCount(int count)
{
	m_countVerts = count;
	m_vertices.reserve(m_countVerts * m_countVerts);
	m_rawVertices.reserve((m_countVerts * m_countVerts) * 3);
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

const std::vector<TerrainVertex>& TerrainGenerator::getVertices() const
{
	return m_vertices;
}

const std::vector<float>& TerrainGenerator::getRawVertices() const
{
	return m_rawVertices;
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

BoundingBox TerrainGenerator::GetBounds(int gridX, int gridZ) const
{
	float min = 100000.0f;
	float max = -100000.0f;

	int xOffset = GRID_OFFSET + gridX * (m_countVerts - 1);
	int zOffset = GRID_OFFSET + gridZ * (m_countVerts - 1);

	for (int i = 0; i < m_countVerts; i++)
	{
		for (int j = 0; j < m_countVerts; j++)
		{
			float height = _generateHeight(j - 0.5f, i - 0.5f, xOffset, zOffset);
			if (height < min)
				min = height;
			if (height > max)
				max = height;
		}
	}
	return BoundingBox(glm::vec3(gridX * m_size, min, gridZ * m_size), glm::vec3(gridX * m_size + m_size, max, gridZ * m_size + m_size));
}

wolf::VertexDeclaration* TerrainGenerator::GenerateVertices(int gridX, int gridZ)
{
	int xOffset = GRID_OFFSET + gridX * (m_countVerts - 1);
	int zOffset = GRID_OFFSET + gridZ * (m_countVerts - 1);

	int count = m_countVerts * m_countVerts;

	std::vector<TerrainVertex> vertices(count);
	m_rawVertices.reserve(count * 3);

	int vertexOffset = 0;
	for (int i = 0; i < m_countVerts; i++)
	{
		for (int j = 0; j < m_countVerts; j++)
		{
			float height = _generateHeight(j - 0.5f, i - 0.5f, xOffset, zOffset);
			glm::vec3 normal = _calculateNormal(j - 0.5f, i - 0.5f, xOffset, zOffset);

			glm::vec3 right = glm::vec3(1.0f, _generateHeight(j + 0.5f, i - 0.5f, xOffset, zOffset) - height, 0.0f);
			glm::vec3 back = glm::vec3(0.0f, _generateHeight(j - 0.5f, i + 0.5f, xOffset, zOffset) - height, 1.0f);
			glm::vec3 tangent = glm::normalize(right);
			glm::vec3 bitangent = glm::normalize(back);

			vertices[vertexOffset] = {
				(float)j / ((float)m_countVerts - 1) * m_size,
				height,
				(float)i / ((float)m_countVerts - 1) * m_size,
				((float)j / ((float)m_countVerts - 1)), // U texcoord
				((float)i / ((float)m_countVerts - 1)), // V texcoord
				normal.x,
				normal.y,
				normal.z,
				tangent.x,
				tangent.y,
				tangent.z,
				bitangent.x,
				bitangent.y,
				bitangent.z,
			};

			m_rawVertices.push_back((float)j / ((float)m_countVerts - 1) * m_size);
			m_rawVertices.push_back(height);
			m_rawVertices.push_back((float)i / ((float)m_countVerts - 1) * m_size);
			vertexOffset++;
		}
	}

	// 4 for quad shape
	std::vector<GLuint> indices(4 * (m_countVerts - 1) * (m_countVerts - 1));

	int counter = 0;
	for (int i = 0; i < m_countVerts - 1; i++)
	{
		for (int j = 0; j < m_countVerts - 1; j++)
		{
			// Use quad shape for GL_PATCHES
			int topLeft = (i * m_countVerts) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * m_countVerts) + j;
			int bottomRight = bottomLeft + 1;
			indices[counter++] = topLeft;
			indices[counter++] = bottomLeft;
			indices[counter++] = bottomRight;
			indices[counter++] = topRight;
		}
	}

	wolf::VertexBuffer* vertexBuffer = wolf::BufferManager::CreateVertexBuffer(&vertices[0], sizeof(TerrainVertex) * vertices.size());
	wolf::IndexBuffer* indexBuffer = wolf::BufferManager::CreateIndexBuffer(&indices[0], sizeof(GLuint) * counter);

	int length = m_rawVertices.size();

	wolf::VertexDeclaration* vertexDeclaration = new wolf::VertexDeclaration();

	vertexDeclaration->Begin();
	vertexDeclaration->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	vertexDeclaration->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	vertexDeclaration->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
	vertexDeclaration->AppendAttribute(wolf::AT_Tangent, 3, wolf::CT_Float);
	vertexDeclaration->AppendAttribute(wolf::AT_BiTangent, 3, wolf::CT_Float);
	vertexDeclaration->SetVertexBuffer(vertexBuffer);
	vertexDeclaration->SetIndexBuffer(indexBuffer);
	vertexDeclaration->End();

	m_vertices = vertices;
	return vertexDeclaration;
}

glm::vec3 TerrainGenerator::_calculateNormal(float x, float z, int xOff, int zOff) const
{
	float heightLeft = _generateHeight(x - 1, z, xOff, zOff);
	float heightRight = _generateHeight(x + 1, z, xOff, zOff);
	float heightFront = _generateHeight(x, z - 1, xOff, zOff);
	float heightBack = _generateHeight(x, z + 1, xOff, zOff);
	glm::vec3 normal = glm::vec3(heightLeft - heightRight, 2.0f, heightBack - heightFront);
	return glm::normalize(normal);
}

float TerrainGenerator::_generateHeight(float x, float z, int xOff, int zOff) const
{
	x += xOff + m_seedOffset;
	z += zOff + m_seedOffset;

	std::pair<int, int> key = std::pair<int, int>((int)(x + 0.5f) - m_seedOffset, (int)(z + 0.5f) - m_seedOffset);

	auto it = m_heightCache.find(key);
	if (it != m_heightCache.end())
	{
		return it->second;
	}

	float height = _fBM(x / (SCALE * m_baseMult), z / (SCALE * m_baseMult));

	/*if (height < m_lowAreaThreshold) {
		height *= m_lowAreaSmoothFactor;
	}*/

	m_heightCache[key] = height * m_amplitude;

	return height * m_amplitude;
}


float TerrainGenerator::_fBM(float x, float z) const
{
	float lacunarity = 2;

	float freq = 1;
	float amp = 1;

	float total = 0;
	for (int i = 0; i < m_octaves; i++)
	{
		total += (_getNoise(x * freq, z * freq) * 2 - 1) * amp;

		freq *= lacunarity;
		amp *= m_roughness;
	}
	return total;
}

int perm[512] = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
		7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
		203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
		20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
		166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
		220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
		63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
		200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
		186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
		126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
		189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
		70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
		98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
		97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
		241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
		199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
		254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
		195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131,
		13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69,
		142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75,
		0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
		165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229,
		122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
		187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
		164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
		123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
		227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119,
		248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172,
		9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
		185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210,
		144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
		107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
		115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
		67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

double TerrainGenerator::_getNoise(double x, double z) const
{
	int X = (int)floor(x) & 255;
	int Z = (int)floor(z) & 255;

	x -= floor(x);
	z -= floor(z);

	int A = perm[X] + Z;
	int B = perm[X + 1] + Z;

	int AA = perm[perm[A]];
	int AB = perm[perm[A + 1]];
	int BA = perm[perm[B]];
	int BB = perm[perm[B + 1]];

	float u = _fade(x);
	float v = _fade(z);

	auto gradAA = _grad(AA, x, z);
	auto gradBA = _grad(BA, x - 1, z);
	auto gradAB = _grad(AB, x, z - 1);
	auto gradBB = _grad(BB, x - 1, z - 1);

	float res = _lerp(v,
		_lerp(u, gradAA, gradBA),
		_lerp(u, gradAB, gradBB));

	res += 0.69f;
	res /= 1.483f;

	return res;
}

double TerrainGenerator::_grad(int hash, double x, double z) const
{
	int h = hash & 15;
	float u = h < 8 ? x : z;
	float v = h < 4 ? z : (h == 12 || h == 14 ? x : 0);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
