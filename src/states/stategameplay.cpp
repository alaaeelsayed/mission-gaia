#include "stategameplay.h"
#include "../obj/model.h"
#include "states.h"

StateGameplay::StateGameplay()
{
}

StateGameplay::~StateGameplay()
{
	delete m_pTerrain;
	delete m_pTerrainGenerator;
	delete m_pFlashlight;
	delete m_pSpotlight;
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	wolf::MaterialManager::DestroyMaterial(m_pMat);
	wolf::TextureManager::DestroyTexture(m_pCreatureTex);
	wolf::TextureManager::DestroyTexture(m_pShipTex);
	delete m_pSkybox;
	delete m_pCam;
	delete m_font;
	delete m_hungerText;
	delete m_thirstText;
	m_pSoundEngine->drop();
	m_pSoundEngine = nullptr;

	for (Model *model : m_lModels)
	{
		delete model;
	}

	for (Light *pLight : m_vLights)
	{
		delete pLight;
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
		m_font = new Font("data/fonts/inconsolata.fnt", "data/textures/fonts/");

		m_pSoundEngine = irrklang::createIrrKlangDevice();
		
		if(m_pSoundEngine)
			m_pSoundEngine->play2D(m_natureSoundPath.c_str(), true);

		m_hungerText = new TextBox(200.0f, 200.0f);
		m_hungerText->SetPos(-550.0f, -120.0f, -800.0f);

		m_hungerText->SetText(m_font, "%f%", m_fHunger);
		m_hungerText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_hungerText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_hungerText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_thirstText = new TextBox(200.0f, 200.0f);
		m_thirstText->SetPos(-550.0f, -200.0f, -800.0f);

		m_thirstText->SetText(m_font, "%f%", m_fThirst);
		m_thirstText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_thirstText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_thirstText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_pFlashlight = new Model("data/models/flashlight.fbx", "dim");
		m_pFlashlight->setPosition(glm::vec3(11, 9, -22));
		m_pFlashlight->setScale(glm::vec3(0.005f, 0.005f, 0.005f));
		m_pFlashlight->setRotation(180, glm::vec3(0.0f, 1.0f, 0.0f));

		m_pSpotlight = new Light();
		m_pSpotlight->vPosRange = glm::vec4(0.0f, 0.0f, 0.0f, 100.0f);
		m_pSpotlight->vColor = glm::vec3(0.005f, 0.005f, 0.005f);
		m_pSpotlight->vAttenuation = glm::vec3(0.0f, 0.5f, 0.0f);
		m_pSpotlight->vLightSpot = glm::vec4(2.0f, 2.0f, 2.0f, 0.8f);
		m_pSpotlight->bEnabled = false;

		m_pFlashlight->attachLight(m_pSpotlight);
		m_pFlashlight->setTexture("data/textures/flashlight.png");
		m_lModels.push_back(m_pFlashlight);

		for (int i = 0; i < 10; i++)
		{
			Model *m_pCreature = new Model("data/models/low_poly_spitter.obj", "skinned");
			m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
			m_pCreature->setNormal("data/textures/gimpy_normal.tga");
			m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin());

			Light *pPointLight = new Light();
			float r = _randomFloat(0.0001f, 0.0018f);
			float g = _randomFloat(0.0001f, 0.0028f);
			float b = _randomFloat(0.0001f, 0.0038f);
			pPointLight->vColor = glm::vec3(r, g, b);
			pPointLight->vAttenuation = glm::vec3(0.9f, 0.9f, 0.9f);
			pPointLight->vPosRange = glm::vec4(0.0f, 0.0f, 0.0f, 3.0f);
			m_vLights.push_back(pPointLight);
			m_pCreature->attachLight(pPointLight);
			int scale = _randomNum(2, 5);
			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-100, 100);
			int z = _randomNum(-100, 100);
			m_pCreature->setScale(glm::vec3(scale, scale, scale));
			m_pCreature->translate(glm::vec3(x, 0.0f, z));
			m_pCreature->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
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
			m_pShrub->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
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
		// 	m_pWeapon->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
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
		// m_pRock->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
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
		// 	m_pLog->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		// 	m_lModels.push_back(m_pLog);
		// }

		m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_pTerrainGenerator = new TerrainGenerator();

		m_pTerrain = new Terrain(0, 0, m_pTerrainGenerator);
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

	glm::vec3 lightspot = glm::normalize(m_pCam->getViewDirection() - glm::vec3(m_pSpotlight->vPosRange.x, m_pSpotlight->vPosRange.y, m_pSpotlight->vPosRange.z));
	m_pSpotlight->vLightSpot = glm::vec4(lightspot, 0.2f);
	m_pFlashlight->setPosition(m_pCam->getViewDirection() + glm::vec3(0.23f, -0.25f, 0.0f));
	m_fHunger = glm::max(m_fHunger - (p_fDelta * (rand() % 3 + 2)), 0.0f);
	m_fThirst = glm::max(m_fThirst - p_fDelta * (rand() % 2 + 1), 0.0f);

	int hunger = (int)m_fHunger;
	int thirst = (int)m_fThirst;

	glm::vec3 camPosition = m_pCam->getPosition();
	
	// m_hungerText->SetPos(camPosition.x - 50.0f, camPosition.y - 50.0f, camPosition.z + 60.0f);
	m_hungerText->SetText(m_font, "%d%", hunger);
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

	// m_thirstText->SetPos(camPosition.x- 50.0f, camPosition.y- 30.0f, camPosition.z + 60.0f);
	m_thirstText->SetText(m_font, "%d%", thirst);

	for (Model *model : m_lModels)
	{
		model->update(p_fDelta);
	}
	m_vPrevCamRot = m_pCam->getRotation();
}

void StateGameplay::Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height)
{

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pWorldProgram->SetUniform("u_lightDir", m_sunDirection);
	m_pWorldProgram->SetUniform("u_viewPos", m_pCam->getViewDirection());

	for (Model *model : m_lModels)
	{

		std::sort(m_vLights.begin(), m_vLights.end(), [this, model](const Light *lhs, const Light *rhs) -> bool
				  {
					  if (lhs->bEnabled && !rhs->bEnabled)
						  return true;

					  if (!lhs->bEnabled || (!rhs->bEnabled && !lhs->bEnabled))
						  return false;

					  float fLight1Dist = glm::distance(glm::vec3(lhs->vPosRange.x, lhs->vPosRange.y, lhs->vPosRange.z), model->getPosition());
					  float fLight2Dist = glm::distance(glm::vec3(rhs->vPosRange.x, rhs->vPosRange.y, rhs->vPosRange.z), model->getPosition());

					  return glm::any(glm::lessThan(lhs->vAttenuation, rhs->vAttenuation)) && (fLight1Dist < fLight2Dist);
				  });

		for (int i = 0; i < 4; i++)
		{
			// Point lights
			Light *pLight = m_vLights[i];
			if (pLight->bEnabled)
			{
				m_pWorldProgram->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->vPosRange);
				m_pWorldProgram->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->vColor);
				m_pWorldProgram->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->vAttenuation);
				model->getMaterial()->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->vPosRange);
				model->getMaterial()->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->vColor);
				model->getMaterial()->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->vAttenuation);
			}
		}

		// Spot Light
		if (m_pSpotlight->bEnabled)
		{
			m_pWorldProgram->SetUniform("u_lightPosRange", m_pSpotlight->vPosRange);
			m_pWorldProgram->SetUniform("u_lightColor", m_pSpotlight->vColor);
			m_pWorldProgram->SetUniform("u_lightSpot", m_pSpotlight->vLightSpot);
			m_pWorldProgram->SetUniform("u_lightAttenuation", m_pSpotlight->vAttenuation);

			model->getMaterial()->SetUniform("u_lightPosRange", m_pSpotlight->vPosRange);
			model->getMaterial()->SetUniform("u_lightColor", m_pSpotlight->vColor);
			model->getMaterial()->SetUniform("u_lightSpot", m_pSpotlight->vLightSpot);
			model->getMaterial()->SetUniform("u_lightAttenuation", m_pSpotlight->vAttenuation);
		}
		else
		{
			m_pWorldProgram->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			m_pWorldProgram->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			m_pWorldProgram->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));

			model->getMaterial()->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			model->getMaterial()->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			model->getMaterial()->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));
		}
		model->render(mProj, mView, m_pCam->getViewDirection());
	}

	if (m_pCam)
		m_pSkybox->render(mProj, mView, width, height);
	m_pTerrain->Render(mProj, mView);

	if (m_pApp->isKeyDown('F') && !m_bKeyDown)
	{
		m_bKeyDown = true;
		m_bFlashlightEquipped = !m_bFlashlightEquipped;
		m_pSpotlight->bEnabled = !m_pSpotlight->bEnabled;
		if (!m_pSpotlight->bEnabled)
		{
			m_pSpotlight->vAttenuation = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			m_pSpotlight->vAttenuation = glm::vec3(0.0f, 0.5f, 0.0f);
		}
	}

	if (!m_pApp->isKeyDown('F'))
		m_bKeyDown = false;

	if (m_bFlashlightEquipped)
		m_pFlashlight->render(mProj, mView);

	m_hungerText->Render(mProj, mView);
	m_thirstText->Render(mProj, mView);
}

int StateGameplay::_randomNum(int lowerBound, int upperBound)
{
	// Generates random number in range. Includes lower bound and upper bound
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}

float StateGameplay::_randomFloat(float lo, float hi)
{
	return lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (hi - lo)));
}

bool StateGameplay::_isEffectiveLight(const Light *pLight1, const Light *pLight2, Model *pModel) const
{

	if (&pLight1 == &pLight2)
	{
		return false;
	}

	if (!pLight1->bEnabled || !pLight2->bEnabled)
	{
		return pLight1->bEnabled;
	}

	// Testing Attenuation
	bool result = glm::any(glm::lessThan(pLight1->vAttenuation, pLight2->vAttenuation));

	// Testing Distance
	float fLight1Dist = glm::distance(glm::vec3(pLight1->vPosRange.x, pLight1->vPosRange.y, pLight1->vPosRange.z), pModel->getPosition());
	float fLight2Dist = glm::distance(glm::vec3(pLight2->vPosRange.x, pLight2->vPosRange.y, pLight2->vPosRange.z), pModel->getPosition());
	result = result || (fLight1Dist >= fLight2Dist);

	return result;
}

void StateGameplay::RegisterCamera(Camera *pCam)
{
	m_pCam = pCam;
}