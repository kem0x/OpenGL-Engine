#pragma once
#include <vector>
#include "Vertex.hpp"
#include "Texture2D.hpp"
#include "ShaderProgram.hpp"
#include "Math.hpp"
#include "DrawList.hpp"

struct Mesh : DrawableObject
{
    std::vector<unsigned int> Indices;
    std::vector<std::shared_ptr<Texture2D>> Textures;

    Mesh(std::vector<Vertex> InVertices,
        std::vector<unsigned int> InIndices,
        std::vector<std::shared_ptr<Texture2D>> InTextures, Location InLoc)
    {
        Loc = InLoc;
        Vertices = InVertices;
        Indices = InIndices;
        Textures = InTextures;

        VAO.Bind();

        VBO.Data(Vertices);
        EBO.Data(Indices);
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader) override
    {
        VAO.Bind();

        CalculateAABB();

        unsigned int DiffuseNr = 1;
        unsigned int SpecularNr = 1;
        for (int i = 0; i < Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = Textures[i]->Type;

            if (name == "texture_diffuse")
            {
                number = std::to_string(DiffuseNr++);
            }
            else if (name == "texture_specular")
            {
                number = std::to_string(SpecularNr++);
            }

            Shader->Uniform((name + number).c_str(), i);

            glBindTexture(GL_TEXTURE_2D, Textures[i]->Index);
        }

        glActiveTexture(GL_TEXTURE0);

        glm::mat4 Model = glm::translate(glm::mat4(1.0f), Loc.Vec);
        Shader->Uniform("model", Model);

        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        
        VAO.Unbind();
    }
};