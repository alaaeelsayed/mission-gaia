#include "./StateGameplay.h"
#include "States.h"

StateGameplay::StateGameplay()
{
}

StateGameplay::~StateGameplay()
{
	delete m_pPlane;
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	wolf::MaterialManager::DestroyMaterial(m_pMat);
	wolf::TextureManager::DestroyTexture(m_pCreatureTex);
	wolf::TextureManager::DestroyTexture(m_pShipTex);
	delete m_pSkybox;

	for (Model *model : m_lModels)
	{
		delete model;
	}
}

void StateGameplay::Exit()
{
}

void StateGameplay::Enter(std::string arg)
{
	if (!m_pWorldProgram)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);

		for (int i = 0; i < 4; i++)
		{
			Model *m_pCreature = new Model("data/models/low_poly_spitter.obj", "spitter");
			m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
			m_pCreature->setNormal("data/textures/gimpy_normal.tga");
			m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin());

			int scale = _randomNum(2, 5);
			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-5, 15);
			int z = _randomNum(-5, 15);
			m_pCreature->setScale(glm::vec3(scale, scale, scale));
			m_pCreature->translate(glm::vec3(x, 0.0f, z));
			m_pCreature->rotate(rotation);
			m_lModels.push_back(m_pCreature);
		}

		for (int i = 0; i < 7; i++)
		{
			Model *m_pShrub = new Model("data/models/shrub.fbx", "shrub");
			m_pShrub->setTexture("data/textures/shrub.png");

			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-20, 20);
			int z = _randomNum(-20, 20);
			m_pShrub->translate(glm::vec3(x, 0.0f, z));
			m_pShrub->setScale(glm::vec3(0.008, 0.008, 0.008));
			m_pShrub->rotate(rotation);
			m_lModels.push_back(m_pShrub);
		}

		for (int i = 0; i < 8; i++)
		{
			Model *m_pWeapon = new Model("data/models/cyborg_weapon.fbx", "weapon");
			m_pWeapon->setTexture("data/textures/weapons/Weapon_BaseColor.png");

			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-15, 5);
			int z = _randomNum(-10, 20);
			m_pWeapon->setOffset(m_pWeapon->getModel()->getAABBMin());
			m_pWeapon->setScale(glm::vec3(6.0f, 6.0f, 6.0f));
			m_pWeapon->translate(glm::vec3(x, 5.0f, z));
			m_pWeapon->rotate(rotation);
			m_lModels.push_back(m_pWeapon);
		}

		std::string path = "data/models/rocks/rock1.fbx";
		Model *m_pRock = new Model(path.c_str(), "rock");
		m_pRock->setTexture("data/textures/diffuseGray.png");

		float rotation = (float)_randomNum(-60, 60);
		int x = _randomNum(-15, 5);
		int z = _randomNum(-10, 20);
		m_pRock->setOffset(m_pRock->getModel()->getAABBMin());

		m_pRock->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
		m_pRock->translate(glm::vec3(x, 0.0f, z));
		m_pRock->rotate(rotation);
		m_lModels.push_back(m_pRock);

		for (int i = 0; i < 8; i++)
		{
			Model *m_pLog = new Model("data/models/log.fbx", "log");
			m_pLog->setTexture("data/textures/log.png");
			m_pLog->setNormal("data/textures/log_normal.png");

			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-20, 10);
			int z = _randomNum(-10, 20);
			m_pLog->translate(glm::vec3(x, 1.5f, z));
			m_pLog->rotate(rotation);
			m_lModels.push_back(m_pLog);
		}

		m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_pPlane = new Plane(m_pWorldProgram, m_groundTexPath);
		m_pPlane->setScale(500.0f);
		if (m_pCam)
		{
			m_pSkybox = new Skybox(m_pWorldProgram, m_pCam, m_skyboxPath);
		}
	}
}

void StateGameplay::Update(float p_fDelta)
{
	m_pSkybox->update(p_fDelta);
}

void StateGameplay::Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height)
{

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pWorldProgram->SetUniform("u_lightDir", m_sunDirection);

	if (m_pCam)
		m_pSkybox->render(mProj, mView, width, height);
	m_pPlane->render(mProj, mView, width, height);
	for (Model *model : m_lModels)
	{
		model->render(mProj, mView, m_pCam->getPosition());
	}
}

int StateGameplay::_randomNum(int lowerBound, int upperBound)
{
	// Generates random number in range. Includes lower bound and upper bound
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}

void StateGameplay::RegisterCamera(Camera *pCam)
{
	m_pCam = pCam;
}