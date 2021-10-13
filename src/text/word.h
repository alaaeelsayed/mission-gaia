#pragma once
#include "font.h"
#include <string>

class Word
{
public:
    Word(const Font* font, const std::string wordString);
    ~Word();

    const std::string& GetString() const;

    const Font* GetFont() const;

    float GetWidth() const;
    float GetSpaceWidth() const;

    void _calculateWidth();
private:
    float m_width = 0;
    float m_spaceWidth = 0;

    std::string m_string;

    const Font* m_font;
};