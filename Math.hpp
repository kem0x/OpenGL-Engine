#pragma once
#include <glm/glm.hpp>

struct Location
{
    constexpr static float ScaleFactor = 0.01f;

    glm::vec3 Vec;

    static Location FromWorldCoords(glm::ivec3 InCoords)
    {
        return { glm::vec3(InCoords) * ScaleFactor };
    }

    glm::ivec3 GetWorldCoords()
    {
        return Vec / ScaleFactor;
    }
};