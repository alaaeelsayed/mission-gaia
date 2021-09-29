#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

class Gaia : public Sample
{
public:
    class GimpProvider : public wolf::Model::MaterialProvider
    {
    public:
        GimpProvider() {}
        wolf::Material *getMaterial(const std::string &nodeName, int subMeshIndex, const std::string &name) const;
    };
    Gaia(wolf::App *pApp) : Sample(pApp, "3D World Transform") {}
    ~Gaia();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    GLuint m_program = 0;
    GLuint m_vboVerts = 0;
    GLuint m_vboColors = 0;
    GLuint m_vao = 0;

    wolf::Model *m_model;

    wolf::Model::MaterialProvider m_materialProvider;

    float m_currRot = 0.0f;
};
