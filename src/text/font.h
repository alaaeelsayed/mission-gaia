#pragma once
#include <string>
#include "../../tinyxml2/tinyxml2.h"
#include "../../wolf/wolf.h"

class Font
{
public:
    struct CharacterDescriptor
    {
        short x, y;
        short width, height;
        float xOffset, yOffset;
        float xAdvance;
        short page;

        CharacterDescriptor() : x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0)
        {
        }
    };

    struct CharacterSet
    {
        short pageCount;
        short lineHeight;
        short base;
        short scaleW, scaleH;
        Font::CharacterDescriptor characters[256];

        CharacterSet() : pageCount(0), lineHeight(0), base(0), scaleW(0), scaleH(0)
        {
        }
    };

    // pngDir should contain all the textures defined in the pages of the .fnt file (should end with /)
    Font(const std::string &fntPath, const std::string &pngDir);
    ~Font();

    const wolf::Texture *GetTextureForChar(char character) const;

    void GetExtentsForChar(char character, float *uStart, float *uEnd, float *vStart, float *vEnd) const;
    void GetDataForChar(char character, float *xOffset, float *yOffset, float *xAdvance, float *width, float *height) const;

    int GetLineHeight() const;

private:
    CharacterSet m_charSet;
    std::string m_pngPath;
    std::vector<wolf::Texture *> m_textures;
};