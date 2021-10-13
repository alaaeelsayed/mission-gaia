#pragma once
#include "word.h"

class Line
{
public:
    Line(float maxWidth);
    ~Line();

    bool AddWord(const Word& word);

    float GetMaxWidth() const;
    float GetCurrentWidth() const;
    
    float GetLineHeight() const;

    const std::vector<Word>& GetWords() const;
private:
    float m_width = 0, m_widthMax;

    std::vector<Word> m_words;
};