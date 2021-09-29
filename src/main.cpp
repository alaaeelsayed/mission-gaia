#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "../samplefw/SampleRunner.h"
#include "Gaia.h"

class ProjectGaia : public wolf::App
{
public:
    ProjectGaia() : App("Project Gaia")
    {
        m_sampleRunner.addSample(new Gaia(this));
    }

    ~ProjectGaia()
    {
    }

    void update(float dt) override
    {
        if (isKeyDown(' '))
        {
            m_lastDown = true;
        }
        else if (m_lastDown)
        {
            m_sampleRunner.nextSample();
            m_lastDown = false;
        }

        m_sampleRunner.update(dt);
    }

    void render() override
    {
        m_sampleRunner.render(m_width, m_height);
    }

private:
    SampleRunner m_sampleRunner;
    bool m_lastDown = false;
};

int main(int, char **)
{
    ProjectGaia gaia;
    gaia.run();
}