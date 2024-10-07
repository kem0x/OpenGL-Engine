#pragma once
#include <filesystem>
#include "DebugCamera.hpp"

namespace LuaRuntime
{
    inline std::vector<std::shared_ptr<sol::state>> GLuaStates;

    static auto CollectAllDebugFunctions()
    {
        std::vector<sol::function> DebugFunctions;

        for (auto&& LuaState : LuaRuntime::GLuaStates)
        {
            auto Fn = LuaState->operator[]("DebugUI");
            if (Fn)
            {
                DebugFunctions.push_back(Fn);
            }
        }

        return DebugFunctions;
    }

    static auto CollectAllRenderFunctions()
    {
        std::vector<sol::function> RenderFunctions;

        for (auto&& LuaState : LuaRuntime::GLuaStates)
        {
            auto Fn = LuaState->operator[]("Render");
            if (Fn)
            {
                RenderFunctions.push_back(Fn);
            }
        }

        return RenderFunctions;
    }

    static void InitLuaStateImGui(std::shared_ptr<sol::state> LuaState)
    {
        sol::table ImGui = LuaState->create_named_table("ImGui");

        ImGui.set_function("Button",
            [](const char* label) -> bool
            {
                return ImGui::Button(label);
            });

        ImGui.set_function("Text", &ImGui::Text);
    }

    static void InitLuaState(std::shared_ptr<sol::state> LuaState)
    {
        LuaState->open_libraries();

        auto Vec2Type = LuaState->new_usertype<glm::vec2>("Vec2",
            "x", &glm::vec2::x,
            "y", &glm::vec2::y);
        Vec2Type["Create"] = [](float X, float Y)
        {
            return glm::vec2(X, Y);
        };

        auto Vec3Type = LuaState->new_usertype<glm::vec3>("Vec3",
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z);
        Vec3Type["Create"] = [](float X, float Y, float Z)
        {
            return glm::vec3(X, Y, Z);
        };

        auto VAOType = LuaState->new_usertype<VertexArrayObject>("VertexArrayObject",
            "Bind",
            &VertexArrayObject::Bind);

        VAOType["Create"] = []()
        {
            return std::make_shared<VertexArrayObject>();
        };

        auto VBOType = LuaState->new_usertype<VertexBufferObject>("VertexBufferObject",
            "Bind",
            &VertexBufferObject::Bind);

        VBOType["Create"] = []()
        {
            return std::make_shared<VertexBufferObject>();
        };

        auto EBOType = LuaState->new_usertype<ElementBufferObject>("ElementBufferObject",
            "Bind",
            &ElementBufferObject::Bind);

        EBOType["Create"] = []()
        {
            return std::make_shared<ElementBufferObject>();
        };

        auto ShaderProgramType = LuaState->new_usertype<ShaderProgram>("ShaderProgram",
            "Attach",
            &ShaderProgram::Attach,
            "Link",
            &ShaderProgram::Link,
            "Use",
            &ShaderProgram::Use,
            "UniformInt",
            &ShaderProgram::Uniform<int>,
            "UniformMat4",
            &ShaderProgram::Uniform<glm::mat4>);

        ShaderProgramType["Create"] = []()
        {
            return std::make_shared<ShaderProgram>();
        };

        ShaderProgramType["VertexLocations"] = [](ShaderProgram SP)
        {
            SP.Location<Vertex, offsetof(Vertex, Position), 3>("aPos");
            SP.Location<Vertex, offsetof(Vertex, TextureCoord), 2>("aTexCoord");
        };

        LuaState->new_usertype<Shader>("Shader",
            sol::constructors<Shader(GLenum)>(),
            "Source",
            &Shader::Source,
            "Compile",
            &Shader::Compile);

        auto LocationType = LuaState->new_usertype<Location>("Location");
        LocationType["FromWorldCoords"] = [](int X, int Y, int Z)
        {
            return Location::FromWorldCoords({ X, Y, Z });
        };

        LuaState->new_usertype<DebugCamera>("DebugCamera",
            sol::constructors<DebugCamera()>(),
            "CameraPos",
            &DebugCamera::CameraPos,
            "CameraFront",
            &DebugCamera::CameraFront,
            "Projection",
            &DebugCamera::Projection,
            "View",
            &DebugCamera::View,
            "SetupInput",
            &DebugCamera::SetupInput,
            "Update",
            &DebugCamera::Update);

        LuaState->new_usertype<Drawlist>("Drawlist",
            "Draw",
            &Drawlist::Draw);

        auto Texture2DType = LuaState->new_usertype<Texture2D>("Texture2D");
        Texture2DType["Create"] = [](std::string FilePath, GLenum Format)
        {
            return std::make_shared<Texture2D>(FilePath, Format);
        };

        auto SpriteType = LuaState->new_usertype<Sprite>("Sprite",
            "Push", &Sprite::Push);
        SpriteType["Create"] = [](std::shared_ptr<Texture2D> InTexture, glm::vec2 InPosition, glm::vec2 InSize, float InRotate, glm::vec3 InColor)
        {
            return std::make_shared<Sprite>(InTexture, InPosition, InSize, InRotate, InColor);
        };

        auto CubeType = LuaState->new_usertype<Cube>("Cube", "Push", &Cube::Push);
        CubeType["Create"] = [](Location InWorldLocation, std::shared_ptr<Texture2D> InTexture)
        {
            return std::make_shared<Cube>(InWorldLocation, InTexture);
        };

        auto ModelType = LuaState->new_usertype<Model>("Model", "Push", &Model::Push);
        ModelType["Create"] = [](std::string Path, Location Loc)
        {
            return std::make_shared<Model>(Path, Loc);
        };

        // FIXME
        LuaState->operator[]("MakeObjectHitscannable") = [](std::shared_ptr<struct DrawableObject> Object)
        {
            HitScanTargets.emplace_back(Object);
        };

        LuaState->operator[]("Projection2D") = glm::ortho(0.0f, static_cast<float>(1280), static_cast<float>(720), 0.0f, -1.0f, 1.0f);
    }

    static void Init()
    {
        for (const auto& Entry : std::filesystem::directory_iterator(std::filesystem::current_path() / "Scripts"))
        {
            if (Entry.path().extension() != ".lua")
            {
                continue;
            }

            std::println("Loading {} state", Entry.path().filename().string());

            auto LuaState = std::make_shared<sol::state>();
            InitLuaState(LuaState);
            InitLuaStateImGui(LuaState);

            LuaState->script_file(Entry.path().string());

            if (!LuaState->operator[]("Render"))
            {
                std::println("File doesn't contain a Render function, ignoring...");
                continue;
            }

            GLuaStates.emplace_back(LuaState);
        }
    }

}