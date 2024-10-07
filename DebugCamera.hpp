#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Keyboard.hpp"
#include "Mouse.hpp"

struct Camera
{
    glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 CameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 CameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

    float Pitch = 0.0f;
    float Yaw = 0.0f;

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::mat4(1.0f);
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    virtual void Update() = 0;
};

struct DebugCamera : Camera
{
    virtual void Update() override
    {
        View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
    }

    void SetupInput()
    {
        auto CameraSpeed = []()
        {
            return 6.5f * DeltaTime;
        };

        Mouse::AddMouseEvent(
            [&](float XOffset, float YOffset, float LastX, float LastY)
            {
                float Sensitivity = 0.1f;
                XOffset *= Sensitivity;
                YOffset *= Sensitivity;

                Yaw -= XOffset;
                Pitch += YOffset;

                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;

                glm::vec3 Direction {};
                Direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
                Direction.y = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
                Direction.z = sin(glm::radians(Pitch)); // Since the positive Z-axis is up
                CameraFront = glm::normalize(Direction);
            });

        Keyboard::AddKeyboardEvent(
            GLFW_KEY_W,
            [&]()
            {
                CameraPos += CameraSpeed() * CameraFront;
            });

        Keyboard::AddKeyboardEvent(
            GLFW_KEY_S,
            [&]()
            {
                CameraPos -= CameraSpeed() * CameraFront;
            });

        Keyboard::AddKeyboardEvent(
            GLFW_KEY_A,
            [&]()
            {
                CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed();
            });

        Keyboard::AddKeyboardEvent(
            GLFW_KEY_D,
            [&]()
            {
                CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed();
            });

        Keyboard::AddKeyboardEvent(
            GLFW_KEY_E,
            [&]()
            {
                CameraPos.z += CameraSpeed();
            });

        Keyboard::AddKeyboardEvent(
            GLFW_KEY_Q,
            [&]()
            {
                CameraPos.z -= CameraSpeed();
            });
    }
};