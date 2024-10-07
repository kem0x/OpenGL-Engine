#pragma once
#include <array>
#include "PrimitiveActor.hpp"

// horrific forgive me please.
static std::vector<Vertex> CubeFrontFace()
{
    return {
        { .Position = { -0.5f, -0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } },
        { .Position = { 0.5f, -0.5f, -0.5f }, .TextureCoord = { 1.0f, 0.0f } },
        { .Position = { 0.5f, 0.5f, -0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { 0.5f, 0.5f, -0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { -0.5f, 0.5f, -0.5f }, .TextureCoord = { 0.0f, 1.0f } },
        { .Position = { -0.5f, -0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } }
    };
}

static std::vector<Vertex> CubeBackFace()
{
    return {
        { .Position = { -0.5f, -0.5f, 0.5f }, .TextureCoord = { 0.0f, 0.0f } },
        { .Position = { 0.5f, -0.5f, 0.5f }, .TextureCoord = { 1.0f, 0.0f } },
        { .Position = { 0.5f, 0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { 0.5f, 0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { -0.5f, 0.5f, 0.5f }, .TextureCoord = { 0.0f, 1.0f } },
        { .Position = { -0.5f, -0.5f, 0.5f }, .TextureCoord = { 0.0f, 0.0f } },
    };
}

static std::vector<Vertex> CubeLeftFace()
{
    return {
        { .Position = { -0.5f, -0.5f, 0.5f }, .TextureCoord = { 0.0f, 0.0f } },
        { .Position = { -0.5f, -0.5f, -0.5f }, .TextureCoord = { 1.0f, 0.0f } },
        { .Position = { -0.5f, 0.5f, -0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { -0.5f, 0.5f, -0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { -0.5f, 0.5f, 0.5f }, .TextureCoord = { 0.0f, 1.0f } },
        { .Position = { -0.5f, -0.5f, 0.5f }, .TextureCoord = { 0.0f, 0.0f } },
    };
}

static std::vector<Vertex> CubeRightFace()
{
    return {
        { .Position = { 0.5f, -0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } },
        { .Position = { 0.5f, -0.5f, 0.5f }, .TextureCoord = { 1.0f, 0.0f } },
        { .Position = { 0.5f, 0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { 0.5f, 0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { 0.5f, 0.5f, -0.5f }, .TextureCoord = { 0.0f, 1.0f } },
        { .Position = { 0.5f, -0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } },
    };
}

static std::vector<Vertex> CubeTopFace()
{
    return {
        { .Position = { -0.5f, 0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } },
        { .Position = { 0.5f, 0.5f, -0.5f }, .TextureCoord = { 1.0f, 0.0f } },
        { .Position = { 0.5f, 0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { 0.5f, 0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { -0.5f, 0.5f, 0.5f }, .TextureCoord = { 0.0f, 1.0f } },
        { .Position = { -0.5f, 0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } },
    };
}

static std::vector<Vertex> CubeBottomFace()
{
    return {
        { .Position = { -0.5f, -0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } },
        { .Position = { 0.5f, -0.5f, -0.5f }, .TextureCoord = { 1.0f, 0.0f } },
        { .Position = { 0.5f, -0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { 0.5f, -0.5f, 0.5f }, .TextureCoord = { 1.0f, 1.0f } },
        { .Position = { -0.5f, -0.5f, 0.5f }, .TextureCoord = { 0.0f, 1.0f } },
        { .Position = { -0.5f, -0.5f, -0.5f }, .TextureCoord = { 0.0f, 0.0f } }
    };
}

struct Cube : PrimitiveActor
{
    std::vector<Vertex> CubeVertices = {};

    std::vector<std::function<std::vector<Vertex>()>> FaceFunctions { CubeFrontFace, CubeBackFace, CubeLeftFace, CubeRightFace, CubeTopFace, CubeBottomFace };

    enum Faces
    {
        Bottom,
        Top,
        Front,
        Back,
        Left,
        Right,

    };

    Cube(Location InWorldLocation, std::shared_ptr<Texture2D> InTexture, std::vector<Faces> FacesToRemove = {})
    {
        for (auto i = 0; i < 6; i++)
        {
            if (std::any_of(FacesToRemove.begin(), FacesToRemove.end(), [&](Faces Face)
                    { return (Faces)i == Face; }))
            {
                continue;
            }

            auto FaceVerts = FaceFunctions[i]();

            CubeVertices.insert(CubeVertices.end(), FaceVerts.begin(), FaceVerts.end());
        }

        Vertices = CubeVertices;
        Loc = InWorldLocation;
        Texture = InTexture;

        VAO.Bind();

        VBO.Data(Vertices);
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader) override
    {
        CalculateAABB();

        __super::Draw(Shader);
    }
};