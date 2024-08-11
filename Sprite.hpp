#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture2D.hpp"

struct Sprite : DrawableObject
{
    std::shared_ptr<Texture2D> Texture;

    glm::vec2 Position;
    glm::vec2 Size;
    float Rotate;
    glm::vec3 Color;

    Sprite(std::shared_ptr<Texture2D> InTexture, glm::vec2 InPosition, glm::vec2 InSize, float InRotate, glm::vec3 InColor)
    {
        Vertices = 
        {
            { .Position = { 0.0f, 1.0f, 0.f }, .TextureCoord = { 0.0f, 1.0f } },
            { .Position = { 1.0f, 0.0f, 0.f }, .TextureCoord = { 1.0f, 0.0f } },
            { .Position = { 0.0f, 0.0f, 0.f }, .TextureCoord = { 0.0f, 0.0f } },
            { .Position = { 0.0f, 1.0f, 0.f }, .TextureCoord = { 0.0f, 1.0f } },
            { .Position = { 1.0f, 1.0f, 0.f }, .TextureCoord = { 1.0f, 1.0f } },
            { .Position = { 1.0f, 0.0f, 0.f }, .TextureCoord = { 1.0f, 0.0f } }
        };

        Texture = InTexture;
        Position = InPosition;
        Size = InSize;
        Rotate = InRotate;
        Color = InColor;
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader, VertexBufferObject VBO, ElementBufferObject EBO) override
    {
        VBO.Data(Vertices);

        Texture->Bind();

        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(Position, 0.0f));

        Model = glm::translate(Model, glm::vec3(0.5f * Size.x, 0.5f * Size.y, 0.0f));
        Model = glm::rotate(Model, glm::radians(Rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        Model = glm::translate(Model, glm::vec3(-0.5f * Size.x, -0.5f * Size.y, 0.0f));

        Model = glm::scale(Model, glm::vec3(Size, 1.0f));

        Shader->Uniform("model", Model);
        Shader->Uniform("spriteColor", Color);

        glDrawArrays(GL_TRIANGLES, 0, Vertices.size());

        Texture->Unbind();
    }
};