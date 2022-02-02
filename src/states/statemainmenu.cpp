#include "./statemainmenu.h"
#include "states.h"

StateMainMenu::StateMainMenu()
{
}

StateMainMenu::~StateMainMenu()
{
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	delete m_pSkybox;
	delete m_font;
	delete m_pProjectGaia;
	delete m_pStartGame;
	delete m_pQuitGame;
	delete m_pLoading;
}

void StateMainMenu::Exit()
{
}

void StateMainMenu::Enter(std::string arg)
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

		m_pProjectGaia = new TextBox(500.0f, 100.0f);
		m_pProjectGaia->SetText(m_font, "Project Gaia");
		m_pProjectGaia->SetColor(0.7f, 0.39f, 0.11f, 1.0f);
		m_pProjectGaia->SetOutlined(true);
		m_pProjectGaia->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pProjectGaia->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pStartGame = new TextBox(500.0f, 100.0f);
		m_pStartGame->SetText(m_font, "Start Game");
		m_pStartGame->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pStartGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pStartGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pQuitGame = new TextBox(500.0f, 100.0f);
		m_pQuitGame->SetText(m_font, "Quit Game");
		m_pQuitGame->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pQuitGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pQuitGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pLoading = new TextBox(500.0f, 100.0f);
		m_pLoading->SetText(m_font, "Loading...");
		m_pLoading->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pLoading->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pLoading->SetVerticalAlignment(TextBox::Alignment::AL_Center);
	}
}

void StateMainMenu::Update(float p_fDelta)
{
	if (m_bIsLoading)
	{
		m_stateMachine->GoToState(eStateGameplay_Gameplay);
	}

	glm::vec2 screenSize = m_app->getScreenSize();

	// m_pSkybox->update(p_fDelta);
	m_pProjectGaia->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 - 300.0f, 0.0f));
	m_pStartGame->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 - 100.0f, 0.0f));
	m_pQuitGame->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2 + 100.0f, 0.0f));
	m_pLoading->SetPos(glm::vec3(screenSize.x / 2 - 250.0f, screenSize.y / 2, 0.0f));

	int iButton = m_app->isLMBDown();
	if (iButton && !m_bIsLoading)
	{
		float x, y = 0;
		x = m_app->getMousePos().x;
		y = m_app->getMousePos().y;
		if (x > m_pStartGame->GetPos().x && x < m_pStartGame->GetPos().x + m_pStartGame->GetWidth() && y > m_pStartGame->GetPos().y && y < m_pStartGame->GetPos().y + m_pStartGame->GetHeight())
		{
			// Resume Game

			m_bIsLoading = true;
		}
		else if (x > m_pQuitGame->GetPos().x && x < m_pQuitGame->GetPos().x + m_pQuitGame->GetWidth() && y > m_pQuitGame->GetPos().y && y < m_pQuitGame->GetPos().y + m_pQuitGame->GetHeight())
		{
			// Exit Game
			m_app->exit();
		}
	}
}

void StateMainMenu::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
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

	if (!m_bIsLoading)
	{
		m_pProjectGaia->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		m_pStartGame->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
		m_pQuitGame->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
	}
	else
		m_pLoading->Render(glm::ortho(0.0f, (float)width, (float)height, 0.0f), glm::mat4(1.0f));
}

int StateMainMenu::_randomNum(int lowerBound, int upperBound)
{
	// Generates random number in range. Includes lower bound and upper bound
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}