#include "font.h"

Font::Font(const std::string& fntPath, const std::string& pngDir) : m_pngPath(pngDir)
{
    tinyxml2::XMLDocument fntDoc;
    fntDoc.LoadFile(fntPath.c_str());
    
    tinyxml2::XMLElement* commonElement = fntDoc.FirstChildElement("font")->FirstChildElement("common");

    commonElement->QueryIntAttribute("lineHeight", (int*) &m_charSet.lineHeight);
    commonElement->QueryIntAttribute("base", (int*) &m_charSet.base);
    commonElement->QueryIntAttribute("scaleW", (int*) &m_charSet.scaleW);
    commonElement->QueryIntAttribute("scaleH", (int*) &m_charSet.scaleH);

    tinyxml2::XMLElement* charElement = fntDoc.FirstChildElement("font")->FirstChildElement("chars")->FirstChildElement("char");

    while (charElement)
    {
        int charID = 0;
        charElement->QueryIntAttribute("id", &charID);
        charElement->QueryIntAttribute("x", (int*) &m_charSet.characters[charID].x);
        charElement->QueryIntAttribute("y", (int*) &m_charSet.characters[charID].y);
        charElement->QueryIntAttribute("width", (int*) &m_charSet.characters[charID].width);
        charElement->QueryIntAttribute("height", (int*) &m_charSet.characters[charID].height);
        charElement->QueryFloatAttribute("xoffset", &m_charSet.characters[charID].xOffset);
        charElement->QueryFloatAttribute("yoffset", &m_charSet.characters[charID].yOffset);
        charElement->QueryFloatAttribute("xadvance", &m_charSet.characters[charID].xAdvance);
        charElement->QueryIntAttribute("page", (int*) &m_charSet.characters[charID].page);
        charElement = charElement->NextSiblingElement();
    }

    tinyxml2::XMLElement* pageElement = fntDoc.FirstChildElement("font")->FirstChildElement("pages")->FirstChildElement("page");

    // assuming pages are in order
    while(pageElement)
    {
        const char* pngFile = pageElement->Attribute("file");
        
        wolf::Texture* texture = wolf::TextureManager::CreateTexture(m_pngPath + pngFile);
        m_textures.push_back(texture);

        pageElement = pageElement->NextSiblingElement();
    }
}

Font::~Font()
{
    for(auto it = m_textures.begin(); it != m_textures.end(); it++)
    {
        wolf::Texture* texture = *it;
        wolf::TextureManager::DestroyTexture(texture);
    }
}

const wolf::Texture* Font::GetTextureForChar(char character) const
{
    return m_textures[m_charSet.characters[character].page];
}

void Font::GetExtentsForChar(char character, float* uStart, float* uEnd, float* vStart, float* vEnd) const
{
    short charX = m_charSet.characters[character].x;
    short charY = m_charSet.characters[character].y;
    short width = m_charSet.characters[character].width;
    short height = m_charSet.characters[character].height;
    
    *uStart = (float) charX / m_charSet.scaleW;
    *uEnd = (float) (charX + width) / m_charSet.scaleW;
    *vStart = (float) charY / m_charSet.scaleH;
    *vEnd = (float) (charY + height) / m_charSet.scaleH;
}

void Font::GetDataForChar(char character, float* xOffset, float* yOffset, float* xAdvance, float* width, float* height) const
{
    *xOffset = m_charSet.characters[character].xOffset;
    *yOffset = m_charSet.characters[character].yOffset;

    *xAdvance = m_charSet.characters[character].xAdvance;

    *width = m_charSet.characters[character].width;
    *height = m_charSet.characters[character].height;
}

int Font::GetLineHeight() const
{
    return m_charSet.lineHeight;
}