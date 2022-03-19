#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include "math.h"
#define _USE_MATH_DEFINES

namespace Util
{
    static std::vector<std::string> splitString(const std::string &p_sString, char p_cDelimeter)
    {
        std::vector<std::string> result;
        std::stringstream s_stream(p_sString);
        while (s_stream.good())
        {
            std::string substr;
            getline(s_stream, substr, p_cDelimeter);
            result.push_back(substr);
        }
        return result;
    }

    static int randNum(int p_iMin, int p_iMax)
    {
        return rand() % (p_iMax - p_iMin + 1) + p_iMin;
    }

    static float randNum(float p_fMin, float p_fMax)
    {
        return p_fMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (p_fMax - p_fMin)));
    }

    static glm::vec3 randVector(glm::vec3 p_vMin, glm::vec3 p_vMax)
    {
        float x = randNum(p_vMin.x, p_vMax.x);
        float y = randNum(p_vMin.y, p_vMax.y);
        float z = randNum(p_vMin.z, p_vMax.z);

        return glm::vec3(x, y, z);
    }

    static glm::vec4 randVector(glm::vec4 p_vMin, glm::vec4 p_vMax)
    {
        float x = randNum(p_vMin.x, p_vMax.x);
        float y = randNum(p_vMin.y, p_vMax.y);
        float z = randNum(p_vMin.z, p_vMax.z);
        float w = randNum(p_vMin.w, p_vMax.w);

        return glm::vec4(x, y, z, w);
    }

    static bool inProximity(const glm::vec3 &p_vPos, const glm::vec3 &p_vPos2, const glm::vec3 &p_vRange)
    {
        float xDist = std::abs(p_vPos.x - p_vPos2.x);
        float yDist = std::abs(p_vPos.y - p_vPos2.y);
        float zDist = std::abs(p_vPos.z - p_vPos2.z);
        return xDist <= p_vRange.x && yDist <= p_vRange.y && zDist <= p_vRange.z;
    }

    static bool randBool()
    {
        const int shift = static_cast<int>(std::log2(RAND_MAX));
        return (rand() >> shift) & 1;
    }

    static float barryCentric(const glm::vec3 &firstPos, const glm::vec3 &secondPos, const glm::vec3 &thirdPos, const glm::vec2 &position)
    {
        float det = (secondPos.z - thirdPos.z) * (firstPos.x - thirdPos.x) + (thirdPos.x - secondPos.x) * (firstPos.z - thirdPos.z);
        float l1 = ((secondPos.z - thirdPos.z) * (position.x - thirdPos.x) + (thirdPos.x - secondPos.x) * (position.y - thirdPos.z)) / det;
        float l2 = ((thirdPos.z - firstPos.z) * (position.x - thirdPos.x) + (firstPos.x - thirdPos.x) * (position.y - thirdPos.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * firstPos.y + l2 * secondPos.y + l3 * thirdPos.y;
    }

    static bool isAllTrue(std::vector<bool> arr)
    {
        for (int i = 0; i < arr.size(); i++)
        {
            if (!arr[i])
                return false;
        }
        return true;
    }
};