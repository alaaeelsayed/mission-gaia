#include "stategameplay.h"
#include "../obj/model.h"
#include "states.h"
#include "../scene/scene.h"
#include "../obj/threads/threadpool.h"
#include "../../wolf/W_PhysicsManager.h"

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

	wolf::BufferManager::DestroyBuffer(m_frameBuffer);
	wolf::ProgramManager::DestroyProgram(m_worldProgram);
	delete m_skybox;
	delete m_font;
	delete m_hungerText;
	delete m_thirstText;
	delete m_stateMachine;
	delete m_playerIcon;

	delete m_soundManager;

	for (const auto& element : m_terrainMap)
	{
		delete element.second;
	}

	for (Light* light : m_lights)
	{
		delete light;
	}
}

void StateGameplay::Exit()
{
}

void StateGameplay::Enter(std::string arg)
{
	m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!m_worldProgram)
	{

		Camera* camera = new GroundedCamera(m_app);
		camera->SetPosition(glm::vec3(500, 100, 500));
		Scene::Instance()->SetActiveCamera(camera);

		srand(time(NULL));

		glm::vec2 screenSize = m_app->getScreenSize();
		m_frameBuffer = wolf::BufferManager::CreateFrameBuffer(m_miniWidth, m_miniHeight);

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

		m_playerIcon = new Triangle(m_playerIconPath);
		m_playerIcon->setScale(glm::vec2(0.07f, 0.07f));

		m_spotlight = new Light();
		m_spotlight->pos = glm::vec4(0.0f, 0.0f, 0.0f, 100.0f);
		m_spotlight->color = glm::vec3(0.95f, 0.95f, 0.9f);
		m_spotlight->attenuation = glm::vec3(1.0f, 0.07f, 0.017f);
		m_spotlight->dir = glm::vec4(2.0f, 2.0f, 2.0f, 0.8f);
		m_spotlight->coneAngle = cos(glm::radians(10.0f));
		m_spotlight->cutOff = cos(glm::radians(15.0f));
		m_spotlight->enabled = false;

		m_flashlight->setTexture("data/textures/flashlight.png");

		m_terrainGenerator = new TerrainGenerator();

		m_terrainSize = m_terrainGenerator->GetSize();
		m_terrainVerts = m_terrainGenerator->GetVertexCount();
		m_terrainOctaves = m_terrainGenerator->GetOctaves();
		m_terrainRoughness = m_terrainGenerator->GetRoughness();
		m_terrainAmplitude = m_terrainGenerator->GetAmplitude();

		_generateTerrain(0, 0);

		m_soundManager = new wolf::SoundManager();
		m_soundManager->CreateSoundSystem();
		m_soundManager->SetVolume(0.0f);

		// m_soundManager->Play3D("fire", m_fireSound, m_ship->getPosition(), 10.0f, true);
		m_soundManager->Play2D("Nature", m_natureSoundPath, true);

		// Debug Menu
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
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
	Camera* camera = Scene::Instance()->GetActiveCamera();
	m_soundManager->SetListenerPosition(camera->GetPosition(), camera->GetViewDirection());

	glm::vec2 dimensions = m_app->getScreenSize();

	float width = dimensions.x;
	float height = dimensions.y;

	wolf::PhysicsManager::Instance()->Update(p_fDelta);

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
	}

	_updateCameraAndScreen(camera);

	_updateTextboxes(camera, width, height);
	_updateSpotlight(camera);

	_handleFlashlightToggle();
	_handleCinematicToggle();
	_handleGravityGunToggle(camera);

	if ((m_hunger <= 0.0f && m_thirst <= 0.0f) || m_health <= 0)
	{
		Scene::Instance()->GetStateMachine()->GoToState(eStateGameplay_Respawn);
	}

	m_terrainGenerator->SetAmplitude(m_terrainAmplitude);
	m_terrainGenerator->SetSize(m_terrainSize);
	m_terrainGenerator->SetVertexCount(m_terrainVerts);
	m_terrainGenerator->SetOctaves(m_terrainOctaves);
	m_terrainGenerator->SetRoughness(m_terrainRoughness);
	m_terrainGenerator->SetBaseMult(m_baseMult);
	m_terrainGenerator->SetValleyMult(m_valleyMult);
	m_terrainGenerator->SetLowAreaThreshold(m_lowAreaThreshold);
	m_terrainGenerator->SetLowAreaSmoothFactor(m_lowAreaSmoothFactor);
	m_terrainGenerator->SetValleyThreshold(m_valleyThreshold);
	m_terrainGenerator->SetValleyDepth(m_valleyDepth);
	m_terrainGenerator->SetSmoothEdge0(m_smoothEdge0);
	m_terrainGenerator->SetSmoothEdge1(m_smoothEdge1);

	if (m_terrainMap.size() > 0)
	{
		std::vector<Terrain*> terrains;
		transform(m_terrainMap.begin(), m_terrainMap.end(), back_inserter(terrains),
			[](const std::map<std::pair<int, int>, Terrain*>::value_type& val)
			{ return val.second; });
		for (Terrain* terrain : terrains)
		{
			for (int i = 0; i < 4; i++)
			{
				terrain->setDispMult(i, m_dispMult[i]);
			}
		}
	}

	if (!m_cube)
	{
		m_cube = new Cube("data/textures/diffuse.png");
		m_cube->SetPos(glm::vec3(500, 100, 500));
		Scene::Instance()->AddNode(m_cube);
		wolf::PhysicsManager::Instance()->GetScene()->addActor(*m_cube->GetRigidBody());
	}

	Scene::Instance()->BuildQuadtree();
}

void StateGameplay::_updateCameraAndScreen(Camera* camera) {
	m_miniCamera->SetRotation(glm::vec2(camera->GetRotation().x, -90.01f));
	m_miniCamera->SetPosition(glm::vec3(camera->GetPosition().x, 500.0f, camera->GetPosition().z));
}

void StateGameplay::_updateTextboxes(Camera* camera, float width, float height) {
	m_thirstText->SetPos(glm::vec3(width / 14, height - height / 8, 0.0f));
	m_hungerText->SetPos(glm::vec3(width / 14, height - height / 4, 0.0f));

	int hunger = (int)m_hunger;
	int thirst = (int)m_thirst;

	m_hungerText->SetText(m_font, "%d%", hunger);
	m_thirstText->SetText(m_font, "%d%", thirst);

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
}

void StateGameplay::_updateSpotlight(Camera* camera) {
	float camX = camera->GetPosition().x;
	float camY = camera->GetPosition().y;
	float camZ = camera->GetPosition().z;

	m_spotlight->pos = glm::vec3(camX, camY, camZ);
	m_spotlight->dir = camera->GetViewDirection();
}

void StateGameplay::_handleFlashlightToggle() {
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
			m_spotlight->attenuation = glm::vec3(1.0f, 0.07f, 0.017f);
		}
	}

	if (!m_app->isKeyDown('F'))
		m_keyDown = false;
}

void StateGameplay::_handleCinematicToggle() {
	if (m_app->isKeyDown('C') && !m_bCinematicButtonDown)
	{
		m_bCinematicButtonDown = true;
		m_bCinematic = !m_bCinematic;
	}

	if (!m_app->isKeyDown('C'))
		m_bCinematicButtonDown = false;
}

void StateGameplay::_handleGravityGunToggle(Camera* camera) {
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

	// Bullets
	if (m_app->isLMBDown() && !m_leftMouseDown && m_gravityGunEquipped)
	{
		// Shoot gravity bullet
		Sphere* bullet = new Sphere(1.0f);
		bullet->SetPos(camera->GetPosition() + camera->GetViewDirection() * 0.5f);
		bullet->SetColor(glm::vec4(0.0f, 0.5f, 1.0f, 0.2f));
		bullet->GetRigidBody()->setLinearVelocity(PxVec3(camera->GetViewDirection().x, camera->GetViewDirection().y, camera->GetViewDirection().z) * 50.0f);
		PxVec3 actualVelocity = bullet->GetRigidBody()->getLinearVelocity();

		Scene::Instance()->AddNode(bullet);
		wolf::PhysicsManager::Instance()->GetScene()->addActor(*bullet->GetRigidBody());

		m_bullets.push_back(bullet);
		m_soundManager->Play2D("gravityGunFaulty", m_gravityGunFaultyPath, false, true);
		m_leftMouseDown = true;
	}

	if (!m_app->isLMBDown())
	{
		m_leftMouseDown = false;
	}
}

void StateGameplay::Render(const glm::mat4& mProj, const glm::mat4& mView)
{
	if (m_bCinematic)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glm::vec2 dimensions = m_app->getScreenSize();
	int width = dimensions.x;
	int height = dimensions.y;

	Camera* camera = Scene::Instance()->GetActiveCamera();

	glm::vec3 spotLightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 spotLightDir = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 spotLightAttenuation = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 spotLightColor = glm::vec3(0.0f, 0.0f, 0.0f);
	float spotLightConeAngle = 0.0f;
	float spotLightCutOff = 0.0f;

	if (m_spotlight->enabled)
	{
		spotLightPos = m_spotlight->pos;
		spotLightDir = m_spotlight->dir;
		spotLightAttenuation = m_spotlight->attenuation;
		spotLightColor = m_spotlight->color;
		spotLightConeAngle = m_spotlight->coneAngle;
		spotLightCutOff = m_spotlight->cutOff;
	}

	m_worldProgram->SetUniform("u_lightDir", Scene::Instance()->GetLightDirection());
	m_worldProgram->SetUniform("u_viewPos", camera->GetViewDirection());

	m_worldProgram->SetUniform("u_lightPosRange", spotLightPos);
	m_worldProgram->SetUniform("u_lightColor", spotLightColor);
	m_worldProgram->SetUniform("u_lightSpot", spotLightDir);
	m_worldProgram->SetUniform("u_lightAttenuation", spotLightAttenuation);

	Terrain* terrain = m_terrainMap[std::make_pair(camera->GetPosition().x / m_terrainSize, camera->GetPosition().z / m_terrainSize)];

	terrain->getProgram()->SetUniform("u_spotLightFactor", (float)m_spotlight->enabled);
	terrain->getProgram()->SetUniform("u_spotLightPos", spotLightPos);
	terrain->getProgram()->SetUniform("u_spotLightColor", spotLightColor);
	terrain->getProgram()->SetUniform("u_spotLightDir", spotLightDir);
	terrain->getProgram()->SetUniform("u_spotLightAttenuation", spotLightAttenuation);
	terrain->getProgram()->SetUniform("u_spotLightConeAngle", spotLightConeAngle);
	terrain->getProgram()->SetUniform("u_spotLightCutOff", spotLightCutOff);

	terrain->getProgram()->SetUniform("u_characterPosition", camera->GetPosition());

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
			m_spotlight->attenuation = glm::vec3(1.0f, 0.07f, 0.017f);
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
		ISound* walking = m_soundManager->GetSound("walking");
		ISound* running = m_soundManager->GetSound("running");
		if (walking && !walking->getIsPaused())
			m_soundManager->PauseSound("walking");
		if (running && !running->getIsPaused())
			m_soundManager->PauseSound("running");
	}

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
		glEnable(GL_DEPTH_TEST);
	}

	if (!m_bCinematic)
	{
		m_hungerText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		m_thirstText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	}
	// MINIMAP
	if (!m_bCinematic)
	{
		_renderMinimap();
	}

	if (m_debug)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug Menu");

		ImGui::SetWindowSize(ImVec2(400.0f, 600.0f), true);

		if (ImGui::BeginTabBar("Config Values"))
		{
			if (ImGui::BeginTabItem("Terrain"))
			{
				ImGui::SliderFloat4("Disp Mults", m_dispMult, 0.0f, 6.0f);
				ImGui::SliderInt("Terrain Size", &m_terrainSize, 0, 1000);
				ImGui::SliderInt("Terrain Vert Count", &m_terrainVerts, 64, 1024);
				ImGui::SliderInt("Terrain Octaves", &m_terrainOctaves, 1, 10);
				ImGui::SliderFloat("Terrain Roughness", &m_terrainRoughness, 0.0f, 10.0f);
				ImGui::SliderFloat("Terrain Amplitude", &m_terrainAmplitude, 0.0f, 1000.0f);

				if (ImGui::Button("Regenerate Terrain")) {
					for (auto& pair : m_terrainMap) {
						Scene::Instance()->RemoveNode(pair.second);
						delete pair.second;
					}
					m_terrainMap.clear();

					m_terrainGenerator->ClearHeightCache();

					_generateTerrain(camera->GetPosition().x / m_terrainGenerator->GetSize(), camera->GetPosition().z / m_terrainGenerator->GetSize());

					camera->SetPosition(glm::vec3(camera->GetPosition().x, camera->GetPosition().y + 500, camera->GetPosition().z));

				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Terrain Details"))
			{
				ImGui::SliderFloat("Base Multiplier", &m_baseMult, 0.0f, 10.0f);
				ImGui::SliderFloat("Valley Multiplier", &m_valleyMult, 0.0f, 10.0f);
				ImGui::SliderFloat("Low Area Threshold", &m_lowAreaThreshold, -1.0f, 20.0f);
				ImGui::SliderFloat("Low Area Smooth Factor", &m_lowAreaSmoothFactor, 0.0f, 1.0f);
				ImGui::SliderFloat("Valley Threshold", &m_valleyThreshold, -1.0f, 1.0f);
				ImGui::SliderFloat("Valley Depth", &m_valleyDepth, 0.0f, 10.0f);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Edge Smoothing"))
			{
				ImGui::SliderFloat("Smooth Edge Start", &m_smoothEdge0, 0.0f, 1.0f);
				ImGui::SliderFloat("Smooth Edge End", &m_smoothEdge1, 0.0f, 1.0f);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}

void StateGameplay::_renderMinimap()
{
	glm::vec2 screenSize = m_app->getScreenSize();

	glViewport(0, 0, m_miniWidth, m_miniHeight);

	m_frameBuffer->Bind();

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4& miniProj = m_miniCamera->GetProjMatrix(m_miniWidth, m_miniHeight);
	const glm::mat4& miniView = m_miniCamera->GetViewMatrix();

	m_skybox->Render(miniProj, miniView);

	std::vector<Terrain*> terrains;
	transform(m_terrainMap.begin(), m_terrainMap.end(), back_inserter(terrains),
		[](const std::map<std::pair<int, int>, Terrain*>::value_type& val)
		{ return val.second; });
	for (Terrain* terrain : terrains)
	{
		terrain->Render(miniProj, miniView);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(screenSize.x - m_miniWidth, screenSize.y - m_miniHeight, m_miniWidth, m_miniHeight);

	m_frameBuffer->Render();

	m_playerIcon->Render(glm::ortho(0.0f, (float)m_miniWidth, (float)m_miniHeight, 0.0f), glm::mat4(1.0f));
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

bool StateGameplay::_isEffectiveLight(const Light* pLight1, const Light* pLight2, Model* pModel) const
{

	if (pLight1->enabled && !pLight2->enabled)
		return true;

	if (!pLight1->enabled || (!pLight2->enabled && !pLight1->enabled))
		return false;

	float fLight1Dist = glm::distance(glm::vec3(pLight1->pos.x, pLight1->pos.y, pLight1->pos.z), pModel->getPosition());
	float fLight2Dist = glm::distance(glm::vec3(pLight2->pos.x, pLight2->pos.y, pLight2->pos.z), pModel->getPosition());

	return glm::any(glm::lessThan(pLight1->attenuation, pLight2->attenuation)) && (fLight1Dist < fLight2Dist);
}

void StateGameplay::_generateTerrain(int centerChunkX, int centerChunkZ)
{
	for (int i = centerChunkX - 2; i <= centerChunkX + 2; i++)
	{
		for (int j = centerChunkZ - 2; j <= centerChunkZ + 2; j++)
		{
			if (m_terrainMap[std::make_pair(i, j)])
				continue;

			Terrain* terrain = new Terrain(i, j, m_terrainGenerator);
			m_terrainMap[std::make_pair(i, j)] = terrain;
			Scene::Instance()->AddNode(terrain);

			Terrain::Biome biome = terrain->getBiome();
		}
	}
}

bool StateGameplay::_inRange(int chunkX, int chunkZ)
{
	return m_terrainMap[std::make_pair(chunkX, chunkZ)];
}