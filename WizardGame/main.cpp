#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "SkeletalAnimation/Types.h"
#include "SkeletalAnimation/SkeletalModelSerializer.h"
#include "SkeletalAnimation/SkeletalModel.h"
#include "AssimpConverter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "object.h"
#include "world.h"
#define WIDTH 800
#define HEIGHT 600
#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 1
#include "luaLibrary.h"
#include "GraphicsComponent.h"
#include "NpcComponent.h"
#include "Component.h"
#include "postProcessing.h"
#include <qu3e/q3.h>
#include "FrameBuffer.h"
#include "imgui_sdl.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
using namespace std;
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#undef main

int main()
{

    bool Left = false;
    bool Right = false;
    // Vertex vertices[] = {Vertex(glm::vec3(-0.5f,-0.5f,0), glm::vec2(0.0,0.0)),
    //                      Vertex(glm::vec3(0.0f,0.5f,0),      glm::vec2(0.5,1.0)),
    //                      Vertex(glm::vec3(0.5f,-0.5f,0),  glm::vec2(1.0,0.0))
    //                     };

    unsigned int indices[] = {0,1,2};
    Display display(WIDTH,HEIGHT, "Wizard Game!");
    float counter = 0.0f;
    World world;
    FrameBuffer postProcessing;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplSDL2_InitForOpenGL(display.m_window, display.m_glContext);
    const char* glsl_version = "#version 130";

    ImGui_ImplOpenGL3_Init(glsl_version);


    // std::cout << e->getType() << "X:" << pos.x << " Y:" << pos.y << " Z:" << pos.z << '\n';
    while(!display.isClosed()) {
        postProcessing.BindFrameBuffer();
        display.Clear(0.294f,0.463f,0.639f,1.0f);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(display.m_window);

        ImGui::NewFrame();
        world.Update();

        //ImGui::ShowDemoWindow();
        world.Draw();

        //pp.Bind();




        // gfxc->Update(world.deltaTime);
        // gfxc->Draw(world.player.GetCamera());
        counter+=0.01f;
        //pp.Render();
        display.Update();
        postProcessing.RenderFrameBuffer();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
