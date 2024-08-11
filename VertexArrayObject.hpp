#pragma once
#include "GLAD/glad.h"

struct VertexArrayObject
{
    GLuint Index = 0;

    VertexArrayObject()
    {
        glGenVertexArrays(1, &Index);
    }

    void Bind()
    {
        glBindVertexArray(Index);
    }
};