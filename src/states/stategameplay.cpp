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
	delete m_gravityGun;
	delete m_spotlight;
	delete m_miniCamera;
	delete m_shipPrompt;
	delete m_gravityForcefield;
	delete m_scannerEffect;
	delete m_gravityItem;

	for (Model *shipPart : m_shipParts)
	{
		delete shipPart;
	}

	wolf::BufferManager::DestroyBuffer(m_frameBuffer);
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
	delete m_collectText;
	delete m_repairShipText;
	delete m_collectMorePartsText;
	delete m_partsCollectedText;
	delete m_stateMachine;
	delete m_playerIcon;

	delete m_soundManager;

	for (const auto &element : m_terrainMap)
	{
		delete element.second;
	}

	std::vector<Water *> waters;
	transform(m_waterMap.begin(), m_waterMap.end(), back_inserter(waters),
			  [](const std::map<std::pair<int, int>, Water *>::value_type &val)
			  { return val.second; });

	for (Water *water : waters)
	{
		delete water;
	}

	for (Model *model : m_models)
	{
		delete model;
	}

	for (Light *light : m_lights)
	{
		delete light;
	}

	for (Effect *effect : m_effects)
	{
		delete effect;
	}

	for (std::thread &thread : m_threads)
	{
		thread.join();
	}

	delete m_blueTrailEffect;
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

		srand(time(NULL));
		wolf::BulletPhysicsManager::CreateInstance("data/physics/physics_materials.xml",
												   "data/shaders/lines.vsh",
												   "data/shaders/lines.fsh");
		glm::vec2 screenSize = m_app->getScreenSize();
		m_frameBuffer = wolf::BufferManager::CreateFrameBuffer(screenSize.x, screenSize.y);

		m_miniCamera = new OrthoCamera(m_app);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);

		m_font = new Font("data/fonts/inconsolata.fnt", "data/textures/fonts/");

		m_hungerText = new TextBox(200.0f, 200.0f);

		m_hungerText->SetText(m_font, "%f%", m_hunger);
		m_hungerText->SetColor(0.996f, 0.760f, 0.576f, 0.6f);
		m_hungerText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_hungerText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_thirstText = new TextBox(200.0f, 200.0f);

		m_thirstText->SetText(m_font, "%f%", m_thirst);
		m_thirstText->SetColor(0.0f, 0.0f, 1.0f, 0.6f);
		m_thirstText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_thirstText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_flashlight = new Model("data/models/flashlight.fbx", "dim");
		m_flashlight->setPosition(glm::vec3(80, -50, -200));
		m_flashlight->setRotation(glm::vec3(0.0f, 180, 0.0f));

		m_gravityGun = new Model("data/models/gravity-gun.obj", "dim");
		m_gravityGun->setPosition(glm::vec3(7.3, -6, -9.7));
		m_gravityGun->setRotation(glm::vec3(-180.0f, 0.0f, 0.0f));
		m_gravityGun->setScale(glm::vec3(5.0f, 5.0f, 5.0f));
		m_gravityGun->setTexture("data/textures/gravity-gun.png");

		m_blueTrailEffect = new Effect(m_blueTrail);
		m_blueTrailEffect->setPos(glm::vec3(12.195, -7.3, -32.927));

		m_drinkText = new TextBox(700.0f, 200.0f);
		m_drinkText->SetPos(glm::vec3(50.0f, 50.0f, 0.0f));

		m_drinkText->SetText(m_font, m_drinkPrompt.c_str());
		m_drinkText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_drinkText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_drinkText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_eatText = new TextBox(700.0f, 200.0f);
		m_eatText->SetPos(glm::vec3(50.0f, 200.0f, 0.0f));

		m_eatText->SetText(m_font, m_eatPrompt.c_str());
		m_eatText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_eatText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_eatText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_collectText = new TextBox(1000.0f, 200.0f);
		m_collectText->SetPos(glm::vec3(50.0f, 50.0f, 0.0f));
		m_collectText->SetText(m_font, m_collectPrompt.c_str());
		m_collectText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_collectText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_collectText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_repairShipText = new TextBox(1000.0f, 200.0f);
		m_repairShipText->SetPos(glm::vec3(100.0f, 50.0f, 0.0f));
		m_repairShipText->SetText(m_font, m_repairPrompt.c_str());
		m_repairShipText->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
		m_repairShipText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_repairShipText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_collectMorePartsText = new TextBox(500.0f, 200.0f);
		m_collectMorePartsText->SetPos(glm::vec3(50.0f, 50.0f, 0.0f));
		m_collectMorePartsText->SetText(m_font, m_noPartsPrompt.c_str());
		m_collectMorePartsText->SetOutlined(true);
		m_collectMorePartsText->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_collectMorePartsText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_collectMorePartsText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		m_playerIcon = new Triangle(m_playerIconPath);
		m_playerIcon->setScale(glm::vec2(0.07f, 0.07f));

		m_spotlight = new Light();
		m_spotlight->posRange = glm::vec4(0.0f, 0.0f, 0.0f, 100.0f);
		m_spotlight->color = glm::vec3(0.45f, 0.54f, 0.4f);
		m_spotlight->attenuation = glm::vec3(0.0f, 0.5f, 0.0f);
		m_spotlight->lightSpot = glm::vec4(2.0f, 2.0f, 2.0f, 0.8f);
		m_spotlight->enabled = false;

		// m_flashlight->attachLight(m_spotlight);
		m_flashlight->setTexture("data/textures/flashlight.png");

		m_terrainGenerator = new TerrainGenerator();

		m_terrainSize = m_terrainGenerator->GetSize();
		m_terrainVerts = m_terrainGenerator->GetVertexCount();
		m_terrainOctaves = m_terrainGenerator->GetOctaves();
		m_terrainRoughness = m_terrainGenerator->GetRoughness();
		m_terrainAmplitude = m_terrainGenerator->GetAmplitude();

		_generateTerrain(0, 0);

		// Ship and ship parts
		m_ship = new Model("data/models/ships/ship3.obj", "skinned");
		m_ship->setTag("ship");
		m_ship->setScale(glm::vec3(10.0f, 10.0f, 10.0f));
		m_ship->setOffset(m_ship->getModel()->getAABBMin() + glm::vec3(0.0f, -5.0f, 0.0f));
		m_ship->setTexture("data/textures/ship-texture.png");

		glm::vec3 shipPosition = Scene::Instance()->GetActiveCamera()->GetPosition() + glm::vec3(-150.0f, 0.0f, 0.0f);
		m_ship->setPosition(glm::vec3(shipPosition.x, m_terrainGenerator->GetHeight(int(shipPosition.x), int(shipPosition.z)), shipPosition.z));

		Effect *fire1 = new Effect(m_firepath);
		Effect *fire2 = new Effect(m_firepath);
		Effect *fire3 = new Effect(m_firepath);
		Effect *fire4 = new Effect(m_firepath);
		Effect *fire5 = new Effect(m_firepath);
		for (int i = 0; i < 7; i++)
		{
			Effect *fire = new Effect(m_firepath);

			int xOff = _randomNum(-40, 50);
			int yOff = _randomNum(0, 7);
			int zOff = _randomNum(-50, 70);
			fire->setPos(m_ship->getPosition() + glm::vec3(xOff, yOff, zOff));
			m_effects.push_back(fire);
		}
		m_gravityForcefield = new Effect(m_forcefieldPath);
		m_models.push_back(m_ship);

		// Ship Parts
		Model *m_part1 = new Model("data/models/ships/ship-parts/engine.obj", "super-dim");
		m_part1->setTag("ship-part");
		m_part1->setScale(glm::vec3(5.0f, 5.0f, 5.0f));
		// m_part1->setOffset(m_part1->getModel()->getAABBMin());
		m_part1->setTexture("data/textures/ship-texture.png");

		Model *m_part2 = new Model("data/models/ships/ship-parts/part3-powerbank.obj", "super-dim");
		m_part2->setTag("ship-part");
		// m_part2->setOffset(m_part2->getModel()->getAABBMin());
		m_part2->setTexture("data/textures/leather.png");
		m_part2->setScale(glm::vec3(5.0f, 5.0f, 5.0f));

		Model *m_part3 = new Model("data/models/ships/ship-parts/plane_door.obj", "super-dim");
		m_part3->setTag("ship-part");
		// m_part3->setOffset(m_part3->getModel()->getAABBMin());
		m_part3->setTexture("data/textures/ship-texture.png");
		m_part3->setRotation(glm::vec3(40.0f, 0.0f, 0.0f));
		m_part3->setScale(glm::vec3(0.05f, 0.05f, 0.05f));

		m_shipParts.push_back(m_part1);
		m_shipParts.push_back(m_part2);
		m_shipParts.push_back(m_part3);

		m_numParts = 1;

		// parts collected label
		m_partsCollectedText = new TextBox(500.0f, 200.0f);

		std::string partsString = "Repaired: " + std::to_string(m_repairedParts) + "/" + std::to_string(m_numParts);
		m_partsCollectedText->SetText(m_font, partsString.c_str());
		m_partsCollectedText->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
		m_partsCollectedText->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_partsCollectedText->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		int x = _randomNum(-m_terrainSize * 2, m_terrainSize * 2);
		int z = _randomNum(-m_terrainSize * 2, m_terrainSize * 2);
		m_part1->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));

		x = _randomNum(-m_terrainSize * 2, m_terrainSize * 2);
		z = _randomNum(-m_terrainSize * 2, m_terrainSize * 2);
		m_part2->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z) + 5.0f, z));

		x = _randomNum(-m_terrainSize * 2, m_terrainSize * 2);
		z = _randomNum(-m_terrainSize * 2, m_terrainSize * 2);
		m_part3->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));

		m_models.push_back(m_part1);
		m_models.push_back(m_part2);
		m_models.push_back(m_part3);

		for (Model *shipPart : m_shipParts)
		{
			// Fire near ship parts
			int xOff = _randomNum(-5, 5);
			int zOff = _randomNum(-10, 10);

			Effect *fire1 = new Effect(m_firepath);
			glm::vec3 newPos = shipPart->getPosition() + glm::vec3(xOff, 0.0f, zOff);
			fire1->setPos(glm::vec3(newPos.x, m_terrainGenerator->GetHeight(newPos.x, newPos.z), newPos.z));
			m_effects.push_back(fire1);

			xOff = _randomNum(-5, 5);
			zOff = _randomNum(-10, 10);

			Effect *fire2 = new Effect(m_firepath);
			newPos = shipPart->getPosition() + glm::vec3(xOff, 0.0f, zOff);
			fire2->setPos(glm::vec3(newPos.x, m_terrainGenerator->GetHeight(newPos.x, newPos.z), newPos.z));
			m_effects.push_back(fire2);
		}

		m_soundManager = new wolf::SoundManager();
		m_soundManager->CreateSoundSystem();

		// m_soundManager->Play3D("fire", m_fireSound, m_ship->getPosition(), 10.0f, true);
		m_soundManager->Play2D("Nature", m_natureSoundPath, true);

		// For Minimap
		m_shipPrompt = new TextBox(700.0f, 200.0f);
		m_shipPrompt->SetPos(glm::vec3(50.0f, 50.0f, 0.0f));
		m_shipPrompt->SetColor(0.0f, 1.0f, 1.0f, 1.0f);
		m_shipPrompt->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_shipPrompt->SetVerticalAlignment(TextBox::Alignment::AL_Top);

		std::string shipDistance = "Ship: " + std::to_string(m_shipDistance) + "m Away";
		m_shipPrompt->SetText(m_font, shipDistance.c_str());

		// Debug Menu
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_app->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 430 core");

		m_worldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_skybox = new Skybox(m_skyboxPath);
		Scene::Instance()->AddNode(m_skybox);

		Scene::Instance()->BuildQuadtree();
	}
}

void StateGameplay::Update(float p_fDelta)
{

	if (m_timer > 0.0f)
	{
		m_timer = glm::max(m_timer - p_fDelta, 0.0f);
	}

	Camera *camera = Scene::Instance()->GetActiveCamera();

	glm::vec2 dimensions = m_app->getScreenSize();
	float width = dimensions.x;
	float height = dimensions.y;
	m_nearShip = false;
	m_nearCollectible = false;
	m_nearFood = false;

	// Was attacked
	if (m_force > 0.0f)
	{
		m_force = glm::max(m_force - (p_fDelta * 5.0f), 0.0f);
		camera->SetPosition(camera->GetPosition() + glm::vec3(m_force, 0.0f, 0.0f));
	}

	// Letting go of enemy
	if (m_gravityItem && !m_usingGravityGun)
	{
		glm::vec3 gravityItemPos = m_gravityItem->getPosition();
		if (gravityItemPos.y > m_terrainGenerator->GetHeight(gravityItemPos.x, gravityItemPos.z))
		{
			m_gravityItem->setPosition(m_gravityItem->getPosition() - glm::vec3(0.0f, p_fDelta * 200.0f, 0.0f));
		}
		else
		{
			m_gravityItem = nullptr;
			m_gravityGunForce = 0.0f;
		}
	}

	// Text boxes
	m_partsCollectedText->SetPos(glm::vec3(width - m_miniWidth, height - height / 2, 0.0f));

	m_thirstText->SetPos(glm::vec3(width / 14, height - height / 8, 0.0f));
	m_hungerText->SetPos(glm::vec3(width / 14, height - height / 4, 0.0f));

	m_miniCamera->SetRotation(glm::vec2(camera->GetRotation().x, -90.01f));
	m_miniCamera->SetPosition(glm::vec3(camera->GetPosition().x, 500.0f, camera->GetPosition().z));

	// m_miniCamera->Update(p_fDelta);
	m_shipDistance = (int)glm::distance(camera->GetPosition(), m_ship->getPosition());
	std::string shipDistance = "Ship: " + std::to_string(m_shipDistance) + "m Away";
	m_shipPrompt->SetText(m_font, shipDistance.c_str());
	m_shipPrompt->SetPos(glm::vec3(width / 14, height - height / 8, 0.0f));

	// glViewport(screenSize.x - screenSize.x / 4, screenSize.y - screenSize.y / 4, screenSize.x / 4, screenSize.y / 4);

	// Set camera to floorcamera->GetPosition() + glm::vec3(0.0f, 700.0f, 0.0f)
	float camX = camera->GetPosition().x;
	float camY = camera->GetPosition().y;
	float camZ = camera->GetPosition().z;

	if (!m_bCinematic)
	{
		camera->SetPosition(glm::vec3(camX, m_terrainGenerator->GetHeight(int(camX), int(camZ)) + 5.0f, camZ));
	}
	// Attach spotlight
	m_spotlight->posRange = glm::vec4(camX, camY, camZ, m_spotlight->posRange.w);

	glm::vec3 lightspot = glm::normalize(camera->GetViewDirection() * glm::vec3(m_spotlight->posRange.x, m_spotlight->posRange.y, m_spotlight->posRange.z));
	m_spotlight->lightSpot = glm::vec4(lightspot, 0.2f);

	wolf::BulletPhysicsManager::Instance()->Update(p_fDelta);

	m_hunger = glm::max(m_hunger - (p_fDelta * (rand() % 2)), 0.0f);
	m_thirst = glm::max(m_thirst - p_fDelta * (rand() % 2), 0.0f);

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
		glm::vec3 oldModelPos = model->getPosition();

		if (model->isDestroyed())
			continue;

		if (model->getTag().compare("ship-part") == 0 && Util::inProximity(model->getPosition(), camera->GetPosition(), glm::vec3(30.0f, 30.0f, 30.0f)))
		{
			m_nearCollectible = true;
			// m_soundManager->Play3D("fire", m_fireSound, model->getPosition(), 30.0f, true);

			if (m_inventoryFull)
			{
				std::string prompt = "Full Inventory";
				m_collectText->SetText(m_font, prompt.c_str());
			}
			else
			{
				m_collectText->SetText(m_font, m_collectPrompt.c_str());
			}

			if (m_app->isKeyDown('E') && !m_inventoryFull)
			{
				m_models.erase(std::remove(m_models.begin(), m_models.end(), model), m_models.end());
				m_shipParts.erase(std::remove(m_shipParts.begin(), m_shipParts.end(), model), m_shipParts.end());

				delete model;
				m_inventoryFull = true;

				std::string partsString = "Repaired: " + std::to_string(m_repairedParts) + "/" + std::to_string(m_numParts);
				m_partsCollectedText->SetText(m_font, partsString.c_str());

				m_soundManager->Play2D("collected", m_pickupSoundPath);
				m_nearCollectible = false;
				continue;
			}
		}

		if (model->getTag().compare("ship") == 0 && Util::inProximity(model->getPosition(), camera->GetPosition(), glm::vec3(150.0f, 30.0f, 150.0f)))
		{
			m_nearShip = true;

			if (m_app->isKeyDown('E') && m_inventoryFull)
			{
				m_inventoryFull = false;
				m_repairedParts++;
				std::string partsString = "Repaired: " + std::to_string(m_repairedParts) + "/" + std::to_string(m_numParts);
				m_partsCollectedText->SetText(m_font, partsString.c_str());
				m_soundManager->Play2D("repair", m_repairSoundPath);

				m_nearCollectible = false;
				if (m_repairedParts == m_numParts)
				{
					Scene::Instance()->GetStateMachine()->GoToState(eStateGameplay_End);
				}
			}
		}

		if (model != m_gravityItem)
		{

			if (model->getTag().compare("enemy") == 0 || model->getTag().compare("animated-enemy") == 0)
			{
				// Set all enemies to floor
				float modelX = model->getPosition().x;
				float modelZ = model->getPosition().z;
				model->setPosition(glm::vec3(modelX, m_terrainGenerator->GetHeight(int(modelX), int(modelZ)), modelZ));

				float modelY = model->getPosition().y;
				glm::vec3 modelPos = model->getPosition();
				glm::vec3 playerPos = camera->GetPosition();

				if (Util::inProximity(modelPos, playerPos, m_enemyRange))
				{

					if (model->getTag().compare("animated-enemy") == 0)
					{
						wolf::SkinnedModel *skinnedModel = static_cast<wolf::SkinnedModel *>(model->getModel());
						skinnedModel->PlayClip("walk");
					}

					float xDist = playerPos.x - modelPos.x;
					float zDist = playerPos.z - modelPos.z;

					// if (!model->isChasing())
					// 	m_soundManager->Play3D("growl", m_creatureGrowlPath, modelPos, 10.0f);

					model->setChasing(true);

					model->setPosition(modelPos + glm::vec3(xDist * p_fDelta * m_enemySpeed, 0.0f, zDist * p_fDelta * m_enemySpeed));
					// Rotate in direction of player
					model->setRotation(glm::vec3(0.0f, glm::degrees(atan2(-zDist, xDist)) + 90.0f, 0.0f));

					if (Util::inProximity(modelPos, playerPos, glm::vec3(10.0f, 10.0f, 10.0f)))
					{
						m_soundManager->Play2D("pain", m_painSoundPath);
						m_health -= 20.0f;
						m_force = 5.0f;
					}
				}
				else
				{
					model->setChasing(false);
					bool rand_bool = Util::randBool();
					if (rand_bool)
					{
						float xDist = Util::randNum(0.0f, 5.0f);
						float zDist = Util::randNum(0.0f, 5.0f);
						model->setPosition(modelPos + glm::vec3(xDist * p_fDelta * m_enemySpeed, 0.0f, zDist * p_fDelta * m_enemySpeed));
					}
				}
			}
		}

		// CHECK IF NEAR FOOD
		if (model->getTag().compare("food") == 0)
		{
			// Model is food
			glm::vec3 playerPos = camera->GetPosition();

			if (Util::inProximity(oldModelPos, playerPos, m_foodRange))
			{
				m_nearFood = true;
			}
		}
	}

	if (m_app->isKeyDown('F') && !m_keyDown)
	{
		m_keyDown = true;
		m_soundManager->Play2D("flashlight", m_flashlightSoundPath, false, true);
		m_flashlightEquipped = !m_flashlightEquipped;
		m_gravityGunEquipped = false;
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

	if (m_app->isKeyDown('P') && !m_scannerButtonDown && !m_scannerEffect && m_timer == 0.0f)
	{
		m_scannerButtonDown = true;
		m_soundManager->Play2D("shipBeep", m_shipBeepSoundPath, false, true);
		m_scannerEffect = new Effect(m_forcefieldPath);
		m_scannerEffect->setPos(camera->GetPosition());
		m_timer = 10.0f;
	}

	if (!m_app->isKeyDown('P'))
		m_scannerButtonDown = false;

	if (m_app->isKeyDown('C') && !m_bCinematicButtonDown)
	{
		m_bCinematicButtonDown = true;
		m_bCinematic = !m_bCinematic;
	}

	if (!m_app->isKeyDown('C'))
		m_bCinematicButtonDown = false;

	if (m_scannerEffect)
	{
		if (m_scannerEffect->getRadius() >= 300.0f)
		{
			m_scannerEffect = nullptr;
		}
		else
		{
			m_scannerEffect->setRadius(m_scannerEffect->getRadius() + p_fDelta * 80.0f);
		}
	}

	if (m_app->isKeyDown('G') && !m_gravityKeyDown)
	{
		m_gravityKeyDown = true;
		if (!m_gravityGunEquipped)
			m_soundManager->Play2D("gravityGun", m_gravityGunSoundPath, false, true);
		else
		{
			m_soundManager->Play2D("gravityShutdown", m_gravityGunShutdownSoundPath, false, true);
		}
		m_gravityGunEquipped = !m_gravityGunEquipped;
		m_flashlightEquipped = false;
		m_spotlight->enabled = false;
	}

	if (!m_app->isKeyDown('G'))
		m_gravityKeyDown = false;

	int chunkX = camera->GetPosition().x / m_terrainSize;
	int chunkZ = camera->GetPosition().z / m_terrainSize;
	if (!_inRange(chunkX + 2, chunkZ + 2) ||
		!_inRange(chunkX - 2, chunkZ - 2) ||
		!_inRange(chunkX, chunkZ + 2) ||
		!_inRange(chunkX, chunkZ - 2) ||
		!_inRange(chunkX + 2, chunkZ) ||
		!_inRange(chunkX - 2, chunkZ) ||
		!_inRange(chunkX - 2, chunkZ + 2) ||
		!_inRange(chunkX + 2, chunkZ - 2))
	{
		_generateTerrain(chunkX, chunkZ);
		// std::thread terrainThread(&StateGameplay::_generateTerrain, this, chunkX, chunkZ);
		// terrainThread.detach();
		// m_threads.push_back(std::move(terrainThread));
	}

	std::vector<Water *> waters;
	transform(m_waterMap.begin(), m_waterMap.end(), back_inserter(waters),
			  [](const std::map<std::pair<int, int>, Water *>::value_type &val)
			  { return val.second; });

	// CHECK IF NEAR WATER (NOT THE BEST WAY)
	int i = 0;
	if (m_nearWater)
	{
		m_nearWater = Util::inProximity(waters[m_waterIndex]->GetPos(), camera->GetPosition(), glm::vec3(m_terrainSize, 3.0f, m_terrainSize));
	}
	else
	{
		for (Water *water : waters)
		{
			m_nearWater = m_nearWater || Util::inProximity(water->GetPos(), camera->GetPosition(), glm::vec3(m_terrainSize, 3.0f, m_terrainSize));
			if (Util::inProximity(water->GetPos(), camera->GetPosition(), glm::vec3(m_terrainSize, 3.0f, m_terrainSize)))
			{
				m_waterIndex = i;
			}
			i++;
		}
	}

	if (m_nearWater && m_app->isKeyDown('E') && !m_drinking)
	{
		m_drinking = true;
		m_soundManager->Play2D("drinking", m_drinkingSoundPath, false, true);

		m_thirst = glm::min(100.0f, m_thirst + 10.0f);
	}

	if (!m_app->isKeyDown('E'))
		m_drinking = false;

	if (m_nearFood && m_app->isKeyDown('E') && !m_eating)
	{
		m_eating = true;
		m_soundManager->Play2D("eating", m_eatingSoundPath, false, true);

		m_hunger = glm::min(100.0f, m_hunger + 10.0f);
	}

	if (!m_app->isKeyDown('E'))
	{
		m_drinking = false;
		m_eating = false;
	}

	if ((m_hunger <= 0.0f && m_thirst <= 0.0f) || m_health <= 0)
	{
		Scene::Instance()->GetStateMachine()->GoToState(eStateGameplay_Respawn);
	}

	if (m_app->isKeyDown('Z') && !m_debugDown)
	{
		wolf::BulletPhysicsManager::Instance()->ToggleDebugRendering();
		m_debugDown = true;
	}

	if (!m_app->isKeyDown('Z'))
	{
		m_debugDown = false;
	}

	// Bullets
	if (m_app->isLMBDown() && !m_leftMouseDown && m_gravityGunEquipped)
	{
		if (m_usingGravityGun)
		{
			// let go of enemy
			m_gravityGunForce = 5.0f;
			m_usingGravityGun = false;
		}
		else
		{
			// Shoot gravity bullet
			Sphere *bullet = new Sphere(1.0f);
			bullet->SetPosition(camera->GetPosition());
			bullet->setTag("projectile");
			bullet->SetColor(glm::vec4(0.0f, 0.5f, 1.0f, 0.5f));
			m_bullets.push_back(bullet);
		}
		m_soundManager->Play2D("gravityGunFaulty", m_gravityGunFaultyPath, false, true);
		m_leftMouseDown = true;
	}

	if (!m_app->isLMBDown())
	{
		m_leftMouseDown = false;
	}

	// Update gravity creature
	if (m_gravityItem)
	{
		// Change creature position;
		glm::vec2 mousePos = m_app->getMousePos();
		glm::vec3 camRotation = camera->GetViewDirection();
		glm::vec3 velocity = camRotation * 50.0f * p_fDelta;

		if (m_gravityItem->getTag() == "enemy" || m_gravityItem->getTag() == "animated-enemy")
			m_gravityItem->setPosition(m_gravityItem->getPosition() + glm::vec3(velocity.x, velocity.y, velocity.z));
		if (m_gravityItem->getTag() == "ship-part" && !m_inventoryFull)
		{
			m_models.erase(std::remove(m_models.begin(), m_models.end(), m_gravityItem), m_models.end());
			m_inventoryFull = true;

			std::string partsString = "Repaired: " + std::to_string(m_repairedParts) + "/" + std::to_string(m_numParts);
			m_partsCollectedText->SetText(m_font, partsString.c_str());

			m_soundManager->Play2D("collected", m_pickupSoundPath);
			m_nearCollectible = false;
			m_usingGravityGun = false;
			m_gravityItem = nullptr;
		}
	}

	// Update bullets
	int index = 0;
	for (Sphere *bullet : m_bullets)
	{
		glm::vec3 camRotation = camera->GetViewDirection();
		// glm::quat radRotation = glm::quat(glm::vec3(DEG_TO_RAD(camRotation.x), DEG_TO_RAD(camRotation.y), DEG_TO_RAD(camRotation.z)));
		glm::vec3 velocity = camRotation * 50.0f * p_fDelta;
		bullet->SetPosition(bullet->GetPosition() + velocity);
		bullet->Update(p_fDelta);

		float distanceToCam = glm::distance(bullet->GetPosition(), camera->GetPosition());

		if (distanceToCam > 100.0f)
		{
			m_bullets.erase(m_bullets.begin() + index);
		}
		index++;
	}

	// Check collision
	if (!m_usingGravityGun)
	{
		index = 0;
		for (Sphere *bullet : m_bullets)
		{
			for (Model *model : m_models)
			{
				if (model->getTag() == "enemy" || model->getTag() == "ship-part" || model->getTag() == "animated-enemy")
				{
					if (Util::inProximity(model->getPosition(), bullet->GetPosition(), glm::vec3(10.0f, 10.0f, 10.0f)))
					{
						m_usingGravityGun = true;
						m_gravityItem = model;
						m_bullets.erase(m_bullets.begin() + index);
					}
				}
			}
			index++;
		}
	}

	// Update Effects
	for (Effect *effect : m_effects)
	{
		effect->update(p_fDelta);
	}

	if (m_scannerEffect)
	{
		m_scannerEffect->update(p_fDelta);
	}

	if (m_usingGravityGun && m_gravityItem)
	{
		m_gravityForcefield->update(p_fDelta);
		m_gravityForcefield->setPos(m_gravityItem->getPosition());
	}
	m_blueTrailEffect->update(p_fDelta);

	Scene::Instance()->BuildQuadtree();
}

void StateGameplay::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
	glm::vec2 dimensions = m_app->getScreenSize();
	int width = dimensions.x;
	int height = dimensions.y;

	Camera *camera = Scene::Instance()->GetActiveCamera();

	m_worldProgram->SetUniform("u_lightDir", Scene::Instance()->GetLightDirection());
	m_worldProgram->SetUniform("u_viewPos", camera->GetViewDirection());

	for (Model *model : m_models)
	{
		if (model->isDestroyed())
			continue;

		// std::sort(m_lights.begin(), m_lights.end(), [this, model](const Light *lhs, const Light *rhs) -> bool
		// 		  { return _isEffectiveLight(lhs, rhs, model); });

		// for (int i = 0; i < 4; i++)
		// {
		// 	// Point lights
		// 	Light *pLight = m_lights[i];
		// 	if (pLight->enabled)
		// 	{
		// 		m_worldProgram->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->posRange);
		// 		m_worldProgram->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->color);
		// 		m_worldProgram->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->attenuation);
		// 		model->getMaterial()->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->posRange);
		// 		model->getMaterial()->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->color);
		// 		model->getMaterial()->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->attenuation);
		// 	}
		// }

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
	}

	glm::vec4 spotLightPosRange = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 spotLightSpot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 spotLightAttenuation = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 spotLightColor = glm::vec3(0.0f, 0.0f, 0.0f);

	if (m_spotlight->enabled)
	{
		spotLightPosRange = m_spotlight->posRange;
		spotLightSpot = m_spotlight->lightSpot;
		spotLightAttenuation = m_spotlight->attenuation;
		spotLightColor = m_spotlight->color;
	}

	Terrain *terrain = m_terrainMap[std::make_pair(camera->GetPosition().x / m_terrainSize, camera->GetPosition().z / m_terrainSize)];

	terrain->getProgram()->SetUniform("u_spotLightPosRange", spotLightPosRange);
	terrain->getProgram()->SetUniform("u_spotLightColor", spotLightColor);
	terrain->getProgram()->SetUniform("u_spotLightSpot", spotLightSpot);
	terrain->getProgram()->SetUniform("u_spotLightAttenuation", spotLightAttenuation);

	wolf::BulletPhysicsManager::Instance()->Render(mProj, mView);

	// Render bullets
	for (Sphere *bullet : m_bullets)
	{
		bullet->Render(glm::mat4(1.0f), mView, mProj);
	}

	if (m_app->isKeyDown('F') && !m_keyDown)
	{
		m_keyDown = true;
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

	// Render Effects
	for (Effect *effect : m_effects)
	{
		effect->render(mProj, mView);
	}

	if (m_usingGravityGun && m_gravityItem)
	{
		m_gravityForcefield->render(mProj, mView);
	}

	for (Model *shipPart : m_shipParts)
	{
		if (shipPart)
			shipPart->Render(mProj, mView);
	}

	m_ship->Render(mProj, mView);
	if (m_flashlightEquipped)
	{
		glDisable(GL_DEPTH_TEST);

		m_flashlight->Render(mProj, glm::mat4(1.0f));
		glEnable(GL_DEPTH_TEST);
	}

	if (m_gravityGunEquipped)
	{
		glDisable(GL_DEPTH_TEST);

		m_gravityGun->Render(mProj, glm::mat4(1.0f));
		m_blueTrailEffect->render(mProj, glm::mat4(1.0f));
		glEnable(GL_DEPTH_TEST);
	}

	if (!m_bCinematic)
	{
		m_hungerText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		m_thirstText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		m_partsCollectedText->Render(glm::ortho(0.0f, (float)dimensions.x, (float)dimensions.y, 0.0f), glm::mat4(1.0f));

		if (m_nearWater)
		{
			m_drinkText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		}
		if (m_nearFood)
		{
			m_eatText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		}

		if (m_nearCollectible)
		{
			m_collectText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		}

		if (m_nearShip)
		{
			if (m_inventoryFull)
			{
				m_repairShipText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
			}
			else
			{
				m_collectMorePartsText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
			}
		}
	}
	// MINIMAP

	glm::vec2 screenSize = m_app->getScreenSize();

	glViewport(0, 0, screenSize.x, screenSize.y);

	m_frameBuffer->Bind();

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// GLfloat pointVertex[] = {screenSize.x / 4, screenSize.y / 4};
	// glEnableClientState(GL_VERTEX_ARRAY);
	// glPointSize(50);
	// glVertexPointer(2, GL_FLOAT, 0, pointVertex);
	// glDrawArrays(GL_POINTS, 0, 1);
	// glDisableClientState(GL_VERTEX_ARRAY);

	const glm::mat4 &miniProj = m_miniCamera->GetProjMatrix(screenSize.x, screenSize.y);
	const glm::mat4 &miniView = m_miniCamera->GetViewMatrix();

	if (!m_bCinematic)
	{

		m_skybox->Render(miniProj, miniView);
		m_ship->Render(miniProj, miniView);

		std::vector<Terrain *> terrains;
		transform(m_terrainMap.begin(), m_terrainMap.end(), back_inserter(terrains),
				  [](const std::map<std::pair<int, int>, Terrain *>::value_type &val)
				  { return val.second; });
		for (Terrain *terrain : terrains)
		{
			terrain->Render(miniProj, miniView);
		}

		for (Effect *effect : m_effects)
		{
			effect->render(miniProj, miniView);
		}

		if (m_scannerEffect)
		{
			m_scannerEffect->render(miniProj, miniView);
		}

		std::vector<Water *> waters;
		transform(m_waterMap.begin(), m_waterMap.end(), back_inserter(waters),
				  [](const std::map<std::pair<int, int>, Water *>::value_type &val)
				  { return val.second; });

		for (Water *water : waters)
		{
			water->Render(miniProj, miniView);
		}
		if (m_timer > 0.0f)
		{
			m_shipPrompt->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		}
	}

	// for (Model *model : m_models)
	// {
	// 	if (model->isDestroyed())
	// 		continue;

	// 	// std::sort(m_lights.begin(), m_lights.end(), [this, model](const Light *lhs, const Light *rhs) -> bool
	// 	// 		  { return _isEffectiveLight(lhs, rhs, model); });

	// 	// for (int i = 0; i < 4; i++)
	// 	// {
	// 	// 	// Point lights
	// 	// 	Light *pLight = m_lights[i];
	// 	// 	if (pLight->enabled)
	// 	// 	{
	// 	// 		m_worldProgram->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->posRange);
	// 	// 		m_worldProgram->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->color);
	// 	// 		m_worldProgram->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->attenuation);
	// 	// 		model->getMaterial()->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->posRange);
	// 	// 		model->getMaterial()->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->color);
	// 	// 		model->getMaterial()->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->attenuation);
	// 	// 	}
	// 	// }

	// 	// Spot Light
	// 	if (m_spotlight->enabled)
	// 	{
	// 		m_worldProgram->SetUniform("u_lightPosRange", m_spotlight->posRange);
	// 		m_worldProgram->SetUniform("u_lightColor", m_spotlight->color);
	// 		m_worldProgram->SetUniform("u_lightSpot", m_spotlight->lightSpot);
	// 		m_worldProgram->SetUniform("u_lightAttenuation", m_spotlight->attenuation);

	// 		model->getMaterial()->SetUniform("u_lightPosRange", m_spotlight->posRange);
	// 		model->getMaterial()->SetUniform("u_lightColor", m_spotlight->color);
	// 		model->getMaterial()->SetUniform("u_lightSpot", m_spotlight->lightSpot);
	// 		model->getMaterial()->SetUniform("u_lightAttenuation", m_spotlight->attenuation);

	// 		for (Terrain *terrain : m_terrains)
	// 		{
	// 			terrain->getProgram()->SetUniform("u_spotLightPosRange", m_spotlight->posRange);
	// 			terrain->getProgram()->SetUniform("u_spotLightColor", m_spotlight->color);
	// 			terrain->getProgram()->SetUniform("u_spotLightSpot", m_spotlight->lightSpot);
	// 			terrain->getProgram()->SetUniform("u_spotLightAttenuation", m_spotlight->attenuation);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		m_worldProgram->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		m_worldProgram->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		m_worldProgram->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));

	// 		model->getMaterial()->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		model->getMaterial()->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		model->getMaterial()->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));

	// 		for (Terrain *terrain : m_terrains)
	// 		{
	// 			terrain->getProgram()->SetUniform("u_spotLightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 			terrain->getProgram()->SetUniform("u_spotLightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 			terrain->getProgram()->SetUniform("u_spotLightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));
	// 		}
	// 	}

	// 	model->render(miniProj, miniView, m_miniCamera->GetViewDirection());
	// }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(screenSize.x - m_miniWidth, screenSize.y - m_miniHeight, m_miniWidth, m_miniHeight);
	if (!m_bCinematic)
	{
		m_frameBuffer->Render();

		m_playerIcon->Render(glm::ortho(0.0f, (float)dimensions.x, (float)dimensions.y, 0.0f), glm::mat4(1.0f));
	}

	// MODEL POSITIONING (TESTING ONLY)

	// ImGui_ImplOpenGL3_NewFrame();
	// ImGui_ImplGlfw_NewFrame();
	// ImGui::NewFrame();
	// ImGui::Begin("Model Debug Menu");
	// ImGui::SetWindowSize(ImVec2(400.0f, 400.0f), true);
	// ImGui::SliderFloat("x", &x, -400.0f, 400.0f);
	// ImGui::SliderFloat("y", &y, -400.0f, 400.0f);
	// ImGui::SliderFloat("z", &z, -400.0f, 400.0f);
	// ImGui::SliderFloat("xRot", &xRot, -180.0f, 180.0f);
	// ImGui::SliderFloat("yRot", &yRot, -180.0f, 180.0f);
	// ImGui::SliderFloat("zRot", &zRot, -180.0f, 180.0f);

	// CURRENT MODEL FOR TESTING
	// Effect *curModel = m_blueTrailEffect;
	// ImGui::End();
	// ImGui::Render();
	// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

void StateGameplay::_generateTerrain(int centerChunkX, int centerChunkZ)
{
	// std::lock_guard<std::mutex> lock(m_mutex);
	for (int i = centerChunkX - 2; i <= centerChunkX + 2; i++)
	{
		for (int j = centerChunkZ - 2; j <= centerChunkZ + 2; j++)
		{
			if (m_terrainMap[std::make_pair(i, j)])
				continue;

			Terrain *terrain = new Terrain(i, j, m_terrainGenerator);
			m_terrainMap[std::make_pair(i, j)] = terrain;
			Scene::Instance()->AddNode(terrain);

			// Render based on Biome
			terrain->setBiome(m_curBiome);
			Terrain::Biome biome = terrain->getBiome();
			switch (biome)
			{
			case Terrain::Regular:
				if (terrain->GetBoundingBox().GetMin().y < -44.0f)
				{
					// Water
					Water *water = new Water(655, 17.5f, glm::vec3(0.0f, 1.0f, 0.0f), 13.3f, 0.1f, 2.0f, glm::vec3(0.019f, 0.537f, 0.854f));
					water->SetScale(glm::vec3(m_terrainSize, m_terrainSize, m_terrainSize));
					water->SetPos(glm::vec3(terrain->GetPos().x + m_terrainSize / 2.0f, -35.0f, terrain->GetPos().z + m_terrainSize / 2.0f));
					m_waterMap[std::make_pair(i, j)] = water;
					Scene::Instance()->AddNode(water);
					m_soundManager->Play3D("Water", m_waterSoundPath, water->GetPos(), 10.0f, true);
				}

				for (int i = 0; i < 1; i++)
				{
					Model *m_pCreature = new Model("data/models/spider.json", "skinned", true);
					m_pCreature->setTag("animated-enemy");
					m_pCreature->setTexture("data/spider/texture/main.png");
					m_pCreature->setNormal("data/spider/texture/normal.png");

					// m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin());
					// m_pCreature->attachRigidBody("data/physics/creature.rigid");

					// Light *pointLight = new Light();
					// float r = _randomFloat(0.0001f, 0.0018f);
					// float g = _randomFloat(0.0001f, 0.0028f);
					// float b = _randomFloat(0.0001f, 0.0038f);
					// pointLight->color = glm::vec3(r, g, b);
					// pointLight->attenuation = glm::vec3(0.9f, 0.9f, 0.9f);
					// pointLight->posRange = glm::vec4(0.0f, 0.0f, 0.0f, 3.0f);
					// m_lights.push_back(pointLight);
					// m_pCreature->attachLight(pointLight);
					// int scale = _randomNum(0.2f, 0.5f);
					float rotation = (float)_randomNum(-60, 60);

					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);
					m_pCreature->setScale(glm::vec3(0.03f, 0.03f, 0.03f));
					m_pCreature->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					m_pCreature->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(m_pCreature);
					Scene::Instance()->AddNode(m_pCreature);
				}

				for (int i = 0; i < 3; i++)
				{
					Model *m_pCreature = new Model("data/models/crawler.obj", "skinned");
					m_pCreature->setTag("enemy");
					m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
					m_pCreature->setNormal("data/textures/gimpy_normal.tga");
					m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin() + glm::vec3(0.0f, -2.0f, 0.0f));
					// m_pCreature->attachRigidBody("data/physics/creature.rigid");

					int scale = _randomNum(2, 5);
					float rotation = (float)_randomNum(-60, 60);

					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

					m_pCreature->setScale(glm::vec3(scale, scale, scale));
					m_pCreature->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					m_pCreature->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(m_pCreature);
					Scene::Instance()->AddNode(m_pCreature);
				}

				for (int i = 0; i < 10; i++)
				{
					Model *bush = new Model("data/models/shrub.fbx", "skinned");
					bush->setTag("food");
					bush->setTexture("data/textures/shrub.png");
					// bush->setOffset(bush->getModel()->getAABBMin());

					int scale = _randomNum(2, 5);
					float rotation = (float)_randomNum(-60, 60);
					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

					bush->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
					bush->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					bush->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(bush);
					Scene::Instance()->AddNode(bush);
				}

				// TREES
				// pot tree offset is aabcenter scale between 0.2 and 5
				// dead tree offset is none (for now) scale between 50 and 100 use wood texture

				for (int i = 0; i < 25; i++)
				{
					Model *tree = new Model("data/models/biomes/regular/dead-tree.obj", "skinned");
					// tree->setTag("food");

					tree->setTexture("data/textures/wood.png");
					// tree->setOffset(tree->getModel()->getAABBCenter());

					float rotation = (float)_randomNum(-60, 60);
					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);
					int scale = _randomNum(50, 100);
					tree->setScale(glm::vec3(scale, scale, scale));
					tree->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					tree->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(tree);
					Scene::Instance()->AddNode(tree);
				}

				for (int i = 0; i < 3; i++)
				{
					Model *log = new Model("data/models/log.fbx", "dim");
					log->setTexture("data/textures/log.png");
					// log->setOffset(log->getModel()->getAABBMin());

					float rotation = (float)_randomNum(-60, 60);
					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

					// log->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
					log->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					log->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(log);
					Scene::Instance()->AddNode(log);
				}

				break;
			case Terrain::Desert:
			{

				// TREES
				// pot tree offset is aabcenter scale between 0.2 and 5
				// dead tree offset is none (for now) scale between 50 and 100 use wood texture

				Model *m_part5 = new Model("data/models/ships/ship-parts/part2-tv.obj", "super-dim");
				m_part5->setTag("ship-part");
				// m_part5->setOffset(m_part4->getModel()->getAABBMin());
				m_part5->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
				m_part5->setTexture("data/textures/ship-texture.png");

				int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
				int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

				m_part5->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));

				m_shipParts.push_back(m_part5);

				m_models.push_back(m_part5);

				// Fire near ship parts
				int xOff = _randomNum(-5, 5);
				int zOff = _randomNum(-10, 10);

				Effect *fire1 = new Effect(m_firepath);
				glm::vec3 newPos = m_part5->getPosition() + glm::vec3(xOff, 0.0f, zOff);
				fire1->setPos(glm::vec3(newPos.x, m_terrainGenerator->GetHeight(newPos.x, newPos.z), newPos.z));
				m_effects.push_back(fire1);

				xOff = _randomNum(-5, 5);
				zOff = _randomNum(-10, 10);

				Effect *fire2 = new Effect(m_firepath);
				newPos = m_part5->getPosition() + glm::vec3(xOff, 0.0f, zOff);
				fire2->setPos(glm::vec3(newPos.x, m_terrainGenerator->GetHeight(newPos.x, newPos.z), newPos.z));
				m_effects.push_back(fire2);

				for (int i = 0; i < 4; i++)
				{
					Model *tree = new Model("data/models/biomes/desert/potTree.obj", "skinned");
					// tree->setTag("food");

					tree->setTexture("data/textures/wood.png");
					tree->setOffset(tree->getModel()->getAABBCenter());

					float rotation = (float)_randomNum(-60, 60);
					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);
					int scale = _randomNum(0.2f, 5.0f);
					// tree->setScale(glm::vec3(scale, scale, scale));
					tree->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					tree->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(tree);
					Scene::Instance()->AddNode(tree);
				}

				for (int i = 0; i < 3; i++)
				{
					Model *log = new Model("data/models/log.fbx", "dim");
					log->setTexture("data/textures/log.png");
					// log->setOffset(log->getModel()->getAABBMin());

					float rotation = (float)_randomNum(-60, 60);
					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

					// log->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
					log->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					log->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(log);
					Scene::Instance()->AddNode(log);
				}

				for (int i = 0; i < 5; i++)
				{
					Model *m_pCreature = new Model("data/models/low_poly_spitter.obj", "skinned");
					m_pCreature->setTag("enemy");
					m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
					m_pCreature->setNormal("data/textures/gimpy_normal.tga");
					m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin());
					// m_pCreature->attachRigidBody("data/physics/creature.rigid");

					// Light *pointLight = new Light();
					// float r = _randomFloat(0.0001f, 0.0018f);
					// float g = _randomFloat(0.0001f, 0.0028f);
					// float b = _randomFloat(0.0001f, 0.0038f);
					// pointLight->color = glm::vec3(r, g, b);
					// pointLight->attenuation = glm::vec3(0.9f, 0.9f, 0.9f);
					// pointLight->posRange = glm::vec4(0.0f, 0.0f, 0.0f, 3.0f);
					// m_lights.push_back(pointLight);
					// m_pCreature->attachLight(pointLight);
					int scale = _randomNum(3.0f, 5.0f);
					float rotation = (float)_randomNum(-60, 60);

					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);
					m_pCreature->setScale(glm::vec3(scale, scale, scale));
					m_pCreature->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					m_pCreature->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(m_pCreature);
					Scene::Instance()->AddNode(m_pCreature);
				}

				for (int i = 0; i < 3; i++)
				{
					Model *m_pCreature = new Model("data/models/crawler.obj", "skinned");
					m_pCreature->setTag("enemy");
					m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
					m_pCreature->setNormal("data/textures/gimpy_normal.tga");
					m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin() + glm::vec3(0.0f, -2.0f, 0.0f));
					// m_pCreature->attachRigidBody("data/physics/creature.rigid");

					int scale = _randomNum(2, 5);
					float rotation = (float)_randomNum(-60, 60);

					int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
					int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

					m_pCreature->setScale(glm::vec3(scale, scale, scale));
					m_pCreature->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));
					m_pCreature->setRotation(glm::vec3(0.0f, rotation, 0.0f));
					m_models.push_back(m_pCreature);
					Scene::Instance()->AddNode(m_pCreature);
				}
			}
			break;
			case Terrain::Lava:
			{

				Model *m_part4 = new Model("data/models/ships/ship-parts/part1-bearing.obj", "super-dim");
				m_part4->setTag("ship-part");
				// m_part4->setOffset(m_part4->getModel()->getAABBMin());
				m_part4->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
				m_part4->setTexture("data/textures/ship-texture.png");

				m_shipParts.push_back(m_part4);

				int x = _randomNum(terrain->GetPos().x - m_terrainSize / 2, terrain->GetPos().x + m_terrainSize / 2);
				int z = _randomNum(terrain->GetPos().z - m_terrainSize / 2, terrain->GetPos().z + m_terrainSize / 2);

				m_part4->setPosition(glm::vec3(x, m_terrainGenerator->GetHeight(x, z), z));

				m_models.push_back(m_part4);

				// Fire near ship parts
				int xOff = _randomNum(-5, 5);
				int zOff = _randomNum(-10, 10);

				Effect *fire1 = new Effect(m_firepath);
				glm::vec3 newPos = m_part4->getPosition() + glm::vec3(xOff, 0.0f, zOff);
				fire1->setPos(glm::vec3(newPos.x, m_terrainGenerator->GetHeight(newPos.x, newPos.z), newPos.z));
				m_effects.push_back(fire1);

				xOff = _randomNum(-5, 5);
				zOff = _randomNum(-10, 10);

				Effect *fire2 = new Effect(m_firepath);
				newPos = m_part4->getPosition() + glm::vec3(xOff, 0.0f, zOff);
				fire2->setPos(glm::vec3(newPos.x, m_terrainGenerator->GetHeight(newPos.x, newPos.z), newPos.z));
				m_effects.push_back(fire2);

				Water *lava = new Water(2000, 19.8f, glm::vec3(0.0f, 1.0f, 0.0f), 10.9f, 0.1f, 1.7f, glm::vec3(0.411f, 0.022f, 0.025f));
				lava->SetScale(glm::vec3(m_terrainSize, m_terrainSize, m_terrainSize));
				lava->SetPos(glm::vec3(terrain->GetPos().x + m_terrainSize / 2.0f, -15.0f, terrain->GetPos().z + m_terrainSize / 2.0f));
				m_waterMap[std::make_pair(i, j)] = lava;
				Scene::Instance()->AddNode(lava);
			}
			break;
			case Terrain::Water:
				Water *water = new Water(1000, 31.3f, glm::vec3(0.0f, 1.0f, 0.0f), 95.3f, 0.1f, 1.7f, glm::vec3(0.019f, 0.537f, 0.854f));
				water->SetScale(glm::vec3(m_terrainSize, m_terrainSize, m_terrainSize));
				water->SetPos(glm::vec3(terrain->GetPos().x + m_terrainSize / 2.0f, -15.0f, terrain->GetPos().z + m_terrainSize / 2.0f));
				m_waterMap[std::make_pair(i, j)] = water;
				Scene::Instance()->AddNode(water);
				// m_soundManager->Play3D("Water", m_waterSoundPath, water->GetPos(), 10.0f, true);
				break;
			}
		}
	}
	m_loadedStart = wolf::min(m_loadedStart, centerChunkX);
	m_loadedEnd = wolf::max(m_loadedEnd, centerChunkZ);
	m_curBiome = static_cast<Terrain::Biome>(rand() % 4);
}

bool StateGameplay::_inRange(int chunkX, int chunkZ)
{
	return m_terrainMap[std::make_pair(chunkX, chunkZ)];
}