#pragma once
#include <memory>
#include "GLFW/glfw3.h"

inline float DeltaTime = 0.0f;
inline float LastFrame = 0.0f;

void UpdateDeltaTime()
{
    float currentFrame = glfwGetTime();
    DeltaTime = currentFrame - LastFrame;
    LastFrame = currentFrame;
}