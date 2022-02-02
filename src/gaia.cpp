#include "gaia.h"
#include "scene/scene.h"

Gaia::Gaia(wolf::App *app) : m_app(app)
{
    Init();
}

Gaia::~Gaia()
{
    printf("Destroying Gaia\n");
    delete m_stateMachine;
}

void Gaia::Init()
{
    Scene::CreateInstance();
    Culling::CreateInstance();

    Camera *camera = new FreeRoamCamera(m_app);
    camera->SetPosition(glm::vec3(10, 10, -20));

    Scene::Instance()->SetActiveCamera(camera);

    m_stateMachine = new Common::StateMachine();

    StateGameplay *gameplay = new StateGameplay();
    gameplay->RegisterApp(m_app);

    StatePauseMenu *pauseMenu = new StatePauseMenu();
    pauseMenu->RegisterApp(m_app);

    StateMainMenu *mainMenu = new StateMainMenu();
    mainMenu->RegisterApp(m_app);

    StateRespawn *respawn = new StateRespawn();
    respawn->RegisterApp(m_app);

    m_stateMachine->RegisterState(eStateGameplay_Gameplay, gameplay);
    m_stateMachine->RegisterState(eStateGameplay_PauseMenu, pauseMenu);
    m_stateMachine->RegisterState(eStateGameplay_MainMenu, mainMenu);
    m_stateMachine->RegisterState(eStateGameplay_Respawn, respawn);
    m_stateMachine->GoToState(eStateGameplay_MainMenu);

    Scene::Instance()->SetStateMachine(m_stateMachine);

    printf("Successfully initialized Gaia\n");
}

void Gaia::Update(float dt)
{
    if (m_app->isKeyDown(GLFW_KEY_ESCAPE) && !m_keyDown && !(m_stateMachine->GetCurrentState() == eStateGameplay_Respawn || m_stateMachine->GetCurrentState() == eStateGameplay_MainMenu))
    {
        if (m_stateMachine->GetCurrentState() == eStateGameplay_PauseMenu)
        {
            m_stateMachine->GoToState(eStateGameplay_Gameplay);
        }
        else
        {
            m_stateMachine->GoToState(eStateGameplay_PauseMenu);
        }
        m_keyDown = true;
    }

    if (!m_app->isKeyDown(GLFW_KEY_ESCAPE))
        m_keyDown = false;

    if (m_stateMachine->GetCurrentState() != eStateGameplay_PauseMenu && m_stateMachine->GetCurrentState() != eStateGameplay_MainMenu && m_stateMachine->GetCurrentState() != eStateGameplay_Respawn && !m_debugMode)
        Scene::Instance()->GetActiveCamera()->Update(dt);

    if (!m_app->isKeyDown('M'))
    {
        m_debugKeyDown = false;
    }

    if (m_app->isKeyDown('M') && !m_debugKeyDown)
    {
        m_debugKeyDown = true;
        m_debugMode = !m_debugMode;
    }

    if (m_debugMode || m_stateMachine->GetCurrentState() == eStateGameplay_PauseMenu || m_stateMachine->GetCurrentState() == eStateGameplay_Respawn || m_stateMachine->GetCurrentState() == eStateGameplay_MainMenu)
    {
        m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        m_app->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    m_stateMachine->Update(dt);
    Scene::Instance()->Update(dt);
}

void Gaia::Render(int width, int height)
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera *camera = Scene::Instance()->GetActiveCamera();
    Scene::Instance()->SetWidth(width);
    Scene::Instance()->SetHeight(height);
    Scene::Instance()->Render();
    m_stateMachine->Render(camera->GetProjMatrix(width, height), camera->GetViewMatrix());
}
