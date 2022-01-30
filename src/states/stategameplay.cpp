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
}

void StateGameplay::Exit()
{
}

void StateGameplay::Enter(std::string arg)
{
	m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!m_worldProgram)
	{
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

		m_spotlight = new Light();
		m_spotlight->posRange = glm::vec4(0.0f, 0.0f, 0.0f, 100.0f);
		m_spotlight->color = glm::vec3(0.005f, 0.005f, 0.005f);
		m_spotlight->attenuation = glm::vec3(0.0f, 0.5f, 0.0f);
		m_spotlight->lightSpot = glm::vec4(2.0f, 2.0f, 2.0f, 0.8f);
		m_spotlight->enabled = false;

		m_flashlight->attachLight(m_spotlight);
		m_flashlight->setTexture("data/textures/flashlight.png");
		m_models.push_back(m_flashlight);

		for (int i = 0; i < 10; i++)
		{
			Model *m_pCreature = new Model("data/models/low_poly_spitter.obj", "skinned");
			m_pCreature->setTexture("data/textures/gimpy_diffuse.tga");
			m_pCreature->setNormal("data/textures/gimpy_normal.tga");
			m_pCreature->setOffset(m_pCreature->getModel()->getAABBMin());

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
			int x = _randomNum(-100, 100);
			int z = _randomNum(-100, 100);
			m_pCreature->setScale(glm::vec3(scale, scale, scale));
			m_pCreature->translate(glm::vec3(x, 0.0f, z));
			// m_pCreature->setRotation(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			m_models.push_back(m_pCreature);
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
		// m_water->setPosition(glm::vec3(1000.0f, -20.0f, 1000.0f));
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
}

void StateGameplay::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
	glm::vec2 dimensions = m_app->getScreenSize();
	Camera *camera = Scene::Instance()->GetActiveCamera();

	m_worldProgram->SetUniform("u_lightDir", m_sunDirection);
	m_worldProgram->SetUniform("u_viewPos", camera->GetViewDirection());

	m_hungerText->Render(glm::ortho(0.0f, (float)dimensions.x, (float)dimensions.y, 0.0f), glm::mat4(1.0f));
	m_thirstText->Render(glm::ortho(0.0f, (float)dimensions.x, (float)dimensions.y, 0.0f), glm::mat4(1.0f));

	// for (Model *model : m_models)
	// {

	// 	std::sort(m_vLights.begin(), m_vLights.end(), [this, model](const Light *lhs, const Light *rhs) -> bool
	// 			  { return _isEffectiveLight(lhs, rhs, model); });

	// 	for (int i = 0; i < 4; i++)
	// 	{
	// 		// Point lights
	// 		Light *pLight = m_vLights[i];
	// 		if (pLight->bEnabled)
	// 		{
	// 			m_worldProgram->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->vPosRange);
	// 			m_worldProgram->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->vColor);
	// 			m_worldProgram->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->vAttenuation);
	// 			model->getMaterial()->SetUniform("u_lightPosRange" + std::to_string(i + 1), pLight->vPosRange);
	// 			model->getMaterial()->SetUniform("u_lightColor" + std::to_string(i + 1), pLight->vColor);
	// 			model->getMaterial()->SetUniform("u_lightAttenuation" + std::to_string(i + 1), pLight->vAttenuation);
	// 		}
	// 	}

	// 	// Spot Light
	// 	if (m_spotlight->bEnabled)
	// 	{
	// 		m_worldProgram->SetUniform("u_lightPosRange", m_spotlight->vPosRange);
	// 		m_worldProgram->SetUniform("u_lightColor", m_spotlight->vColor);
	// 		m_worldProgram->SetUniform("u_lightSpot", m_spotlight->vLightSpot);
	// 		m_worldProgram->SetUniform("u_lightAttenuation", m_spotlight->vAttenuation);

	// 		model->getMaterial()->SetUniform("u_lightPosRange", m_spotlight->vPosRange);
	// 		model->getMaterial()->SetUniform("u_lightColor", m_spotlight->vColor);
	// 		model->getMaterial()->SetUniform("u_lightSpot", m_spotlight->vLightSpot);
	// 		model->getMaterial()->SetUniform("u_lightAttenuation", m_spotlight->vAttenuation);
	// 	}
	// 	else
	// 	{
	// 		m_worldProgram->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		m_worldProgram->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		m_worldProgram->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));

	// 		model->getMaterial()->SetUniform("u_lightPosRange", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		model->getMaterial()->SetUniform("u_lightSpot", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 		model->getMaterial()->SetUniform("u_lightAttenuation", glm::vec3(1.0f, 1.0f, 1.0f));
	// 	}
	// 	model->render(mProj, mView, m_pCam->getViewDirection());
	// }

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

	// if (m_bFlashlightEquipped)
	// 	m_flashlight->render(mProj, glm::mat4(1.0f));

	// m_hungerText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	// m_thirstText->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
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