#include "stategameplay.h"
#include "../obj/model.h"
#include "states.h"

StateGameplay::StateGameplay()
{
}

StateGameplay::~StateGameplay()
{
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
	delete m_drinkText;
	delete m_eatText;
	delete m_pStateMachine;

	delete m_pSoundManager;

	for (Terrain *terrain : m_lTerrains)
	{
		delete terrain;
	}

	for (Model *model : m_lModels)
	{
		delete model;
	}

	for (Light *pLight : m_vLights)
	{
		delete pLight;
	}
	wolf::BulletPhysicsManager::DestroyInstance();
}

void StateGameplay::Exit()
{
}

void StateGameplay::Reset()
{
	delete m_pTerrainGenerator;
	delete m_pFlashlight;
	delete m_pSpotlight;
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	m_pWorldProgram = nullptr;
	wolf::MaterialManager::DestroyMaterial(m_pMat);
	wolf::TextureManager::DestroyTexture(m_pCreatureTex);
	wolf::TextureManager::DestroyTexture(m_pShipTex);
	delete m_pSkybox;
	delete m_pCam;
	delete m_font;
	delete m_hungerText;
	delete m_thirstText;
	delete m_drinkText;
	delete m_eatText;
	delete m_pStateMachine;

	delete m_pSoundManager;

	for (Terrain *terrain : m_lTerrains)
	{
		delete terrain;
	}

	for (Model *model : m_lModels)
	{
		delete model;
	}

	for (Light *pLight : m_vLights)
	{
		delete pLight;
	}
	wolf::BulletPhysicsManager::DestroyInstance();
}

void StateGameplay::Enter(std::string arg)
{
	m_pApp->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	srand(time(NULL));
	if (!m_pWorldProgram)
	{

		wolf::BulletPhysicsManager::CreateInstance("data/physics/physics_materials.xml",
												   "data/shaders/lines.vsh",
												   "data/shaders/lines.fsh");

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);
		m_font = new Font("data/fonts/inconsolata.fnt", "data/textures/fonts/");

		m_hungerText = new TextBox(200.0f, 200.0f);
		m_hungerText->SetPos(20.0f, 500.0f, 0.0f);

		m_hungerText->SetText(m_font, "%f%", m_fHunger);
		m_hungerText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_hungerText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_hungerText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_thirstText = new TextBox(200.0f, 200.0f);
		m_thirstText->SetPos(20.0f, 600.0f, 0.0f);

		m_thirstText->SetText(m_font, "%f%", m_fThirst);
		m_thirstText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_thirstText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_thirstText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_drinkText = new TextBox(700.0f, 200.0f);
		m_drinkText->SetPos(400.0f, 50.0f, 0.0f);

		m_drinkText->SetText(m_font, m_drinkPrompt.c_str());
		m_drinkText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_drinkText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_drinkText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_eatText = new TextBox(700.0f, 200.0f);
		m_eatText->SetPos(400.0f, 50.0f, 0.0f);

		m_eatText->SetText(m_font, m_eatPrompt.c_str());
		m_eatText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_eatText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_eatText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_pFlashlight = new Model("data/models/flashlight.fbx", "dim");
		m_pFlashlight->setPosition(glm::vec3(11, 9, -22));
		m_pFlashlight->setScale(glm::vec3(0.005f, 0.005f, 0.005f));
		// m_pFlashlight->setRotation(180, glm::vec3(0.0f, 1.0f, 0.0f));

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
			m_pCreature->setTag("enemy");
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

		for (int i = 0; i < 10; i++)
		{
			Model *m_pBush = new Model("data/models/shrub.fbx", "skinned");
			m_pBush->setTag("food");
			m_pBush->setTexture("data/textures/shrub.png");
			m_pBush->setOffset(m_pBush->getModel()->getAABBMin());

			int scale = _randomNum(2, 5);
			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(-100, 100);
			int z = _randomNum(-100, 100);
			m_pBush->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
			m_pBush->translate(glm::vec3(x, 0.0f, z));
			m_pBush->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			m_lModels.push_back(m_pBush);
		}

		// m_font = new Font("data/fonts/inconsolata.fnt", "data/textures/fonts/");
		m_pSoundManager = new wolf::SoundManager();
		m_pSoundManager->CreateSoundSystem();
		// m_pSoundManager->Play2D("Nature", m_natureSoundPath, true);
		// m_pSoundManager->Play2D("Water", m_waterSoundPath, true);
		// Debug Menu
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_pApp->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 430 core");

		m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_pTerrainGenerator = new TerrainGenerator();

		m_terrainSize = m_pTerrainGenerator->GetSize();
		m_terrainVerts = m_pTerrainGenerator->GetVertexCount();
		m_terrainOctaves = m_pTerrainGenerator->GetOctaves();
		m_terrainRoughness = m_pTerrainGenerator->GetRoughness();
		m_terrainAmplitude = m_pTerrainGenerator->GetAmplitude();

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Terrain *terrain = new Terrain(i, j, m_pTerrainGenerator);
				m_lTerrains.push_back(terrain);
			}
		}

		// Ground
		m_pPlane = new Plane(m_pWorldProgram, m_groundTexPath);
		m_pPlane->setScale(100);

		// Water
		m_pWater = new Water();
		m_pWater->setCamera(m_pCam);
		m_pWater->setScale(glm::vec3(100.0f, 10.0f, 100.0f));
		m_pWater->setPosition(glm::vec3(300.0f, -20.0f, 300.0f));

		if (m_pCam)
		{
			m_pSkybox = new Skybox(m_pWorldProgram, m_pCam, m_skyboxPath);
		}
	}
}

void StateGameplay::Update(float p_fDelta)
{
	wolf::BulletPhysicsManager::Instance()->Update(p_fDelta);
	if (m_pCam)
		m_pSkybox->update(p_fDelta);

	glm::vec3 lightspot = glm::normalize(m_pCam->getViewDirection() - glm::vec3(m_pSpotlight->vPosRange.x, m_pSpotlight->vPosRange.y, m_pSpotlight->vPosRange.z));
	m_pSpotlight->vLightSpot = glm::vec4(lightspot, 0.2f);
	m_pFlashlight->setPosition(m_pCam->getPosition() + glm::vec3(0.23f, -0.25f, 0.0f));
	m_fHunger = glm::max(m_fHunger - (p_fDelta * (rand() % 3 + 2)), 0.0f);
	m_fThirst = glm::max(m_fThirst - p_fDelta * (rand() % 2 + 1), 0.0f);

	int hunger = (int)m_fHunger;
	int thirst = (int)m_fThirst;

	glm::vec3 camPosition = m_pCam->getPosition();

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

	m_thirstText->SetText(m_font, "%d%", thirst);

	for (Model *model : m_lModels)
	{
		model->update(p_fDelta);
	}
	m_vPrevCamRot = m_pCam->getRotation();
	m_pWater->update(p_fDelta);

	if (m_pApp->isKeyDown('F') && !m_bKeyDown)
	{
		m_bKeyDown = true;
		m_pSoundManager->Play2D("flashlight", m_flashlightSoundPath, false, true);
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

	m_pTerrainGenerator->SetSize(m_terrainSize);
	m_pTerrainGenerator->SetAmplitude(m_terrainAmplitude);
	m_pTerrainGenerator->SetOctaves(m_terrainOctaves);
	m_pTerrainGenerator->SetVertexCount(m_terrainVerts);
	m_pTerrainGenerator->SetRoughness(m_terrainRoughness);

	m_bNearFood = false;
	for (Model *pModel : m_lModels)
	{
		if (pModel->getTag().compare("enemy") == 0)
		{
			// Model is an enemy
			glm::vec3 m_vModelPos = pModel->getPosition();
			glm::vec3 m_vPlayerPos = m_pCam->getPosition();

			if (Util::inProximity(m_vModelPos, m_vPlayerPos, m_vEnemyRange))
			{
				float xDist = m_vPlayerPos.x - m_vModelPos.x;
				float zDist = m_vPlayerPos.z - m_vModelPos.z;
				pModel->setPosition(m_vModelPos + glm::vec3(xDist * m_fEnemySpeed, 0.0f, zDist * m_fEnemySpeed));
			}
			else
			{
				bool rand_bool = Util::randBool();
				if (rand_bool)
				{
					float xDist = Util::randNum(0.0f, 5.0f);
					float zDist = Util::randNum(0.0f, 5.0f);
					pModel->setPosition(m_vModelPos + glm::vec3(xDist * m_fEnemySpeed, 0.0f, zDist * m_fEnemySpeed));
				}
			}
		}

		// CHECK IF NEAR FOOD
		if (pModel->getTag().compare("food") == 0)
		{
			// Model is an enemy
			glm::vec3 m_vModelPos = pModel->getPosition();
			glm::vec3 m_vPlayerPos = m_pCam->getPosition();

			if (Util::inProximity(m_vModelPos, m_vPlayerPos, m_vFoodRange))
			{
				m_bNearFood = true;
			}
		}
	}

	// CHECK IF NEAR WATER
	m_bNearWater = Util::inProximity(m_pWater->getPosition(), m_pCam->getPosition(), m_pWater->getScale());

	if (m_bNearWater && m_pApp->isKeyDown('E') && !m_bDrinking)
	{
		m_bDrinking = true;
		m_pSoundManager->Play2D("drinking", m_drinkingSoundPath, false, true);

		m_fThirst += 10.0f;
		if (m_fThirst > 100.0f)
			m_fThirst = 100.0f;
	}

	if (!m_pApp->isKeyDown('E'))
		m_bDrinking = false;

	if (m_bNearFood && m_pApp->isKeyDown('E') && !m_bEating)
	{
		m_bEating = true;
		m_pSoundManager->Play2D("eating", m_eatingSoundPath, false, true);

		m_fHunger += 10.0f;
		if (m_fHunger > 100.0f)
			m_fHunger = 100.0f;
	}

	if (!m_pApp->isKeyDown('E'))
	{
		m_bDrinking = false;
		m_bEating = false;
	}

	if (m_fHunger <= 0.0f && m_fThirst <= 0.0f)
	{
		m_pStateMachine->GoToState(eStateGameplay_Respawn);
	}
}

void StateGameplay::Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height)
{
	// Debug Menu

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pWorldProgram->SetUniform("u_lightDir", m_sunDirection);
	m_pWorldProgram->SetUniform("u_viewPos", m_pCam->getViewDirection());

	for (Model *model : m_lModels)
	{

		std::sort(m_vLights.begin(), m_vLights.end(), [this, model](const Light *lhs, const Light *rhs) -> bool
				  { return _isEffectiveLight(lhs, rhs, model); });

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

	m_pWater->render(mProj, mView, width, height);
	m_pPlane->render(mProj, mView, width, height);

	// for (Terrain *terrain : m_lTerrains)
	// {
	// 	terrain->Render(mProj, mView);
	// }

	// ImGui_ImplOpenGL3_NewFrame();
	// ImGui_ImplGlfw_NewFrame();
	// ImGui::NewFrame();
	// ImGui::Begin("Terrain Debug Menu");
	// ImGui::SetWindowSize(ImVec2(400.0f, 400.0f), true);
	// ImGui::SliderInt("Size", &m_terrainSize, 0, 1600);
	// ImGui::SliderInt("Vertex Count", &m_terrainVerts, 0, 1024);
	// ImGui::SliderInt("Octaves", &m_terrainOctaves, 0, 10);
	// ImGui::SliderFloat("Amplitude", &m_terrainAmplitude, 0, 200.0f);
	// ImGui::SliderFloat("Roughness", &m_terrainRoughness, 0, 2.0f);
	// if (ImGui::Button("Rerender"))
	// {
	// 	_renderTerrain();
	// }
	// if (ImGui::Button("Generate New Terrain"))
	// {
	// 	delete m_pTerrainGenerator;
	// 	m_pTerrainGenerator = new TerrainGenerator();
	// 	m_pTerrainGenerator->SetSize(m_terrainSize);
	// 	m_pTerrainGenerator->SetAmplitude(m_terrainAmplitude);
	// 	m_pTerrainGenerator->SetOctaves(m_terrainOctaves);
	// 	m_pTerrainGenerator->SetVertexCount(m_terrainVerts);
	// 	m_pTerrainGenerator->SetRoughness(m_terrainRoughness);
	// 	_renderTerrain();
	// }
	// ImGui::End();
	// ImGui::Render();
	// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	if (m_pApp->isKeyDown('W') || m_pApp->isKeyDown('A') || m_pApp->isKeyDown('D') || m_pApp->isKeyDown('S'))
	{
		if (!m_bWalking && !m_bRunning)
		{
			m_pSoundManager->Play2D("walking", m_walkingSoundPath, true, true);
			m_bWalking = true;
		}

		if (m_pApp->isKeyDown(GLFW_KEY_LEFT_CONTROL) && !m_bRunning)
		{
			m_pSoundManager->Play2D("running", m_runningSoundPath, true, true);
			m_bWalking = false;
			m_bRunning = true;
		}
	}
	if (!m_pApp->isKeyDown('W') && !m_pApp->isKeyDown('A') && !m_pApp->isKeyDown('D') && !m_pApp->isKeyDown('S'))
	{

		m_bWalking = false;
		m_bRunning = false;
		ISound *pWalking = m_pSoundManager->GetSound("walking");
		ISound *pRunning = m_pSoundManager->GetSound("running");
		if (pWalking && !pWalking->getIsPaused())
			m_pSoundManager->PauseSound("walking");
		if (pRunning && !pRunning->getIsPaused())
			m_pSoundManager->PauseSound("running");
	}

	if (m_bFlashlightEquipped)
		m_pFlashlight->render(mProj, mView);

	m_hungerText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	m_thirstText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	if (m_bNearWater)
	{
		m_drinkText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	}
	if (m_bNearFood)
	{
		m_eatText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	}
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

	if (pLight1->bEnabled && !pLight2->bEnabled)
		return true;

	if (!pLight1->bEnabled || (!pLight2->bEnabled && !pLight1->bEnabled))
		return false;

	float fLight1Dist = glm::distance(glm::vec3(pLight1->vPosRange.x, pLight1->vPosRange.y, pLight1->vPosRange.z), pModel->getPosition());
	float fLight2Dist = glm::distance(glm::vec3(pLight2->vPosRange.x, pLight2->vPosRange.y, pLight2->vPosRange.z), pModel->getPosition());

	return glm::any(glm::lessThan(pLight1->vAttenuation, pLight2->vAttenuation)) && (fLight1Dist < fLight2Dist);
}

void StateGameplay::RegisterCamera(Camera *pCam)
{
	m_pCam = pCam;
}

void StateGameplay::RegisterMachine(Common::StateMachine *pMachine)
{
	m_pStateMachine = pMachine;
}

void StateGameplay::_renderTerrain()
{
	for (Terrain *terrain : m_lTerrains)
	{
		delete terrain;
	}
	m_lTerrains.clear();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Terrain *terrain = new Terrain(i, j, m_pTerrainGenerator);
			m_lTerrains.push_back(terrain);
		}
	}
}