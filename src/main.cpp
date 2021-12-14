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
    // FILE *imageFile;
    // int height = 1024, width = 1024;

    // imageFile = fopen("image.pgm", "wb");
    // if (imageFile == NULL)
    // {
    //     perror("ERROR: Cannot open output file");
    //     exit(EXIT_FAILURE);
    // }

    // fprintf(imageFile, "P5\n");
    // fprintf(imageFile, "%d %d\n", width, height);
    // fprintf(imageFile, "255\n");
    // TerrainGenerator generator;
    // for (int j = 0; j < height; j++)
    // {
    //     for (int i = 0; i < width; i++)
    //     {
    //         float value = generator.GenerateHeight(i, j, 0, 0);
    //         fputc(((value + 60.0f) / 120.0f) * 255, imageFile);
    //     }
    // }

    // fclose(imageFile);
    ProjectGaia gaia;
    gaia.run();
}