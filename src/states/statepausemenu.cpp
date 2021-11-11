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

		m_textBox = new TextBox(200.0f, 200.0f);
		m_textBox->SetPos(0.0f, 0.0f);
		m_textBox->SetText(m_font, "Pause Menu\n\nResume Game\n\nExit Game");
		m_textBox->SetColor(0.1f, 0.0f, 0.0f, 1.0f);
		m_textBox->SetHorizontalAlignment(TextBox::Alignment::AL_Center);
		m_textBox->SetVerticalAlignment(TextBox::Alignment::AL_Center);
	}
}

void StatePauseMenu::Update(float p_fDelta)
{
	// m_pSkybox->update(p_fDelta);

	int iButton = m_pApp->isLMBDown();
	if (iButton)
	{
		int x, y = 0;
		x = m_pApp->getMousePos().x;
		y = m_pApp->getMousePos().y;
		if (x > 392 && x < 1005 && y > 284 && y < 369)
		{
			// Resume Game
			m_pStateMachine->GoToState(eStateGameplay_Gameplay);
		}
		else if (x > 391 && x < 818 && y > 543 && y < 619)
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
	m_textBox->Render(mProj, mView, width, height);
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