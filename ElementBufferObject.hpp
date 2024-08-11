#pragma once
#include "GLAD/glad.h"
#include <vector>

struct ElementBufferObject
{
    GLuint Index = 0;

    ElementBufferObject()
    {
        glGenBuffers(1, &Index);
    }

    void Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index);
    }

    template <typename T>
    void Data(std::vector<T> InData)
    {
        Bind();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * InData.size(), InData.data(), GL_STATIC_DRAW);
    }
};