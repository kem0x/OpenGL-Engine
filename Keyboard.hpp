#pragma once
#include "GLAD/glad.h"

#include "GLGlobals.hpp"

#include <vector>
#include <functional>
#include <utility>

// TODO: rework this garbage.
namespace Keyboard
{
    inline std::vector<std::pair<int, std::function<void()>>> Callbacks;
    inline void AddKeyboardEvent(int Key, std::function<void()> Callback)
    {
        Callbacks.emplace_back(std::pair<int, std::function<void()>> { Key, Callback });
    }

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
        {
            static bool IsOn = false;

            if (!IsOn)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            IsOn = !IsOn;
        }

        for (auto&& Callback : Callbacks)
        {
            if (glfwGetKey(window, Callback.first) == GLFW_PRESS)
            {
                Callback.second();
            }
        }
    }
}
