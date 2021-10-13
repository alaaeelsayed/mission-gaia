#include "line.h"

Line::Line(float maxWidth) : m_widthMax(maxWidth)
{
}

Line::~Line()
{
}

bool Line::AddWord(const Word& word)
{
    float newWidth = m_width + word.GetSpaceWidth() + word.GetWidth();
    if(newWidth <= m_widthMax) {
        m_words.push_back(word);
        m_width = newWidth;
        return true;
    }
    return false;
}

float Line::GetCurrentWidth() const
{
    return m_width;
}

float Line::GetLineHeight() const
{
    // we use the lineheight of the first word's font
    return m_words.empty() ? 0.0f : m_words[0].GetFont()->GetLineHeight();    
}

float Line::GetMaxWidth() const
{
    return m_widthMax;
}

const std::vector<Word>& Line::GetWords() const
{
    return m_words;
}