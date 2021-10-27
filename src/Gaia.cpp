#include "gaia.h"

Gaia::~Gaia()
{
    printf("Destroying Gaia\n");
    delete m_pFreeRoamCamera;
    delete m_pStateMachine;
}

void Gaia::init()
{

    // Initialize the StateMachine and supported states and go to the main statem_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
    m_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
    m_pFreeRoamCamera->setPosition(glm::vec3(10, 10, -20));
    m_pStateMachine = new Common::StateMachine();

    StateGameplay *pGameplay = new StateGameplay();
    pGameplay->RegisterCamera(m_pFreeRoamCamera);
    pGameplay->RegisterApp(m_pApp);

    StatePauseMenu *pPauseMenu = new StatePauseMenu();
    pPauseMenu->RegisterCamera(m_pFreeRoamCamera);
    pPauseMenu->RegisterApp(m_pApp);

    m_pStateMachine->RegisterState(eStateGameplay_Gameplay, pGameplay);
    m_pStateMachine->RegisterState(eStateGameplay_PauseMenu, pPauseMenu);
    m_pStateMachine->GoToState(eStateGameplay_Gameplay);

    printf("Successfully initialized Gaia\n");
}

void Gaia::update(float dt)
{
    if (m_pApp->isKeyDown(GLFW_KEY_ESCAPE) && !m_bKeyDown)
    {
        if (m_pStateMachine->GetCurrentState() == eStateGameplay_PauseMenu)
        {
            m_pStateMachine->GoToState(eStateGameplay_Gameplay);
        }
        else
        {
            m_pStateMachine->GoToState(eStateGameplay_PauseMenu);
        }
        m_bKeyDown = true;
    }

    if (!m_pApp->isKeyDown(GLFW_KEY_ESCAPE))
        m_bKeyDown = false;

    if (m_pStateMachine->GetCurrentState() != eStateGameplay_PauseMenu)
        m_pFreeRoamCamera->update(dt);
    m_pStateMachine->Update(dt);
}

void Gaia::render(int width, int height)
{
    glm::mat4 mProj = m_pFreeRoamCamera->getProjMatrix(width, height);
    glm::mat4 mView = m_pFreeRoamCamera->getViewMatrix();
    m_pStateMachine->Render(mProj, mView, width, height);
}
