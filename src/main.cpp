#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "gaia.h"

class ProjectGaia : public wolf::App
{
public:
    ProjectGaia() : App("Project Gaia")
    {
        m_gaia = new Gaia(this);
    }

    ~ProjectGaia()
    {
    }

    void update(float dt) override
    {
        m_gaia->update(dt);
    }

    void render() override
    {
        m_gaia->render(m_width, m_height);
    }

private:
    Gaia *m_gaia;
};

int main(int, char **)
{
    ProjectGaia gaia;
    gaia.run();
}