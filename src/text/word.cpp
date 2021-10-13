#pragma once
#include "word.h"

Word::Word(const Font* font, const std::string string) : m_font(font), m_string(string)
{
    _calculateWidth();
}

Word::~Word()
{
}

void Word::_calculateWidth()
{
    float xOffset, yOffset, xAdvance, width, height;
    for(char character : m_string) 
    {
        m_font->GetDataForChar(character, &xOffset, &yOffset, &xAdvance, &width, &height);
        m_width += xAdvance;
    }
    
    m_font->GetDataForChar(' ', &xOffset, &yOffset, &xAdvance, &width, &height);
    m_spaceWidth = xAdvance;
}

float Word::GetWidth() const
{
    return m_width;
}

float Word::GetSpaceWidth() const
{
    return m_spaceWidth;
}

const std::string& Word::GetString() const
{
    return m_string;
}

const Font* Word::GetFont() const
{
    return m_font;
}