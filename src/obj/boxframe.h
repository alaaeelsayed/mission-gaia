#pragma once

#include "../../wolf/wolf.h"
#include "boundingbox.h"

struct BoxVertex
{
    GLfloat x, y, z;
    GLfloat r, g, b, a;
};

class BoxFrame
{
public:
    BoxFrame(const BoundingBox &box);
    virtual ~BoxFrame();

    virtual void Render(const glm::mat4 &mProj, const glm::mat4 &mView);

private:
    BoundingBox m_box;
    wolf::VertexBuffer *m_vb = 0;
    wolf::VertexDeclaration *m_decl = 0;
    wolf::Program *m_program = 0;
};