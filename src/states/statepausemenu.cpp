#include "./statepausemenu.h"
#include "states.h"

StatePauseMenu::StatePauseMenu()
{
}

StatePauseMenu::~StatePauseMenu()
{
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	delete m_pSkybox;
	delete m_font;
	delete m_pPauseMenu;
	delete m_pResumeGame;
	delete m_pQuitGame;
}

void StatePauseMenu::Exit()
{
}

void StatePauseMenu::Enter(std::string arg)
{
	m_pApp->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_pApp->setCursorPos(500.0f, 300.0f);

	if (!m_pWorldProgram)
	{

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);

		m_font = new Font("data/fonts/alpha.fnt", "data/textures/fonts/");

		m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		if (m_pCam)
		{
			m_pSkybox = new Skybox(m_pWorldProgram, m_pCam, m_skyboxPath);
		}

		// Menu

		m_pPauseMenu = new TextBox(500.0f, 100.0f);
		m_pPauseMenu->SetText(m_font, "Pause Menu");
		m_pPauseMenu->SetColor(0.0f, 0.2f, 0.2f, 1.0f);
		m_pPauseMenu->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pPauseMenu->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pResumeGame = new TextBox(500.0f, 100.0f);
		m_pResumeGame->SetText(m_font, "Resume Game");
		m_pResumeGame->SetColor(0.0f, 0.2f, 0.2f, 1.0f);
		m_pResumeGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pResumeGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);

		m_pQuitGame = new TextBox(500.0f, 100.0f);
		m_pQuitGame->SetText(m_font, "Quit Game");
		m_pQuitGame->SetColor(0.0f, 0.2f, 0.2f, 1.0f);
		m_pQuitGame->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_pQuitGame->SetVerticalAlignment(TextBox::Alignment::AL_Center);
	}
}

void StatePauseMenu::Update(float p_fDelta)
{
	// m_pSkybox->update(p_fDelta);
	m_pPauseMenu->SetPos(-250.0f, 200.0f, -600.0f);
	m_pResumeGame->SetPos(-250.0f, 50.0f, -600.0f);
	m_pQuitGame->SetPos(-250.0f, -100.0f, -600.0f);

	int iButton = m_pApp->isLMBDown();
	if (iButton)
	{
		int x, y = 0;
		x = m_pApp->getMousePos().x;
		y = m_pApp->getMousePos().y;
		if (x > m_pResumeGame->GetPos().x && x < m_pResumeGame->GetPos().x + m_pResumeGame->GetWidth() && y > m_pResumeGame->GetPos().y && y < m_pResumeGame->GetPos().y + m_pResumeGame->GetHeight())
		{
			// Resume Game
			m_pStateMachine->GoToState(eStateGameplay_Gameplay);
		}
		else if (x > m_pQuitGame->GetPos().x && x < m_pQuitGame->GetPos().x + m_pQuitGame->GetWidth() && y > m_pQuitGame->GetPos().y && y < m_pQuitGame->GetPos().y + m_pQuitGame->GetHeight())
		{
			// Exit Game
			m_pApp->exit();
		}
	}
}

void StatePauseMenu::Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pWorldProgram->SetUniform("u_lightDir", m_sunDirection);

	m_pSkybox->render(mProj, mView, width, height);
	m_pPauseMenu->Render(mProj, mView);
	m_pResumeGame->Render(mProj, mView);
	m_pQuitGame->Render(mProj, mView);
}

int StatePauseMenu::_randomNum(int lowerBound, int upperBound)
{
	// Generates random number in range. Includes lower bound and upper bound
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}

void StatePauseMenu::RegisterCamera(Camera *pCam)
{
	m_pCam = pCam;
}