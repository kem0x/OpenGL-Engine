#pragma once
#include "GLAD/glad.h"

#include "GLGlobals.hpp"

#include <vector>
#include <functional>

// TODO: rework this garbage.
namespace Keyboard
{
    inline std::vector<std::function<void()>> OnKeyW;
    inline void AddWKeyEvent(std::function<void()> Fn)
    {
        OnKeyW.emplace_back(Fn);
    }

    inline std::vector<std::function<void()>> OnKeyA;
    inline void AddAKeyEvent(std::function<void()> Fn)
    {
        OnKeyA.emplace_back(Fn);
    }

    inline std::vector<std::function<void()>> OnKeyS;
    inline void AddSKeyEvent(std::function<void()> Fn)
    {
        OnKeyS.emplace_back(Fn);
    }

    inline std::vector<std::function<void()>> OnKeyD;
    inline void AddDKeyEvent(std::function<void()> Fn)
    {
        OnKeyD.emplace_back(Fn);
    }

    inline std::vector<std::function<void()>> OnKeyE;
    inline void AddEKeyEvent(std::function<void()> Fn)
    {
        OnKeyE.emplace_back(Fn);
    }

    inline std::vector<std::function<void()>> OnKeyQ;
    inline void AddQKeyEvent(std::function<void()> Fn)
    {
        OnKeyQ.emplace_back(Fn);
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

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            for (auto&& Callback : OnKeyW)
            {
                Callback();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            for (auto&& Callback : OnKeyS)
            {
                Callback();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            for (auto&& Callback : OnKeyA)
            {
                Callback();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            for (auto&& Callback : OnKeyD)
            {
                Callback();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            for (auto&& Callback : OnKeyE)
            {
                Callback();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            for (auto&& Callback : OnKeyQ)
            {
                Callback();
            }
        }
    }
}
