// ImGui SDL2 binding with OpenGL
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#include "Precompiled.h"

#include "ImGuiWrapper.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Engine/Engine.h"

#include <glad/glad.h>
#include <SDL_opengl.h>
#include <SDL_syswm.h>

#include "ImGui/examples/imgui_impl_sdl.h"
#include "ImGui/examples/imgui_impl_opengl3.h"
//#include "ImGui/examples/imgui_impl_vulkan.h" // TODO(jserrano)

namespace Framework::ImGuiWrapper
{
    void Initialize(const Window& window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplSDL2_InitForOpenGL(window.GetHandle(), SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init();

        ImGui::StyleColorsDark();
    }

    void DeInitialize()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void Update(const Window& window)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.GetHandle());
        ImGui::NewFrame();
    }

    void Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ProcessEvent(SDL_Event* event)
    {
        ImGui_ImplSDL2_ProcessEvent(event);
    }
}