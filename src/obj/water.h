#pragma once

#include "../../wolf/wolf.h"

class Water
{
public:
    Water();
    virtual ~Water();
    void update(float dt);
    void render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height);

private:
};