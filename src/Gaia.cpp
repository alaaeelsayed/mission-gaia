#include "gaia.h"

Gaia::~Gaia()
{
    printf("Destroying Gaia\n");
    delete m_pFreeRoamCamera;
    delete m_pStateMachine;
    delete m_font;
}

void Gaia::init()
{

    m_font = new Font("data/fonts/alpha.fnt", "data/textures/fonts/");
    // Initialize the StateMachine and supported states and go to the main statem_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
    m_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
    m_pFreeRoamCamera->setPosition(glm::vec3(10, 10, -20));
    m_pStateMachine = new Common::StateMachine();

    StateGameplay *pGameplay = new StateGameplay();
    pGameplay->RegisterCamera(m_pFreeRoamCamera);

    StateMainMenu *pMainMenu = new StateMainMenu();
    pMainMenu->RegisterCamera(m_pFreeRoamCamera);

    m_pStateMachine->RegisterState(eStateGameplay_Gameplay, pGameplay);
    m_pStateMachine->RegisterState(eStateGameplay_MainMenu, pMainMenu);
    m_pStateMachine->GoToState(eStateGameplay_Gameplay);

    m_textBox = new TextBox(800.0f, 300.0f);
    m_textBox->SetPos(240.0f, 5.0f);
    m_textBox->AddText(m_font, "Hey there");
    m_textBox->SetColor(0.1f, 0.0f, 0.0f, 1.0f);
    m_textBox->SetHorizontalAlignment(TextBox::Alignment::AL_Left);
    m_textBox->SetVerticalAlignment(TextBox::Alignment::AL_Top);

    printf("Successfully initialized Gaia\n");
}

void Gaia::update(float dt)
{
    if (m_pApp->isKeyDown('R') && !m_bKeyDown)
    {
        if (m_pStateMachine->GetCurrentState() == eStateGameplay_MainMenu)
        {
            m_pStateMachine->GoToState(eStateGameplay_Gameplay);
        }
        else
        {
            m_pStateMachine->GoToState(eStateGameplay_MainMenu);
        }
        m_bKeyDown = true;
    }

    if (!m_pApp->isKeyDown('R'))
        m_bKeyDown = false;

    m_pFreeRoamCamera->update(dt);
    m_pStateMachine->Update(dt);
}

void Gaia::render(int width, int height)
{
    glm::mat4 mProj = m_pFreeRoamCamera->getProjMatrix(width, height);
    glm::mat4 mView = m_pFreeRoamCamera->getViewMatrix();
    m_pStateMachine->Render(mProj, mView, width, height);
    m_textBox->Render(width, height);
}
