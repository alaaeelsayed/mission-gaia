#include "./StateGameplay.h"
#include "States.h"

StateGameplay::StateGameplay()
{
}

StateGameplay::~StateGameplay()
{
	delete m_pPlane;
	delete m_pFlashlight;
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	wolf::MaterialManager::DestroyMaterial(m_pMat);
	wolf::TextureManager::DestroyTexture(m_pCreatureTex);
	wolf::TextureManager::DestroyTexture(m_pShipTex);
	delete m_pSkybox;
	delete m_pCam;
	delete m_font;
	delete m_hungerText;
	delete m_thirstText;

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
	m_pApp->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!m_pWorldProgram)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);
		m_font = new Font("data/fonts/alpha.fnt", "data/textures/fonts/");

		m_hungerText = new TextBox(300.0f, 300.0f);
		m_hungerText->SetPos(80.0f, 470.0f);

		std::string hunger = std::to_string(m_fHunger) + "%";
		m_hungerText->SetText(m_font, hunger.c_str());
		m_hungerText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_hungerText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_hungerText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_thirstText = new TextBox(300.0f, 300.0f);
		m_thirstText->SetPos(80.0f, 570.0f);

		std::string thirst = std::to_string(m_fThirst) + "%";
		m_thirstText->SetText(m_font, thirst.c_str());
		m_thirstText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_thirstText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_thirstText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_pFlashlight = new Model("data/models/flashlight.fbx", "dim");
		m_pFlashlight->setPosition(glm::translate(glm::mat4(1.0f), glm::vec3(11, 9, -22)));
		m_pFlashlight->setTexture("data/textures/flashlight.png");

		for (int i = 0; i < 4; i++)
		{
			Model *m_pCreature = new Model("data/models/low_poly_spitter.obj", "skinned");
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
			Model *m_pShrub = new Model("data/models/shrub.fbx", "skinned");
			m_pShrub->setTexture("data/textures/shrub.png");

			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-20, 20);
			int z = _randomNum(-20, 20);
			m_pShrub->translate(glm::vec3(x, 0.0f, z));
			m_pShrub->setScale(glm::vec3(0.008, 0.008, 0.008));
			m_pShrub->rotate(rotation);
			m_lModels.push_back(m_pShrub);
		}

		// for (int i = 0; i < 8; i++)
		// {
		// 	Model *m_pWeapon = new Model("data/models/cyborg_weapon.fbx", "skinned");
		// 	m_pWeapon->setTexture("data/textures/weapons/Weapon_BaseColor.png");

		// 	float rotation = (float)_randomNum(-60, 60);
		// 	int x = _randomNum(-15, 5);
		// 	int z = _randomNum(-10, 20);
		// 	m_pWeapon->setOffset(m_pWeapon->getModel()->getAABBMin());
		// 	m_pWeapon->setScale(glm::vec3(6.0f, 6.0f, 6.0f));
		// 	m_pWeapon->translate(glm::vec3(x, 5.0f, z));
		// 	m_pWeapon->rotate(rotation);
		// 	m_lModels.push_back(m_pWeapon);
		// }

		// std::string path = "data/models/rocks/rock1.fbx";
		// Model *m_pRock = new Model(path.c_str(), "skinned");
		// m_pRock->setTexture("data/textures/diffuseGray.png");

		// float rotation = (float)_randomNum(-60, 60);
		// int x = _randomNum(-15, 5);
		// int z = _randomNum(-10, 20);
		// m_pRock->setOffset(m_pRock->getModel()->getAABBMin());

		// m_pRock->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
		// m_pRock->translate(glm::vec3(x, 0.0f, z));
		// m_pRock->rotate(rotation);
		// m_lModels.push_back(m_pRock);

		// for (int i = 0; i < 8; i++)
		// {
		// 	Model *m_pLog = new Model("data/models/log.fbx", "skinned");
		// 	m_pLog->setTexture("data/textures/log.png");
		// 	m_pLog->setNormal("data/textures/log_normal.png");

		// 	float rotation = (float)_randomNum(-60, 60);
		// 	int x = _randomNum(-20, 10);
		// 	int z = _randomNum(-10, 20);
		// 	m_pLog->translate(glm::vec3(x, 1.5f, z));
		// 	m_pLog->rotate(rotation);
		// 	m_lModels.push_back(m_pLog);
		// }

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
	if (m_pCam)
		m_pSkybox->update(p_fDelta);

	glm::mat4 mWorld(1.0f);
	m_pFlashlight->setPosition(glm::translate(mWorld, m_pCam->getPosition() + glm::vec3(0.23f, -0.25f, -0.9f)));
	m_pFlashlight->setScale(glm::vec3(0.005f, 0.005f, 0.005f));
	m_pFlashlight->rotate(-180);

	if (m_fHunger != 0)
		m_fHunger -= p_fDelta * (rand() % 3 + 2);
	if (m_fThirst != 0)
		m_fThirst -= p_fDelta * (rand() % 2 + 1);

	std::string hunger = std::to_string((int)m_fHunger);
	std::string thirst = std::to_string((int)m_fThirst);

	m_hungerText->SetText(m_font, hunger.c_str());
	if (m_fHunger >= 70)
		m_hungerText->SetColor(0.0f, 1.0f, 0.0f, 0.6f);
	else if (m_fHunger >= 40)
		m_hungerText->SetColor(1.0f, 1.0f, 0.0f, 0.6f);
	else
		m_hungerText->SetColor(1.0f, 0.0f, 0.0f, 0.6f);

	if (m_fThirst >= 70)
		m_thirstText->SetColor(0.0f, 1.0f, 0.0f, 0.6f);
	else if (m_fThirst >= 40)
		m_thirstText->SetColor(1.0f, 1.0f, 0.0f, 0.6f);
	else
		m_thirstText->SetColor(1.0f, 0.0f, 0.0f, 0.6f);

	m_thirstText->SetText(m_font, thirst.c_str());
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
		if (m_bFlashlightEquipped)
		{
			model->getMaterial()->SetUniform("u_lightPosRange", glm::vec4(m_pCam->getPosition(), 100.0f));
			model->getMaterial()->SetUniform("u_lightSpot", glm::vec4(m_pCam->getPosition().z + glm::vec3(0.0f, 0.0f, -20.0f), 0.0f));
			model->getMaterial()->SetUniform("u_lightAttenuation", glm::vec3(0.0f, 0.3f, 0.0f));
		}
		else
		{
			model->getMaterial()->SetUniform("u_lightPosRange", glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f));
			model->getMaterial()->SetUniform("u_lightSpot", glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f));
			model->getMaterial()->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));
		}
		model->render(mProj, mView, m_pCam->getPosition());
	}

	if (m_pApp->isKeyDown('F') && !m_bKeyDown)
	{
		m_bKeyDown = true;
		m_bFlashlightEquipped = !m_bFlashlightEquipped;
	}

	if (!m_pApp->isKeyDown('F'))
		m_bKeyDown = false;

	if (m_bFlashlightEquipped)
		m_pFlashlight->render(mProj, mView);

	m_hungerText->Render(width, height);
	m_thirstText->Render(width, height);
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