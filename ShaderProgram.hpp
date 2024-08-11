#pragma once
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLAD/glad.h"
#include "Shader.hpp"

struct ShaderProgram
{
    GLuint Index = 0;

    ShaderProgram()
    {
        Index = glCreateProgram();
    }

    void Attach(Shader shader)
    {
        glAttachShader(Index, shader.Index);
    }

    void Link()
    {
        glLinkProgram(Index);
    }

    template <typename T, auto F, auto S>
    void Location(const GLchar* name)
    {
        const GLint IdxLocation = glGetAttribLocation(Index, name);
        glEnableVertexAttribArray(IdxLocation);

        glVertexAttribPointer(
            IdxLocation,
            S,
            GL_FLOAT,
            GL_FALSE,
            sizeof(T),
            (void*)F);
    }

    template <typename... Args>
    void Uniform(const GLchar* name, Args... args)
    {
        using FirstType = std::tuple_element_t<0, std::tuple<Args...>>;

        int UniformLocation = glGetUniformLocation(Index, name);

        const std::size_t SizeOfArgs = sizeof...(Args);

        if constexpr (std::is_same_v<FirstType, glm::mat4>)
        {
            return glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(args...));
        }
        else if constexpr (std::is_same_v<FirstType, glm::vec3>)
        {
            return glUniform3fv(UniformLocation, 1, glm::value_ptr(args...));
        }
        else if constexpr (std::is_same_v<FirstType, int>)
        {
            if constexpr (SizeOfArgs == 1)
            {
                return glUniform1i(UniformLocation, args...);
            }
            else if constexpr (SizeOfArgs == 2)
            {
                return glUniform2i(UniformLocation, args...);
            }
            else if constexpr (SizeOfArgs == 3)
            {
                return glUniform3i(UniformLocation, args...);
            }
            else if constexpr (SizeOfArgs == 4)
            {
                return glUniform4i(UniformLocation, args...);
            }
        }
        else if constexpr (std::is_same_v<FirstType, float>)
        {
            if constexpr (SizeOfArgs == 1)
            {
                return glUniform1f(UniformLocation, args...);
            }
            else if constexpr (SizeOfArgs == 2)
            {
                return glUniform2f(UniformLocation, args...);
            }
            else if constexpr (SizeOfArgs == 3)
            {
                return glUniform3f(UniformLocation, args...);
            }
            else if constexpr (SizeOfArgs == 4)
            {
                return glUniform4f(UniformLocation, args...);
            }
        }
    }

    void Use()
    {
        glUseProgram(Index);
    }
};