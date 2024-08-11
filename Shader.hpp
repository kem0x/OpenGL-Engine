#pragma once
#include "GLAD/glad.h"
#include <fstream>
#include <sstream>
#include <iostream>

struct Shader
{
    GLuint Index = 0;

    Shader(GLenum type)
    {
        Index = glCreateShader(type);
    }

    void Source(std::string shaderFilePath)
    {
        // TODO: make a proper file system
        std::ifstream ShaderFile(shaderFilePath);
        std::stringstream Buffer;
        Buffer << ShaderFile.rdbuf();

        auto ShaderScript = Buffer.str();
        auto ShaderScriptC = ShaderScript.c_str();

        glShaderSource(Index, 1, &ShaderScriptC, 0);
    }

    void Compile()
    {
        glCompileShader(Index);

        int Success = 0;
        glGetShaderiv(Index, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            char InfoLog[512] { 0 };
            glGetShaderInfoLog(Index, 512, NULL, InfoLog);

            std::cout << "Shader Compilation Failed: " << InfoLog << std::endl;
        };
    }
};

