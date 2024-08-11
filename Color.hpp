#pragma once
#include "GLAD/glad.h"

struct Color
{
    GLfloat R;
    GLfloat G;
    GLfloat B;
    GLfloat A;

    static Color FromInt(int r, int g, int b, int a)
    {
        return { r / 255.0F, g / 255.0F, b / 255.0F, a / 255.0F };
    }

    static Color FromFloat(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        return { r, g, b, a };
    }
};