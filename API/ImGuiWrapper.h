#pragma once

typedef union SDL_Event SDL_Event;

namespace Framework { class Window; }

namespace Framework::ImGuiWrapper
{
    void Initialize(const Window& window);
    void DeInitialize();
    void Update(const Window& window);
    void Render();
    void ProcessEvent(SDL_Event* event);
}