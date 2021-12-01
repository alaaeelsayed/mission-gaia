#pragma once
#include <string>
#include "font.h"
#include "line.h"
#include <stdarg.h>

class TextBox
{
public:
    enum Alignment
    {
        AL_Center = 0,
        AL_Left,
        AL_Right,
        AL_Top,
        AL_Bottom
    };

    TextBox(float width, float height);
    ~TextBox();

    void SetPos(float x, float y, float z);

    void SetColor(float r, float g, float b, float a);

    void SetText(const Font *font, const char *text, ...);
    void AddText(const Font *font, const char *text, ...);

    void SetHorizontalAlignment(Alignment alignment);
    void SetVerticalAlignment(Alignment alignment);

    void SetOutlined(bool outlined);

    glm::vec2 GetPos() const;
    float GetWidth() const;
    float GetHeight() const;

    void Render(const glm::mat4 &mProj, const glm::mat4 &mView);

private:
    void _clearLines();
    void _clearVertices();
    void _calculateVertices();
    void _addText(const Font *font, const std::string &text);
    void _addVertices(std::vector<float> &vertices, float xStart, float xEnd, float yStart, float yEnd, float uStart, float uEnd, float vStart, float vEnd);

    float _getCurrentHeight();

    float m_x, m_y, m_z = 0.0f;
    float m_colorR = 1.0f, m_colorG = 0.0f, m_colorB = 0.0f, m_colorA = 1.0f;
    float m_width, m_height;
    bool m_outlined = false;

    Alignment m_horizontalAlignment = AL_Left, m_verticalAlignment = AL_Top;

    std::vector<Line *> m_lines;
    // Keeps track of the number of vertices per texture/vertexdeclaration
    std::map<const wolf::Texture *, int> m_verticesCount;
    // asumming map treats pointers as memory addresses (integers) this should be safe?
    // this looks weird to me but seems to be reasonable
    std::map<const wolf::Texture *, wolf::VertexDeclaration *> m_vertices;

    // used when we need to destroy them
    std::vector<wolf::VertexBuffer *> m_vbs;
    std::vector<wolf::VertexDeclaration *> m_decls;

    // needed to draw the outline
    wolf::VertexBuffer *m_outlineVB = 0;
    wolf::VertexDeclaration *m_outlineDecl = 0;

    wolf::Program *m_program = 0;
};