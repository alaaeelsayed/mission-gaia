#include "terrain.h"

Terrain::Terrain(int x, int z, TerrainGenerator* terrainGenerator) : Node(BoundingBox())
{

	m_x = x * terrainGenerator->GetSize();
	m_z = z * terrainGenerator->GetSize();

	SetPos(glm::vec3(m_x, 0.0f, m_z));

	m_program = wolf::ProgramManager::CreateProgram("data/shaders/terrain/terrain.vsh", "data/shaders/terrain/terrain.fsh");
	m_program->addShader("data/shaders/terrain/terrain.tesc", GL_TESS_CONTROL_SHADER);
	m_program->addShader("data/shaders/terrain/terrain.tese", GL_TESS_EVALUATION_SHADER);
	//m_program->addShader("data/shaders/terrain.gsh", GL_GEOMETRY_SHADER);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	m_decl = terrainGenerator->GenerateVertices(x, z);

	m_heightMap = new Heightmap(terrainGenerator->getVertices(), terrainGenerator->GetVertexCount(), terrainGenerator->GetSize(), PxVec3(m_x, 0.0f, m_z));

	m_terrainGenerator = terrainGenerator;

	BoundingBox bounds = terrainGenerator->GetBounds(x, z);

	GetBoundingBox().SetMin(bounds.GetMin());
	GetBoundingBox().SetMax(bounds.GetMax());
}

Terrain::~Terrain()
{
	wolf::ProgramManager::DestroyProgram(m_program);
	m_decl->Clean();
	delete m_decl;
	delete m_heightMap;
}

void Terrain::Update(float dt)
{
}

Terrain::Biome Terrain::getBiome()
{
	return m_biome;
}

void Terrain::setBiome(Terrain::Biome pBiome)
{
	m_biome = pBiome;
}

void Terrain::Render(const glm::mat4& mProj, const glm::mat4& mView)
{
	glm::mat4 mWorld = glm::mat4(1.0f);
	mWorld = glm::translate(mWorld, glm::vec3(m_x, 0.0f, m_z));

	m_program->SetUniform("projection", mProj);
	m_program->SetUniform("view", mView);
	m_program->SetUniform("world", mWorld);
	m_program->SetUniform("worldIT", glm::transpose(glm::inverse(mWorld)));

	wolf::Texture* regularTexture1 = wolf::TextureManager::CreateTexture("data/textures/ground/sand/diff.jpg");
	wolf::Texture* regularTexture2 = wolf::TextureManager::CreateTexture("data/textures/ground/grass/diff.jpg");
	wolf::Texture* regularTexture3 = wolf::TextureManager::CreateTexture("data/textures/ground/rocks/diff.jpg");
	wolf::Texture* regularTexture4 = wolf::TextureManager::CreateTexture("data/textures/ground/snow/diff.jpg");

	wolf::Texture* regularTexture1Normal = wolf::TextureManager::CreateTexture("data/textures/ground/sand/nor.exr");
	wolf::Texture* regularTexture2Normal = wolf::TextureManager::CreateTexture("data/textures/ground/grass/nor.jpg");
	wolf::Texture* regularTexture3Normal = wolf::TextureManager::CreateTexture("data/textures/ground/rocks/nor.jpg");
	wolf::Texture* regularTexture4Normal = wolf::TextureManager::CreateTexture("data/textures/ground/snow/nor.exr");

	wolf::Texture* regularTexture1Disp = wolf::TextureManager::CreateTexture("data/textures/ground/sand/disp.png");
	wolf::Texture* regularTexture2Disp = wolf::TextureManager::CreateTexture("data/textures/ground/grass/disp.jpg");
	wolf::Texture* regularTexture3Disp = wolf::TextureManager::CreateTexture("data/textures/ground/rocks/disp.jpg");
	wolf::Texture* regularTexture4Disp = wolf::TextureManager::CreateTexture("data/textures/ground/snow/disp.png");

	regularTexture1->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture2->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture3->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture4->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);

	regularTexture1->SetFilterMode(wolf::Texture::FilterMode::FM_TrilinearMipmap, wolf::Texture::FilterMode::FM_TrilinearMipmap);
	regularTexture2->SetFilterMode(wolf::Texture::FilterMode::FM_TrilinearMipmap, wolf::Texture::FilterMode::FM_TrilinearMipmap);
	regularTexture3->SetFilterMode(wolf::Texture::FilterMode::FM_TrilinearMipmap, wolf::Texture::FilterMode::FM_TrilinearMipmap);
	regularTexture4->SetFilterMode(wolf::Texture::FilterMode::FM_TrilinearMipmap, wolf::Texture::FilterMode::FM_TrilinearMipmap);

	regularTexture1Normal->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture2Normal->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture3Normal->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture4Normal->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);

	regularTexture1Disp->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture2Disp->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture3Disp->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);
	regularTexture4Disp->SetWrapMode(wolf::Texture::WrapMode::WM_Repeat, wolf::Texture::WrapMode::WM_Repeat);

	wolf::Texture* desertTexture = wolf::TextureManager::CreateTexture("data/textures/ground/desert.png");
	wolf::Texture* lavaTexture = wolf::TextureManager::CreateTexture("data/textures/ground/lava.png");
	wolf::Texture* sandTexture = wolf::TextureManager::CreateTexture("data/textures/ground/sand.png");

	if (m_biome == Desert)
	{
		m_program->SetTexture("terrainRegions[0].colorMap", desertTexture);
		m_program->SetTexture("terrainRegions[1].colorMap", desertTexture);
		m_program->SetTexture("terrainRegions[2].colorMap", desertTexture);
		m_program->SetTexture("terrainRegions[3].colorMap", desertTexture);
	}
	else if (m_biome == Water)
	{
		m_program->SetTexture("terrainRegions[0].colorMap", sandTexture);
		m_program->SetTexture("terrainRegions[1].colorMap", sandTexture);
		m_program->SetTexture("terrainRegions[2].colorMap", sandTexture);
		m_program->SetTexture("terrainRegions[3].colorMap", sandTexture);
	}

	else if (m_biome == Lava)
	{
		m_program->SetTexture("terrainRegions[0].colorMap", lavaTexture);
		m_program->SetTexture("terrainRegions[1].colorMap", lavaTexture);
		m_program->SetTexture("terrainRegions[2].colorMap", lavaTexture);
		m_program->SetTexture("terrainRegions[3].colorMap", lavaTexture);
	}
	else if (m_biome == Regular)
	{
		m_program->SetTexture("terrainRegions[0].colorMap", regularTexture1);
		m_program->SetTexture("terrainRegions[1].colorMap", regularTexture2);
		m_program->SetTexture("terrainRegions[2].colorMap", regularTexture3);
		m_program->SetTexture("terrainRegions[3].colorMap", regularTexture4);

		m_program->SetTexture("terrainRegions[0].normalMap", regularTexture1Normal);
		m_program->SetTexture("terrainRegions[1].normalMap", regularTexture2Normal);
		m_program->SetTexture("terrainRegions[2].normalMap", regularTexture3Normal);
		m_program->SetTexture("terrainRegions[3].normalMap", regularTexture4Normal);

		m_program->SetTexture("terrainRegions[0].dispMap", regularTexture1Disp);
		m_program->SetTexture("terrainRegions[1].dispMap", regularTexture2Disp);
		m_program->SetTexture("terrainRegions[2].dispMap", regularTexture3Disp);
		m_program->SetTexture("terrainRegions[3].dispMap", regularTexture4Disp);

		m_program->SetUniform("terrainRegions[0].tileFactor", 1.0f);
		m_program->SetUniform("terrainRegions[1].tileFactor", 1.0f);
		m_program->SetUniform("terrainRegions[2].tileFactor", 1.0f);
		m_program->SetUniform("terrainRegions[3].tileFactor", 1.0f);

		for (int i = 0; i < 4; i++)
		{
			m_program->SetUniform("terrainRegions[" + std::to_string(i) + "].dispMult", m_dispMult[i]);
		}
	}

	m_program->SetUniform("terrainRegions[0].min", -403.564362f);
	m_program->SetUniform("terrainRegions[0].max", -10.0f);

	m_program->SetUniform("terrainRegions[1].min", -10.0f);
	m_program->SetUniform("terrainRegions[1].max", 127.587504f);

	m_program->SetUniform("terrainRegions[2].min", 127.587504f);
	m_program->SetUniform("terrainRegions[2].max", 255.175008f);

	m_program->SetUniform("terrainRegions[3].min", 255.175008f);
	m_program->SetUniform("terrainRegions[3].max", 382.762512f);

	m_program->SetUniform("u_lightPos", Scene::Instance()->GetLightDirection());
	m_program->SetUniform("u_viewPos", glm::vec3(0.0f, 0.0f, 0.0f));
	// 158, 108, 56
	m_program->SetUniform("u_ambientLight", glm::vec3(0.3f, 0.4f, 0.5f));

	m_program->Bind();
	m_decl->Bind();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_PATCHES,
		4 * ((m_terrainGenerator->GetVertexCount() - 1) * (m_terrainGenerator->GetVertexCount() - 1)),
		GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
