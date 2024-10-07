#pragma once
#include "GLAD/glad.h"
#include <vector>
#include <mutex>

struct VertexBufferObject
{
    GLuint Index = 0;

    VertexBufferObject()
    {
        glGenBuffers(1, &Index);
    }

    void Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, Index);
    }

    template <typename T>
    void Data(std::vector<T>& InData)
    {
        Bind();

        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * InData.size(), InData.data(), GL_STATIC_DRAW);
    }
};