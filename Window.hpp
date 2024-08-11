#pragma once
#include "GLFW/glfw3.h"
#include "GLAD/glad.h"

#include "Keyboard.hpp"
#include "Mouse.hpp"

#include <cstdlib>
#include <cstdio>
#include <utility>

struct Window
{
    static void ErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    GLFWwindow* window = nullptr;

    Window()
    {
        glfwSetErrorCallback(ErrorCallback);

        if (!glfwInit())
        {
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(1280, 720, "TheCube", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        gladLoadGL();
        glfwSwapInterval(1);

        glEnable(GL_DEPTH_TEST);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetKeyCallback(window, Keyboard::KeyCallback);

        glfwSetCursorPosCallback(window, Mouse::MouseCallback);
    }

    bool ShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    std::pair<int, int> GetSize()
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        return { width, height };
    }

    void SwapBuffer()
    {
        glfwSwapBuffers(window);
    }

    ~Window()
    {
        glfwDestroyWindow(window);

        glfwTerminate();
    }
};