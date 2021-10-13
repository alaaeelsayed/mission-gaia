#pragma once
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class TextTable
{
public:
    // doesn't actually load anything just prepares
    static void Load(const std::string& path);
    // actually loads the language into memory
    static void SetLanguage(const std::string& language);

    static void SetString(const std::string& propertyName, const std::string& value);
    static std::string GetString(const std::string& propertyName);
private:
    // don't
    TextTable() {};

    static void _setProperty(const std::string& line, int idLocation, int propertyLocation);

    static std::map<std::string, std::string> m_propertyMap;
    static std::string m_currentLanguage;
    static std::string m_tablePath;
};