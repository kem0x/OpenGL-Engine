#define GLAD_GL_IMPLEMENTATION
#include "GLAD/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#define GLT_IMPLEMENTATION
#include "gltext.h"

#define SOL_LUAJIT 1
#define SOL_PRINT_ERRORS 1
#include "SOL/sol.hpp"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "VertexBufferObject.hpp"
#include "Texture2D.hpp"
#include "ElementBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "GLGlobals.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "PrimitiveActor.hpp"
#include "Cube.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "Math.hpp"
#include "ChunkSystem.hpp"
#include "LuaRuntime.hpp"

#pragma comment(lib, "GLFW/glfw3_mt.lib")

int main(int argc, char** argv)
{
    auto MainWindow = Window();

    gltInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(MainWindow.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glfwSetInputMode(MainWindow.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    LuaRuntime::Init();

    auto LuaRenderFunctions = LuaRuntime::CollectAllRenderFunctions();
    auto LuaDebugUIFunctions = LuaRuntime::CollectAllDebugFunctions();

    // auto HitScanText = std::make_shared<Text>(0, 35, "", Color::FromInt(136, 137, 150, 255));
    // HitScanText->Push();

    // ChunkSystem CS;
    // CS.GenerateTerrainChunk();

    /* auto FragmentShader = Shader(GL_FRAGMENT_SHADER);
    FragmentShader.Source("E:\\source\\repos\\TheCube\\Shaders\\shader3d.frag");
    FragmentShader.Compile();

    auto VertexShader = Shader(GL_VERTEX_SHADER);
    VertexShader.Source("E:\\source\\repos\\TheCube\\Shaders\\shader3d.vert");
    VertexShader.Compile();

    auto ShaderProg = std::make_shared<ShaderProgram>();
    ShaderProg->Attach(VertexShader);
    ShaderProg->Attach(FragmentShader);
    ShaderProg->Link();

    ShaderProg->Use();

    auto DefaultCamera = DebugCamera();
    DefaultCamera.SetupInput();

    auto GrassTexture = std::make_shared<Texture2D>("E:\\source\\repos\\TheCube\\Textures\\grass.png", 6408);

    auto GrassBlock = std::make_shared<Cube>(Location::FromWorldCoords({ 200, 200, 200 }), GrassTexture);
    GrassBlock->Push();

    ShaderProg->Location<Vertex, offsetof(Vertex, Position), 3>("aPos");
    ShaderProg->Location<Vertex, offsetof(Vertex, TextureCoord), 2>("aTexCoord");
    */

    while (!MainWindow.ShouldClose())
    {
        if (glfwGetWindowAttrib(MainWindow.window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        glfwPollEvents();

        UpdateDeltaTime();

        auto WindowSize = MainWindow.GetSize();

        glViewport(0, 0, WindowSize.first, WindowSize.second);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* 
        ShaderProg->Use();

        DefaultCamera.Update();

        ShaderProg->Uniform("projection", DefaultCamera.Projection);
        ShaderProg->Uniform("view", DefaultCamera.View);

        Drawlist::Draw(ShaderProg);
        */

        for (auto&& Render : LuaRenderFunctions)
        {
            Render();
        }

        /*

        auto HitTargets = CheckHitScanTargets(DefaultCamera.CameraPos, DefaultCamera.CameraFront);
        if (!HitTargets.empty())
        {
            HitScanText->SetText("Aiming at a target");
        }
        else
        {
            HitScanText->SetText("NOT Aiming at a target");
        }*/

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Begin("Debug UI");

        for (auto&& DebugUI : LuaDebugUIFunctions)
        {
            DebugUI();
            ImGui::Separator();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        MainWindow.SwapBuffer();
    }
}