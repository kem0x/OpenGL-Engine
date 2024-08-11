#pragma once
#include <vector>
#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoord;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int BoneIDs[MAX_BONE_INFLUENCE];
    float Weights[MAX_BONE_INFLUENCE];
};