#pragma once
#include "PerlinNoise.hpp"
#include <memory>

struct ChunkSystem
{
    PerlinNoise perlin { 123456u };

    void GenerateTerrainChunk()
    {
        auto GrassTex = std ::make_shared<Texture2D>("E:\\source\\repos\\TheCube\\Textures\\grass.png", GL_RGBA);
        auto BrickTex = std ::make_shared<Texture2D>("E:\\source\\repos\\TheCube\\Textures\\brick.png", GL_RGBA);

        constexpr int ChunkSize = 32;

        constexpr int BlockSize = 100;

        constexpr float HeightScale = 8.0f;

        constexpr float Scale = 0.1f;

        for (int z = 0; z < ChunkSize; ++z)
        {
            for (int y = 0; y < ChunkSize; ++y)
            {
                for (int x = 0; x < ChunkSize; ++x)
                {
                    // TODO: make it that after a random surface thickness (rand~1-8) the chunk is filled fully

                    const float Noise = perlin.noise(x * Scale, y * Scale, z * Scale);
                    const float Height = std::round(Noise * HeightScale);

                    const float BlockX = x * BlockSize;
                    const float BlockY = y * BlockSize;
                    const float BlockZ = z * BlockSize + Height * BlockSize;

                    static bool IsOn = false;

                    if (!IsOn)
                    {
                        std::vector<Cube::Faces> Faces = { Cube::Faces::Front };
                        auto D = std::make_shared<Cube>(Location::FromWorldCoords({ BlockX, BlockY, BlockZ }), BrickTex, Faces);
                        D->Push();
                    }
                    else
                    {
                        std::vector<Cube::Faces> Faces = { Cube::Faces::Left };
                        auto D = std::make_shared<Cube>(Location::FromWorldCoords({ BlockX, BlockY, BlockZ }), GrassTex, Faces);
                        D->Push();
                    }

                    IsOn = !IsOn;
                }
            }
        }
    }
};