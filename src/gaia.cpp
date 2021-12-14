#include "gaia.h"

Gaia::Gaia(wolf::App *app) : m_app(app)
{
    init();
}

Gaia::~Gaia()
{
    printf("Destroying Gaia\n");
    delete m_camera;
    delete m_pStateMachine;
}

void Gaia::init()
{
    // Initialize the StateMachine and supported states and go to the main statem_pFreeRoamCamera = new FreeRoamCamera(m_pApp);
    m_camera = new FreeRoamCamera(m_app);
    m_camera->setPosition(glm::vec3(10, 10, -20));
    m_pStateMachine = new Common::StateMachine();

    StateGameplay *pGameplay = new StateGameplay();
    pGameplay->RegisterCamera(m_camera);
    pGameplay->RegisterApp(m_app);

    StatePauseMenu *pPauseMenu = new StatePauseMenu();
    pPauseMenu->RegisterCamera(m_camera);
    pPauseMenu->RegisterApp(m_app);

    m_pStateMachine->RegisterState(eStateGameplay_Gameplay, pGameplay);
    m_pStateMachine->RegisterState(eStateGameplay_PauseMenu, pPauseMenu);
    m_pStateMachine->GoToState(eStateGameplay_Gameplay);

    printf("Successfully initialized Gaia\n");
}

void Gaia::update(float dt)
{
    if (m_app->isKeyDown(GLFW_KEY_ESCAPE) && !m_bKeyDown)
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

    if (!m_app->isKeyDown(GLFW_KEY_ESCAPE))
        m_bKeyDown = false;

    if (m_pStateMachine->GetCurrentState() != eStateGameplay_PauseMenu && !m_bDebugMode)
        m_camera->update(dt);

    if (!m_app->isKeyDown('M'))
    {
        m_bDebugKeyDown = false;
    }

    if (m_app->isKeyDown('M') && !m_bDebugKeyDown)
    {
        m_bDebugKeyDown = true;
        m_bDebugMode = !m_bDebugMode;
    }

    if (m_bDebugMode || m_pStateMachine->GetCurrentState() == eStateGameplay_PauseMenu)
    {
        m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    m_pStateMachine->Update(dt);
}

void Gaia::render(int width, int height)
{
    glm::mat4 mProj = m_camera->getProjMatrix(width, height);
    glm::mat4 mView = m_camera->getViewMatrix();
    m_pStateMachine->Render(mProj, mView, width, height);
}
