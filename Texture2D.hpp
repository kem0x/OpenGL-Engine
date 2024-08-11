#pragma once
#include <string>
#include <print>

#include "GLAD/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture2D
{
    GLuint Index = 0;
    std::string Type;

    std::string Path;

    int Width, Height;
    Texture2D(std::string FilePath, GLenum format = GL_RGB)
    {
        stbi_set_flip_vertically_on_load(true);

        // TODO: add file path checks
        int Channels;
        unsigned char* ImageData = stbi_load(FilePath.c_str(), &Width, &Height, &Channels, 0);

        if (!ImageData)
        {
            std::println("Failed to load texture! ({})", FilePath);
        }

        Path = FilePath;

        glGenTextures(1, &Index);

        this->Bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, ImageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(ImageData);
    }

    void Bind(GLenum texture = GL_TEXTURE0)
    {
        glActiveTexture(texture);

        glBindTexture(GL_TEXTURE_2D, Index);
    }

    void Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};