#include "./StateRespawn.h"
#include "states.h"

StateRespawn::StateRespawn()
{
}

StateRespawn::~StateRespawn()
{
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	delete m_pSkybox;
	delete m_font;
	delete m_pDead;
	delete m_pRespawn;
	delete m_pQuitGame;
}

void StateRespawn::Exit()
{
}

void StateRespawn::Enter(std::string arg)
{
	m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_app->setCursorPos(500.0f, 300.0f);

	if (!m_pWorldProgram)
	{

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glm::vec2 screenSize = m_app->getScreenSize();
		m_frameBuffer = wolf::BufferManager::CreateFrameBuffer(screenSize.x, screenSize.y);

		m_font = new Font("data/fonts/alpha.fnt", "data/textures/fonts/");

		m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_pSkybox = new Skybox(m_skyboxPath);

		// Menu

		m_pDead = new TextBox(500.0f, 100.0f);
		m_pDead->SetText(m_font, "You are Dead");
		m_pDead->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_pDead->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pDead->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pRespawn = new TextBox(500.0f, 100.0f);
		m_pRespawn->SetText(m_font, "Respawn");
		m_pRespawn->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pRespawn->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pRespawn->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pQuitGame = new TextBox(500.0f, 100.0f);
		m_pQuitGame->SetText(m_font, "Quit Game");
		m_pQuitGame->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pQuitGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pQuitGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);
	}
}

void StateRespawn::Update(float p_fDelta)
{
	glm::vec2 screenSize = m_app->getScreenSize();

	// m_pSkybox->update(p_fDelta);
	m_pDead->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 - 300.0f, 0.0f));
	m_pRespawn->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 - 100.0f, 0.0f));
	m_pQuitGame->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 + 100.0f, 0.0f));

	int iButton = m_app->isLMBDown();
	if (iButton)
	{
		float x, y = 0;
		x = m_app->getMousePos().x;
		y = m_app->getMousePos().y;
		// if (x > m_pRespawn->GetPos().x && x < m_pRespawn->GetPos().x + m_pRespawn->GetWidth() && y > m_pRespawn->GetPos().y && y < m_pRespawn->GetPos().y + m_pRespawn->GetHeight())
		// {
		// 	// Respawn Game
		// 	m_pGameplayState->Reset();
		// 	m_pStateMachine->GoToState(eStateGameplay_Gameplay);
		// }
		if (x > m_pQuitGame->GetPos().x && x < m_pQuitGame->GetPos().x + m_pQuitGame->GetWidth() && y > m_pQuitGame->GetPos().y && y < m_pQuitGame->GetPos().y + m_pQuitGame->GetHeight())
		{
			// Exit Game
			m_app->exit();
		}
	}
}

void StateRespawn::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
	glm::vec2 screenSize = m_app->getScreenSize();
	int width = screenSize.x;
	int height = screenSize.y;

	m_frameBuffer->Bind();

	glViewport(0, 0, width, height);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pWorldProgram->SetUniform("u_lightDir", Scene::Instance()->GetLightDirection());

	m_pSkybox->Render(mProj, mView);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_frameBuffer->Render();

	m_pDead->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	// m_pRespawn->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	m_pQuitGame->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
}

int StateRespawn::_randomNum(int lowerBound, int upperBound)
{
	// Generates random number in range. Isncludes lower bound and upper bound
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}