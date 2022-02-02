#include "stategameplay.h"
#include "../obj/model.h"
#include "states.h"
#include "../scene/scene.h"

StateGameplay::StateGameplay()
{
}

StateGameplay::~StateGameplay()
{
	delete m_terrainGenerator;
	delete m_flashlight;
	delete m_spotlight;
	wolf::ProgramManager::DestroyProgram(m_worldProgram);
	wolf::MaterialManager::DestroyMaterial(m_mat);
	wolf::TextureManager::DestroyTexture(m_creatureTex);
	wolf::TextureManager::DestroyTexture(m_shipTex);
	delete m_skybox;
	delete m_font;
	delete m_hungerText;
	delete m_thirstText;
	delete m_drinkText;
	delete m_eatText;
	delete m_stateMachine;

	delete m_soundManager;

	for (Terrain *terrain : m_terrains)
	{
		delete terrain;
	}

	for (Model *model : m_models)
	{
		delete model;
	}

	for (Light *light : m_lights)
	{
		delete light;
	}
	wolf::BulletPhysicsManager::DestroyInstance();
}

void StateGameplay::Exit()
{
}

// void StateGameplay::Reset()
// {
// 	delete m_pTerrainGenerator;
// 	delete m_pFlashlight;
// 	delete m_pSpotlight;
// 	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
// 	m_pWorldProgram = nullptr;
// 	wolf::MaterialManager::DestroyMaterial(m_pMat);
// 	wolf::TextureManager::DestroyTexture(m_pCreatureTex);
// 	wolf::TextureManager::DestroyTexture(m_pShipTex);
// 	delete m_pSkybox;
// 	delete m_pCam;
// 	delete m_font;
// 	delete m_hungerText;
// 	delete m_thirstText;
// 	delete m_drinkText;
// 	delete m_eatText;
// 	delete m_pStateMachine;

// 	delete m_pSoundManager;

// 	for (Terrain *terrain : m_lTerrains)
// 	{
// 		delete terrain;
// 	}

// 	for (Model *model : m_lModels)
// 	{
// 		delete model;
// 	}

// 	for (Light *pLight : m_vLights)
// 	{
// 		delete pLight;
// 	}
// 	wolf::BulletPhysicsManager::DestroyInstance();
// }

void StateGameplay::Enter(std::string arg)
{
	m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!m_worldProgram)
	{

		wolf::BulletPhysicsManager::CreateInstance("data/physics/physics_materials.xml",
												   "data/shaders/lines.vsh",
												   "data/shaders/lines.fsh");

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);

		Scene::Instance()->BuildQuadtree();

		m_font = new Font("data/fonts/inconsolata.fnt", "data/textures/fonts/");

		m_hungerText = new TextBox(200.0f, 200.0f);
		m_hungerText->SetPos(glm::vec3(20.0f, 500.0f, 0.0f));

		m_hungerText->SetText(m_font, "%f%", m_hunger);
		m_hungerText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_hungerText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_hungerText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_thirstText = new TextBox(200.0f, 200.0f);
		m_thirstText->SetPos(glm::vec3(20.0f, 600.0f, 0.0f));

		m_thirstText->SetText(m_font, "%f%", m_thirst);
		m_thirstText->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
		m_thirstText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_thirstText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_flashlight = new Model("data/models/flashlight.fbx", "dim");
		m_flashlight->setPosition(glm::vec3(80, -50, -200));
		m_flashlight->setRotation(glm::vec3(0.0f, DEG_TO_RAD(180), 0.0f));

		m_drinkText = new TextBox(700.0f, 200.0f);
		m_drinkText->SetPos(glm::vec3(400.0f, 50.0f, 0.0f));

		m_drinkText->SetText(m_font, m_drinkPrompt.c_str());
		m_drinkText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_drinkText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_drinkText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_eatText = new TextBox(700.0f, 200.0f);
		m_eatText->SetPos(glm::vec3(400.0f, 50.0f, 0.0f));

		m_eatText->SetText(m_font, m_eatPrompt.c_str());
		m_eatText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_eatText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_eatText->SetVerticalAlignment(TextBox::Alignment::AL_Top);
		// m_pFlashlight->setRotation(180, glm::vec3(0.0f, 1.0f, 0.0f));

		m_spotlight = new Light();
		m_spotlight->posRange = glm::vec4(0.0f, 0.0f, 0.0f, 100.0f);
		m_spotlight->color = glm::vec3(0.005f, 0.005f, 0.005f);
		m_spotlight->attenuation = glm::vec3(0.0f, 0.5f, 0.0f);
		m_spotlight->lightSpot = glm::vec4(2.0f, 2.0f, 2.0f, 0.8f);
		m_spotlight->enabled = false;

		m_flashlight->attachLight(m_spotlight);
		m_flashlight->setTexture("data/textures/flashlight.png");
		m_models.push_back(m_flashlight);

		for (int i = 0; i < 30; i++)
		{
			Model *m_pCreature = new Model("data/models/low_poly_spitter.obj", "skinned");
			m_pCreature->setTag("enemy");
			m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
			m_pCreature->setNormal("data/textures/gimpy_normal.tga");
			m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin());
			// m_pCreature->attachRigidBody("data/physics/creature.rigid");

			Light *pointLight = new Light();
			float r = _randomFloat(0.0001f, 0.0018f);
			float g = _randomFloat(0.0001f, 0.0028f);
			float b = _randomFloat(0.0001f, 0.0038f);
			pointLight->color = glm::vec3(r, g, b);
			pointLight->attenuation = glm::vec3(0.9f, 0.9f, 0.9f);
			pointLight->posRange = glm::vec4(0.0f, 0.0f, 0.0f, 3.0f);
			m_lights.push_back(pointLight);
			m_pCreature->attachLight(pointLight);
			int scale = _randomNum(2, 5);
			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(0, m_terrainGenerator->GetSize());

			int z = _randomNum(0, m_terrainGenerator->GetSize());

			m_pCreature->setScale(glm::vec3(scale, scale, scale));
			m_pCreature->translate(glm::vec3(x, 0.0f, z));
			m_pCreature->setRotation(glm::vec3(0.0f, rotation, 0.0f));
			m_models.push_back(m_pCreature);
		}
		for (int i = 0; i < 10; i++)
		{
			Model *bush = new Model("data/models/shrub.fbx", "skinned");
			bush->setTag("food");
			bush->setTexture("data/textures/shrub.png");
			bush->setOffset(bush->getModel()->getAABBMin());

			int scale = _randomNum(2, 5);
			float rotation = (float)_randomNum(-60, 60);
			int x = _randomNum(200, 600);
			int z = _randomNum(200, 600);

			bush->setScale(glm::vec3(0.01f, 0.01f, 0.01f));

			bush->translate(glm::vec3(x, 0.0f, z));
			bush->setRotation(glm::vec3(0.0f, rotation, 0.0f));
			m_models.push_back(bush);
		}
		m_soundManager = new wolf::SoundManager();
		m_soundManager->CreateSoundSystem();
		m_soundManager->Play2D("Nature", m_natureSoundPath, true);
		m_soundManager->Play3D("Water", m_waterSoundPath, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, true);

		// Debug Menu
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_app->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 430 core");

		m_worldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_terrainGenerator = new TerrainGenerator();

		m_terrainSize = m_terrainGenerator->GetSize();
		m_terrainVerts = m_terrainGenerator->GetVertexCount();
		m_terrainOctaves = m_terrainGenerator->GetOctaves();
		m_terrainRoughness = m_terrainGenerator->GetRoughness();
		m_terrainAmplitude = m_terrainGenerator->GetAmplitude();

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Terrain *terrain = new Terrain(i, j, m_terrainGenerator);
				m_terrains.push_back(terrain);
				Scene::Instance()->AddNode(terrain);
			}
		}

		// Water
		m_water = new Water();
		m_water->SetScale(glm::vec3(500.0f, 500.0f, 500.0f));
		m_water->SetPos(glm::vec3(300.0f, 5.0f, 300.0f));
		Scene::Instance()->AddNode(m_water);

		m_skybox = new Skybox(m_skyboxPath);
		Scene::Instance()->AddNode(m_skybox);
	}
}

void StateGameplay::Update(float p_fDelta)
{
	Camera *camera = Scene::Instance()->GetActiveCamera();

	glm::vec3 lightspot = glm::normalize(camera->GetViewDirection() - glm::vec3(m_spotlight->posRange.x, m_spotlight->posRange.y, m_spotlight->posRange.z));
	m_spotlight->lightSpot = glm::vec4(lightspot, 0.2f);

	wolf::BulletPhysicsManager::Instance()->Update(p_fDelta);

	m_hunger = glm::max(m_hunger - (p_fDelta * (rand() % 3 + 2)), 0.0f);
	m_thirst = glm::max(m_thirst - p_fDelta * (rand() % 2 + 1), 0.0f);

	int hunger = (int)m_hunger;
	int thirst = (int)m_thirst;

	glm::vec3 camPosition = camera->GetPosition();

	m_hungerText->SetText(m_font, "%d%", hunger);
	if (m_hunger >= 70)
		m_hungerText->SetColor(0.0f, 1.0f, 0.0f, 0.6f);
	else if (m_hunger >= 40)
		m_hungerText->SetColor(1.0f, 1.0f, 0.0f, 0.6f);
	else
		m_hungerText->SetColor(1.0f, 0.0f, 0.0f, 0.6f);

	if (m_thirst >= 70)
		m_thirstText->SetColor(0.0f, 1.0f, 0.0f, 0.6f);
	else if (m_thirst >= 40)
		m_thirstText->SetColor(1.0f, 1.0f, 0.0f, 0.6f);
	else
		m_thirstText->SetColor(1.0f, 0.0f, 0.0f, 0.6f);

	m_thirstText->SetText(m_font, "%d%", thirst);

	m_terrainGenerator->SetSize(m_terrainSize);
	m_terrainGenerator->SetAmplitude(m_terrainAmplitude);
	m_terrainGenerator->SetOctaves(m_terrainOctaves);
	m_terrainGenerator->SetVertexCount(m_terrainVerts);
	m_terrainGenerator->SetRoughness(m_terrainRoughness);
	m_soundManager->SetListenerPosition(camera->GetPosition(), camera->GetViewDirection());

	for (Model *model : m_models)
	{
		model->update(p_fDelta);
	}

	if (m_app->isKeyDown('F') && !m_keyDown)
	{
		m_keyDown = true;
		m_soundManager->Play2D("flashlight", m_flashlightSoundPath, false, true);
		m_flashlightEquipped = !m_flashlightEquipped;
		m_spotlight->enabled = !m_spotlight->enabled;
		if (!m_spotlight->enabled)
		{
			m_spotlight->attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			m_spotlight->attenuation = glm::vec3(0.0f, 0.5f, 0.0f);
		}
	}

	if (!m_app->isKeyDown('F'))
		m_keyDown = false;

	m_nearFood = false;
	for (Model *model : m_models)
	{

		if (model->getTag().compare("enemy") == 0)
		{
			// Model is an enemy
			glm::vec3 modelPos = model->getPosition();
			glm::vec3 playerPos = camera->GetPosition();

			if (Util::inProximity(modelPos, playerPos, m_enemyRange))
			{
				float xDist = playerPos.x - modelPos.x;
				float zDist = playerPos.z - modelPos.z;
				model->setPosition(modelPos + glm::vec3(xDist * p_fDelta * m_enemySpeed, 0.0f, zDist * p_fDelta * m_enemySpeed));

				// Rotate in direction of player
				model->setRotation(glm::vec3(0.0f, glm::degrees(atan2(-zDist, xDist)) + 90.0f, 0.0f));
			}
			else
			{
				bool rand_bool = Util::randBool();
				if (rand_bool)
				{
					float xDist = Util::randNum(0.0f, 5.0f);
					float zDist = Util::randNum(0.0f, 5.0f);
					model->setPosition(modelPos + glm::vec3(xDist * p_fDelta * m_enemySpeed, 0.0f, zDist * p_fDelta * m_enemySpeed));
				}
			}
		}

		// CHECK IF NEAR FOOD
		if (model->getTag().compare("food") == 0)
		{
			// Model is an enemy
			glm::vec3 modelPos = model->getPosition();
			glm::vec3 playerPos = camera->GetPosition();

			if (Util::inProximity(modelPos, playerPos, m_foodRange))
			{
				m_nearFood = true;
			}
		}
	}

	// CHECK IF NEAR WATER
	m_nearWater = Util::inProximity(m_water->GetPos(), camera->GetPosition(), m_water->GetScale());

	if (m_nearWater && m_app->isKeyDown('E') && !m_drinking)
	{
		m_drinking = true;
		m_soundManager->Play2D("drinking", m_drinkingSoundPath, false, true);

		m_thirst += 10.0f;
		if (m_thirst > 100.0f)
			m_thirst = 100.0f;
	}

	if (!m_app->isKeyDown('E'))
		m_drinking = false;

	if (m_nearFood && m_app->isKeyDown('E') && !m_eating)
	{
		m_eating = true;
		m_soundManager->Play2D("eating", m_eatingSoundPath, false, true);

		m_hunger = wolf::max(100.0f, m_hunger + 10.0f);
	}

	if (!m_app->isKeyDown('E'))
	{
		m_drinking = false;
		m_eating = false;
	}

	if (m_hunger <= 0.0f && m_thirst <= 0.0f)
	{
		m_stateMachine->GoToState(eStateGameplay_Respawn);
	}
}

void StateGameplay::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
	glm::vec2 dimensions = m_app->getScreenSize();
	int width = dimensions.x;
	int height = dimensions.y;

	Camera *camera = Scene::Instance()->GetActiveCamera();

	m_worldProgram->SetUniform("u_lightDir", m_sunDirection);
	m_worldProgram->SetUniform("u_viewPos", camera->GetViewDirection());

	m_hungerText->Render(glm::ortho(0.0f, (float)dimensions.x, (float)dimensions.y, 0.0f), glm::mat4(1.0f));
	m_thirstText->Render(glm::ortho(0.0f, (float)dimensions.x, (float)dimensions.y, 0.0f), glm::mat4(1.0f));

	for (Model *model : m_models)
	{

		std::sort(m_lights.begin(), m_lights.end(), [this, model](const Light *lhs, const Light *rhs) -> bool
				  { return _isEffectiveLight(lhs, rhs, model); });

		for (int i = 0; i < 4; i++)
		{
			// Point lights
			Light *pLight = m_lights[i];
			if (pLight->enabled)
			{
				m_worldProgram->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->posRange);
				m_worldProgram->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->color);
				m_worldProgram->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->attenuation);
				model->getMaterial()->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->posRange);
				model->getMaterial()->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->color);
				model->getMaterial()->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->attenuation);
			}
		}

		// Spot Light
		if (m_spotlight->enabled)
		{
			m_worldProgram->SetUniform("u_lightPosRange", m_spotlight->posRange);
			m_worldProgram->SetUniform("u_lightColor", m_spotlight->color);
			m_worldProgram->SetUniform("u_lightSpot", m_spotlight->lightSpot);
			m_worldProgram->SetUniform("u_lightAttenuation", m_spotlight->attenuation);

			model->getMaterial()->SetUniform("u_lightPosRange", m_spotlight->posRange);
			model->getMaterial()->SetUniform("u_lightColor", m_spotlight->color);
			model->getMaterial()->SetUniform("u_lightSpot", m_spotlight->lightSpot);
			model->getMaterial()->SetUniform("u_lightAttenuation", m_spotlight->attenuation);
		}
		else
		{
			m_worldProgram->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			m_worldProgram->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			m_worldProgram->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));

			model->getMaterial()->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			model->getMaterial()->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			model->getMaterial()->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));
		}
		model->render(mProj, mView, camera->GetViewDirection());
	}

	// if (m_pCam)
	// 	m_skybox->Render(mProj, mView, width, height);

	// m_water->render(mProj, mView, width, height);

	// for (Terrain *terrain : m_terrains)
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
	// 	delete m_terrainGenerator;
	// 	m_terrainGenerator = new TerrainGenerator();
	// 	m_terrainGenerator->SetSize(m_terrainSize);
	// 	m_terrainGenerator->SetAmplitude(m_terrainAmplitude);
	// 	m_terrainGenerator->SetOctaves(m_terrainOctaves);
	// 	m_terrainGenerator->SetVertexCount(m_terrainVerts);
	// 	m_terrainGenerator->SetRoughness(m_terrainRoughness);
	// 	_renderTerrain();
	// }
	// ImGui::End();
	// ImGui::Render();
	// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// if (m_app->isKeyDown('F') && !m_bKeyDown)
	// {
	// 	m_bKeyDown = true;
	// 	m_bFlashlightEquipped = !m_bFlashlightEquipped;
	// 	m_spotlight->bEnabled = !m_spotlight->bEnabled;
	// 	if (!m_spotlight->bEnabled)
	// 	{
	// 		m_spotlight->vAttenuation = glm::vec3(1.0f, 1.0f, 1.0f);
	// 	}
	// 	else
	// 	{
	// 		m_spotlight->vAttenuation = glm::vec3(0.0f, 0.5f, 0.0f);
	// 	}
	// }

	// if (!m_app->isKeyDown('F'))
	// 	m_bKeyDown = false;

	if (m_app->isKeyDown('W') || m_app->isKeyDown('A') || m_app->isKeyDown('D') || m_app->isKeyDown('S'))
	{
		if (!m_walking && !m_running)
		{
			m_soundManager->Play2D("walking", m_walkingSoundPath, true, true);
			m_walking = true;
		}

		if (m_app->isKeyDown(GLFW_KEY_LEFT_CONTROL) && !m_running)
		{
			m_soundManager->Play2D("running", m_runningSoundPath, true, true);
			m_walking = false;
			m_running = true;
		}
	}
	if (!m_app->isKeyDown('W') && !m_app->isKeyDown('A') && !m_app->isKeyDown('D') && !m_app->isKeyDown('S'))
	{

		m_walking = false;
		m_running = false;
		ISound *walking = m_soundManager->GetSound("walking");
		ISound *running = m_soundManager->GetSound("running");
		if (walking && !walking->getIsPaused())
			m_soundManager->PauseSound("walking");
		if (running && !running->getIsPaused())
			m_soundManager->PauseSound("running");
	}

	if (m_flashlightEquipped)
		m_flashlight->render(mProj, mView);

	m_hungerText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	m_thirstText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	if (m_nearWater)
	{
		m_drinkText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	}
	if (m_nearFood)
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

	if (pLight1->enabled && !pLight2->enabled)
		return true;

	if (!pLight1->enabled || (!pLight2->enabled && !pLight1->enabled))
		return false;

	float fLight1Dist = glm::distance(glm::vec3(pLight1->posRange.x, pLight1->posRange.y, pLight1->posRange.z), pModel->getPosition());
	float fLight2Dist = glm::distance(glm::vec3(pLight2->posRange.x, pLight2->posRange.y, pLight2->posRange.z), pModel->getPosition());

	return glm::any(glm::lessThan(pLight1->attenuation, pLight2->attenuation)) && (fLight1Dist < fLight2Dist);
}

void StateGameplay::_renderTerrain()
{
	for (Terrain *terrain : m_terrains)
	{
		delete terrain;
	}
	m_terrains.clear();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Terrain *terrain = new Terrain(i, j, m_terrainGenerator);
			m_terrains.push_back(terrain);
		}
	}
}