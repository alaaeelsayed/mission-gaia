#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "./States.h"
#include "./StateMachine.h"
#include "./StateGameplay.h"
#include "./StateMainMenu.h"

#define ESC 27

class Gaia : public Sample
{
public:
    Gaia(wolf::App *pApp) : Sample(pApp, "Generating Gaia") {}
    ~Gaia();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    FreeRoamCamera *m_pFreeRoamCamera = 0;
    bool m_bKeyDown = false;
    Common::StateMachine *m_pStateMachine = 0;
};