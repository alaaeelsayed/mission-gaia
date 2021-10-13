#include "texttable.h"

std::map<std::string, std::string> TextTable::m_propertyMap;
std::string TextTable::m_currentLanguage;
std::string TextTable::m_tablePath;

void TextTable::_setProperty(const std::string& line, int idLocation, int propertyLocation) 
{
    std::string propertyId;
    std::string propertyValue;

    std::istringstream iss(line);

    std::string s;
    for (int i = 0; std::getline(iss, s, '\t'); i++)
    {
        if(i == idLocation)
        {
            propertyId = s;
        }
        if(i == propertyLocation)
        {
            propertyValue = s;
        }

        if(!propertyId.empty() && !propertyValue.empty()) break;
    }
    
    m_propertyMap[propertyId] = propertyValue;
}

void TextTable::Load(const std::string& path) 
{
    m_tablePath = path;
}

void TextTable::SetLanguage(const std::string& language) 
{
    std::fstream inFile;

    inFile.open(m_tablePath, std::ios::in);

     if (inFile.fail()) {
        printf("Could not open file");
        return;
    }
    
    std::string line;

    std::getline(inFile, line);

    std::istringstream iss(line);

    int i = 0;
    std::string s;
    while (std::getline(iss, s, '\t'))
    {
        if(s == language) break;

        i++;
    }

    while(std::getline(inFile, line))
    {
        _setProperty(line, 0, i);
    }
}

void TextTable::SetString(const std::string& propertyName, const std::string& value) 
{
    m_propertyMap[propertyName] = value;
}

std::string TextTable::GetString(const std::string& propertyName) 
{
    return m_propertyMap[propertyName];
}
