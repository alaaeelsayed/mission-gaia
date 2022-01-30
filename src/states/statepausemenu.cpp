#include "statepausemenu.h"
#include "states.h"
#include "../scene/scene.h"

StatePauseMenu::StatePauseMenu()
{
}

StatePauseMenu::~StatePauseMenu()
{
	wolf::ProgramManager::DestroyProgram(m_worldProgram);
	delete m_skybox;
	delete m_font;
	delete m_pauseMenu;
	delete m_resumeGame;
	delete m_quitGame;
}

void StatePauseMenu::Exit()
{
}

void StatePauseMenu::Enter(std::string arg)
{
	m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_app->setCursorPos(500.0f, 300.0f);

	if (!m_worldProgram)
	{

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glm::vec2 screenSize = m_app->getScreenSize();
		m_frameBuffer = wolf::BufferManager::CreateFrameBuffer(screenSize.x, screenSize.y);

		m_font = new Font("data/fonts/alpha.fnt", "data/textures/fonts/");

		m_worldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_skybox = new Skybox(m_skyboxPath);

		// Menu

		m_pauseMenu = new TextBox(500.0f, 100.0f);
		m_pauseMenu->SetText(m_font, "Pause Menu");
		m_pauseMenu->SetColor(0.0f, 0.2f, 0.2f, 1.0f);
		m_pauseMenu->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pauseMenu->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_resumeGame = new TextBox(500.0f, 100.0f);
		m_resumeGame->SetText(m_font, "Resume Game");
		m_resumeGame->SetColor(0.0f, 0.2f, 0.2f, 1.0f);
		m_resumeGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_resumeGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_quitGame = new TextBox(500.0f, 100.0f);
		m_quitGame->SetText(m_font, "Quit Game");
		m_quitGame->SetColor(0.0f, 0.2f, 0.2f, 1.0f);
		m_quitGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_quitGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);
	}
}

void StatePauseMenu::Update(float dt)
{
	m_skybox->Update(dt);

	glm::vec2 screenSize = m_app->getScreenSize();

	// m_skybox->update(p_fDelta);
	m_pauseMenu->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 - 300.0f, 0.0f));
	m_resumeGame->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 - 100.0f, 0.0f));
	m_quitGame->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 + 100.0f, 0.0f));

	int iButton = m_app->isLMBDown();
	if (iButton)
	{
		float x, y = 0;
		x = m_app->getMousePos().x;
		y = m_app->getMousePos().y;
		if (x > m_resumeGame->GetPos().x && x < m_resumeGame->GetPos().x + m_resumeGame->GetWidth() && y > m_resumeGame->GetPos().y && y < m_resumeGame->GetPos().y + m_resumeGame->GetHeight())
		{
			// Resume Game
			m_stateMachine->GoToState(eStateGameplay_Gameplay);
		}
		else if (x > m_quitGame->GetPos().x && x < m_quitGame->GetPos().x + m_quitGame->GetWidth() && y > m_quitGame->GetPos().y && y < m_quitGame->GetPos().y + m_quitGame->GetHeight())
		{
			// Exit Game
			m_app->exit();
		}
	}
}

void StatePauseMenu::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
	glm::vec2 screenSize = m_app->getScreenSize();

	m_frameBuffer->Bind();

	glViewport(0, 0, screenSize.x, screenSize.y);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_worldProgram->SetUniform("u_lightDir", m_sunDirection);

	m_skybox->Render(mProj, mView);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_frameBuffer->Render();

	m_pauseMenu->Render(glm::ortho(0.0f, (float)screenSize.x, (float)screenSize.y, 0.0f), glm::mat4(1.0f));
	m_resumeGame->Render(glm::ortho(0.0f, (float)screenSize.x, (float)screenSize.y, 0.0f), glm::mat4(1.0f));
	m_quitGame->Render(glm::ortho(0.0f, (float)screenSize.x, (float)screenSize.y, 0.0f), glm::mat4(1.0f));
}

int StatePauseMenu::_randomNum(int lowerBound, int upperBound)
{
	// Generates random number in range. Includes lower bound and upper bound
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}