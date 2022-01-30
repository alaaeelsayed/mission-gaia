#pragma once
#include "../wolf/wolf.h"
#include "states/states.h"
#include "states/statemachine.h"
#include "states/stategameplay.h"
#include "states/statepausemenu.h"

class Gaia
{
public:
    Gaia(wolf::App *pApp);
    ~Gaia();

    void Init();
    void Update(float dt);
    void Render(int width, int height);

private:
    wolf::App *m_app = nullptr;
    bool m_keyDown = false;
    bool m_debugKeyDown = false;
    bool m_debugMode = false;
    Common::StateMachine *m_stateMachine = 0;
};