#pragma once
#include <glm/glm.hpp>

#include "GLAD/glad.h"

#include <vector>
#include <functional>

namespace Mouse
{
    bool FirstMouse = true;
    float LastX = 400, LastY = 300;

    inline std::vector<std::function<void(float xoffset, float yoffset, float lastX, float lastY)>> MouseCallbacks;
    inline void AddMouseEvent(std::function<void(float xoffset, float yoffset, float lastX, float lastY)> Fn)
    {
        MouseCallbacks.emplace_back(Fn);
    }

    void MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (FirstMouse)
        {
            LastX = xpos;
            LastY = ypos;
            FirstMouse = false;
        }

        float XOffset = xpos - LastX;
        float YOffset = LastY - ypos;
        LastX = xpos;
        LastY = ypos;

        for (auto&& Callback : MouseCallbacks)
        {
            Callback(XOffset, YOffset, LastX, LastY);
        }
    }
}