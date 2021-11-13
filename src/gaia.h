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

    void init();
    void update(float dt);
    void render(int width, int height);

private:
    Camera *m_camera = nullptr;
    wolf::App *m_app = nullptr;
    bool m_keyDown = false;
    Common::StateMachine *m_pStateMachine = 0;
};