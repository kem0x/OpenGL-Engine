#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.hpp"
#include "Vertex.hpp"
#include "VertexBufferObject.hpp"
#include "DrawList.hpp"
#include "Math.hpp"

struct PrimitiveActor : DrawableObject
{
    std::shared_ptr<Texture2D> Texture;

    PrimitiveActor() = default;

    PrimitiveActor(std::vector<Vertex> InVertices, Location InLocation, std::shared_ptr<Texture2D> InTexture)
    {
        Vertices = InVertices;
        Loc = InLocation;
        Texture = InTexture;

        VAO.Bind();

        VBO.Data(Vertices);
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader) override
    {
        VAO.Bind();

        CalculateAABB();

        Texture->Bind();

        glm::mat4 Model = glm::translate(glm::mat4(1.0f), Loc.Vec);
        Shader->Uniform("model", Model);

        glDrawArrays(GL_TRIANGLES, 0, Vertices.size());

        Texture->Unbind();

        VAO.Unbind();
    }
};
