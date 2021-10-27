#include "./StatePauseMenu.h"
#include "States.h"

StatePauseMenu::StatePauseMenu()
{
}

StatePauseMenu::~StatePauseMenu()
{
}

void StatePauseMenu::Exit()
{
	delete m_pPlane;
	wolf::ProgramManager::DestroyProgram(m_pWorldProgram);
	delete m_pSkybox;
	delete m_font;
}

void StatePauseMenu::Enter(std::string arg)
{
	if (!m_pWorldProgram)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_DEPTH_TEST);

		m_font = new Font("data/fonts/alpha.fnt", "data/textures/fonts/");

		m_pWorldProgram = wolf::ProgramManager::CreateProgram("data/shaders/world.vsh", "data/shaders/world.fsh");

		m_pPlane = new Plane(m_pWorldProgram, m_groundTexPath);
		m_pPlane->setScale(500.0f);
		if (m_pCam)
		{
			m_pSkybox = new Skybox(m_pWorldProgram, m_pCam, m_skyboxPath);
		}

		m_textBox = new TextBox(800.0f, 300.0f);
		m_textBox->SetPos(300.0f, 5.0f);
		m_textBox->AddText(m_font, "Pause Menu");
		m_textBox->SetColor(0.1f, 0.0f, 0.0f, 1.0f);
		m_textBox->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
		m_textBox->SetVerticalAlignment(TextBox::Alignment::AL_Top);
	}
}

void StatePauseMenu::Update(float p_fDelta)
{
	m_pSkybox->update(p_fDelta);
}

void StatePauseMenu::Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height)
{

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pWorldProgram->SetUniform("u_lightDir", m_sunDirection);

	if (m_pCam)
		m_pSkybox->render(mProj, mView, width, height);
	m_pPlane->render(mProj, mView, width, height);
	m_textBox->Render(width, height);
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