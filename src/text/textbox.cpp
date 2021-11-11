#include "textbox.h"

TextBox::TextBox(float width, float height) : m_width(width), m_height(height)
{
    GLfloat gs_outlineVertices[] = {
        -0.5f,
        -0.5f,
        width,
        -0.5f,

        width,
        -0.5f,
        width,
        height,

        width,
        height,
        -0.5f,
        height,

        -0.5f,
        height,
        -0.5f,
        -0.5f,
    };

    // Appreciate wolf for caching all of this
    m_program = wolf::ProgramManager::CreateProgram("data/shaders/text.vsh", "data/shaders/text.fsh");

    m_outlineVB = wolf::BufferManager::CreateVertexBuffer(gs_outlineVertices, sizeof(GLfloat) * 4 * 5);

    m_outlineDecl = new wolf::VertexDeclaration();
    m_outlineDecl->Begin();
    m_outlineDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
    m_outlineDecl->SetVertexBuffer(m_outlineVB);
    m_outlineDecl->End();
}

TextBox::~TextBox()
{
    _clearLines();
    _clearVertices();
    wolf::ProgramManager::DestroyProgram(m_program);
    wolf::BufferManager::DestroyBuffer(m_outlineVB);
    delete m_outlineDecl;
}

void TextBox::_clearLines()
{
    for (auto it = m_lines.begin(); it != m_lines.end(); ++it)
    {
        auto line = *it;
        delete line;
    }
    m_lines.clear();
}

void TextBox::_clearVertices()
{
    for (auto it = m_vbs.begin(); it != m_vbs.end(); ++it)
    {
        auto buffer = *it;
        wolf::BufferManager::DestroyBuffer(buffer);
    }

    for (auto it = m_decls.begin(); it != m_decls.end(); ++it)
    {
        auto decl = *it;
        delete decl;
    }
    m_vertices.clear();
    m_vbs.clear();
    m_decls.clear();
}

void TextBox::_calculateVertices()
{
    _clearVertices();

    std::map<const wolf::Texture *, std::vector<GLfloat>> tempFloats;

    float currX = 0;
    float currY = 0;
    float currHeight = _getCurrentHeight();
    if (m_verticalAlignment == AL_Center)
    {
        currY = (m_height - currHeight) / 2;
    }
    else if (m_verticalAlignment == AL_Bottom)
    {
        currY = (m_height - currHeight);
    }
    for (Line *line : m_lines)
    {
        if (m_horizontalAlignment == AL_Center)
        {
            currX = (line->GetMaxWidth() - line->GetCurrentWidth()) / 2;
        }
        else if (m_horizontalAlignment == AL_Right)
        {
            currX = line->GetMaxWidth() - line->GetCurrentWidth();
        }
        // left/top handled by default

        float lineHeight = line->GetLineHeight();

        for (Word word : line->GetWords())
        {
            const Font *font = word.GetFont();
            const wolf::Texture *currentTexture = nullptr;

            for (char character : word.GetString())
            {
                if (currentTexture != font->GetTextureForChar(character))
                {
                    currentTexture = font->GetTextureForChar(character);
                }
                std::vector<GLfloat> &floats = tempFloats[currentTexture];

                float uStart, uEnd, vStart, vEnd;
                float xAdvance, xOffset, yOffset, width, height;

                font->GetExtentsForChar(character, &uStart, &uEnd, &vStart, &vEnd);
                font->GetDataForChar(character, &xOffset, &yOffset, &xAdvance, &width, &height);

                float x = currX - xOffset;
                float y = currY - yOffset;

                _addVertices(floats, x, y, x + width, y + height, uStart, uEnd, vStart, vEnd);

                currX -= xAdvance;
            }
            currX -= word.GetSpaceWidth();
        }
        currX = 0;
        currY -= lineHeight;
    }
    for (std::map<const wolf::Texture *, std::vector<GLfloat>>::iterator it = tempFloats.begin(); it != tempFloats.end(); ++it)
    {
        const wolf::Texture *texture = it->first;
        std::vector<GLfloat> floats = it->second;

        if (floats.empty())
        {
            continue;
        }
        int floatsCount = (int)floats.size();
        // OpenGL will copy the contents to a memory buffer, the floats vector alongside the map should be gone by the end of this method
        wolf::VertexBuffer *vb = wolf::BufferManager::CreateVertexBuffer(&floats[0], sizeof(GLfloat) * floatsCount);

        m_verticesCount[texture] = floatsCount / 4;

        wolf::VertexDeclaration *decl = new wolf::VertexDeclaration();
        decl->Begin();
        decl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
        decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
        decl->SetVertexBuffer(vb);
        decl->End();

        m_vbs.push_back(vb);
        m_decls.push_back(decl);
        m_vertices[texture] = decl;
    }
}

void TextBox::_addText(const Font *font, const std::string &text)
{
    Line *currentLine = m_lines.empty() ? new Line(m_width) : m_lines.back();
    std::string currentWord;
    for (char character : text)
    {
        if ((character == ' ' || character == '\n'))
        {
            Word word(font, currentWord);
            if (!currentLine->AddWord(word))
            {
                m_lines.push_back(currentLine);
                currentLine = new Line(m_width);
                currentLine->AddWord(word);
            }
            currentWord.clear();
            if (character == '\n')
            {
                Word word(font, currentWord);
                currentLine->AddWord(word);
                m_lines.push_back(currentLine);
                currentLine = new Line(m_width);
            }
            continue;
        }
        currentWord += character;
    }
    if (!currentWord.empty())
    {
        Word word(font, currentWord);
        if (!currentLine->AddWord(word))
        {
            m_lines.push_back(currentLine);
            currentLine = new Line(m_width);
            currentLine->AddWord(word);
        }
    }
    if (m_lines.empty() || currentLine != m_lines.back())
    {
        m_lines.push_back(currentLine);
    }
}

void TextBox::_addVertices(std::vector<GLfloat> &floats, float xStart, float yStart, float xEnd, float yEnd, float uStart, float uEnd, float vStart, float vEnd)
{
    // Top left
    floats.push_back(xStart);
    floats.push_back(yStart);
    floats.push_back(uEnd);
    floats.push_back(vEnd);

    // Top right
    floats.push_back(xEnd);
    floats.push_back(yStart);
    floats.push_back(uStart);
    floats.push_back(vEnd);

    // Bottom right
    floats.push_back(xEnd);
    floats.push_back(yEnd);
    floats.push_back(uStart);
    floats.push_back(vStart);

    // Bottom right
    floats.push_back(xEnd);
    floats.push_back(yEnd);
    floats.push_back(uStart);
    floats.push_back(vStart);

    // Bottom left
    floats.push_back(xStart);
    floats.push_back(yEnd);
    floats.push_back(uEnd);
    floats.push_back(vStart);

    // Top left
    floats.push_back(xStart);
    floats.push_back(yStart);
    floats.push_back(uEnd);
    floats.push_back(vEnd);
}

float TextBox::_getCurrentHeight()
{
    float height = 0.0f;
    for (Line *line : m_lines)
    {
        height += line->GetLineHeight();
    }
    return height;
}

void TextBox::SetPos(float x, float y)
{
    m_x = x;
    m_y = y;
}

void TextBox::SetColor(float r, float g, float b, float a)
{
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_colorA = a;
}

void TextBox::SetText(const Font *font, const char *text, ...)
{
    va_list args;
    va_start(args, text);

    int size = vsnprintf(nullptr, 0, text, args) + 1;
    char *formatted = new char[size];
    vsnprintf(formatted, size, text, args);
    std::string str(formatted);

    delete[] formatted;

    _clearLines();
    _addText(font, str);
    _calculateVertices();
}

void TextBox::AddText(const Font *font, const char *text, ...)
{
    va_list args;
    va_start(args, text);

    int size = vsnprintf(nullptr, 0, text, args) + 1;
    char *formatted = new char[size];
    vsnprintf(formatted, size, text, args);
    std::string str(formatted);

    delete[] formatted;

    _addText(font, str);
    // Only recalculate if using the public AddText method
    // in case we may need to add intermediate text and recalculate the verties in bulk
    _calculateVertices();
}

void TextBox::SetHorizontalAlignment(Alignment alignment)
{
    switch (alignment)
    {
    case AL_Top:
        alignment = AL_Left;
        break;
    case AL_Bottom:
        alignment = AL_Right;
        break;
    default:
        break;
    }
    m_horizontalAlignment = alignment;
    _calculateVertices();
}

void TextBox::SetVerticalAlignment(Alignment alignment)
{
    switch (alignment)
    {
    case AL_Left:
        alignment = AL_Top;
        break;
    case AL_Right:
        alignment = AL_Bottom;
        break;
    default:
        break;
    }
    m_verticalAlignment = alignment;
    _calculateVertices();
}

void TextBox::SetOutlined(bool outlined)
{
    m_outlined = outlined;
}

glm::vec2 TextBox::GetPos() const
{
    return glm::vec2(m_x, m_y);
}

float TextBox::GetWidth() const
{
    return m_width;
}

float TextBox::GetHeight() const
{
    return m_height;
}

// 1 draw call per texture (aka 1 call per fontsheet)
void TextBox::Render(const glm::mat4 &mProj, const glm::mat4 &mView, int width, int height)
{
    glm::mat4 mWorld = glm::mat4(1.0f);
    mWorld *= glm::scale(glm::mat4(1.0f), glm::vec3(m_width / 1000.0f, m_height / 1000.0f, 1.0f));
    // draw the outline first
    if (m_outlined)
    {
        m_program->SetUniform("projection", mProj);
        m_program->SetUniform("view", mView);
        m_program->SetUniform("world", mWorld);

        m_program->SetUniform("u_outline", true);
        m_program->SetUniform("u_translation", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        m_program->SetUniform("u_color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        m_program->Bind();
        m_outlineDecl->Bind();

        glDrawArrays(GL_LINES, 0, 8);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto it = m_vertices.begin(); it != m_vertices.end(); ++it)
    {
        const wolf::Texture *texture = it->first;
        wolf::VertexDeclaration *decl = it->second;

        m_program->SetUniform("projection", mProj);
        m_program->SetUniform("view", mView);
        m_program->SetUniform("world", mWorld);

        m_program->SetUniform("u_outline", false);
        m_program->SetUniform("u_translation", glm::vec4(m_x, m_y, 0.0f, 0.0f));
        m_program->SetUniform("u_color", glm::vec4(m_colorR, m_colorG, m_colorB, m_colorA));
        m_program->SetTexture("u_tex", texture);

        m_program->Bind();
        decl->Bind();

        glDrawArrays(GL_TRIANGLES, 0, m_verticesCount[texture]);
    }
    glDisable(GL_BLEND);
}