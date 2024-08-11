#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DrawList.hpp"
#include "Mesh.hpp"

struct Model : DrawableObject
{
    ShaderProgram Program;

    Location Loc;

    std::string Path;
    std::vector<Texture2D> LoadedTextures;
    std::vector<Mesh> Meshes;

    Model(std::string ModelPath, Location InLoc)
    {
        Loc = InLoc;
        Path = ModelPath;
        LoadModel(ModelPath);
    }

    std::vector<Texture2D> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, std::string TypeName)
    {
        auto Directory = Path.substr(0, Path.find_last_of('\\'));

        std::vector<Texture2D> Textures;
        for (unsigned int i = 0; i < Mat->GetTextureCount(Type); i++)
        {
            aiString Str;
            Mat->GetTexture(Type, i, &Str);

            bool Skip = false;
            for (unsigned int j = 0; j < LoadedTextures.size(); j++)
            {
                if (LoadedTextures[j].Path.ends_with(Str.C_Str()))
                {
                    Textures.emplace_back(LoadedTextures[j]);
                    Skip = true;
                    break;
                }
            }

            if (!Skip)
            {
                auto Texture = Texture2D(Directory + '/' + Str.C_Str());
                Texture.Type = TypeName;
                Textures.emplace_back(Texture);
                LoadedTextures.emplace_back(Texture);
            }
        }
        return Textures;
    }

    void LoadModel(std::string FilePath)
    {
        Assimp::Importer Import;
        const aiScene* Scene = Import.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << Import.GetErrorString() << std::endl;
            return;
        }

        ProcessNode(Scene->mRootNode, Scene);
    }

    void ProcessNode(aiNode* Node, const aiScene* Scene)
    {
        for (unsigned int i = 0; i < Node->mNumMeshes; i++)
        {
            aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
            Meshes.push_back(ProcessMesh(Mesh, Scene));
        }

        for (unsigned int i = 0; i < Node->mNumChildren; i++)
        {
            ProcessNode(Node->mChildren[i], Scene);
        }
    }

    Mesh ProcessMesh(aiMesh* InMesh, const aiScene* InScene)
    {
        std::vector<unsigned int> Indices;
        std::vector<Texture2D> Textures;

        for (unsigned int i = 0; i < InMesh->mNumVertices; i++)
        {
            Vertex Vertex {};

            Vertex.Position = { InMesh->mVertices[i].x,
                InMesh->mVertices[i].y,
                InMesh->mVertices[i].z };

            if (InMesh->HasNormals())
            {
                Vertex.Normal = { InMesh->mNormals[i].x,
                    InMesh->mNormals[i].y,
                    InMesh->mNormals[i].z };
            }

            if (InMesh->mTextureCoords[0])
            {
                Vertex.TextureCoord = { InMesh->mTextureCoords[0][i].x, InMesh->mTextureCoords[0][i].y };

                if (InMesh->mTangents)
                {
                    Vertex.Tangent = { InMesh->mTangents[i].x, InMesh->mTangents[i].y, InMesh->mTangents[i].z };
                }

                if (InMesh->mBitangents)
                {
                    Vertex.Bitangent = { InMesh->mBitangents[i].x, InMesh->mBitangents[i].y, InMesh->mBitangents[i].z };
                }
            }
            else
            {
                Vertex.TextureCoord = glm::vec2(0.0f, 0.0f);
            }

            Vertices.emplace_back(Vertex);
        }

        for (unsigned int i = 0; i < InMesh->mNumFaces; i++)
        {
            aiFace Face = InMesh->mFaces[i];
            for (unsigned int j = 0; j < Face.mNumIndices; j++)
            {
                Indices.push_back(Face.mIndices[j]);
            }
        }

        aiMaterial* Material = InScene->mMaterials[InMesh->mMaterialIndex];

        // 1. diffuse maps
        std::vector<Texture2D> DiffuseMaps = LoadMaterialTextures(Material, aiTextureType_DIFFUSE, "texture_diffuse");
        Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

        // 2. specular maps
        std::vector<Texture2D> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR, "texture_specular");
        Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());

        // 3. normal maps
        std::vector<Texture2D> NormalMaps = LoadMaterialTextures(Material, aiTextureType_HEIGHT, "texture_normal");
        Textures.insert(Textures.end(), NormalMaps.begin(), NormalMaps.end());

        // 4. height maps
        std::vector<Texture2D> HeightMaps = LoadMaterialTextures(Material, aiTextureType_AMBIENT, "texture_height");
        Textures.insert(Textures.end(), HeightMaps.begin(), HeightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(Vertices, Indices, Textures, Loc);
    }

    void SetShaderProgram(ShaderProgram InProgram)
    {
        Program = InProgram;
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader, VertexBufferObject VBO, ElementBufferObject EBO) override
    {
        AABB ModelAABB = Meshes[0].BoundingBox;

        for (auto&& Mesh : Meshes)
        {
            ModelAABB.Min = glm::min(ModelAABB.Min, Mesh.BoundingBox.Min);
            ModelAABB.Max = glm::max(ModelAABB.Max, Mesh.BoundingBox.Max);

            BoundingBox = ModelAABB;

            Mesh.Draw(Shader, VBO, EBO);
        }
    }
};