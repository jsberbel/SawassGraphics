#pragma once

struct SDL_Window;

namespace Framework
{
    class Config;

    class Window
    {
        NON_COPYABLE_CLASS(Window);
        friend class GraphicsManager;

    public:
        SDL_Window*        GetHandle() const;
        const std::string& GetTitle()  const;
        u32                GetWidth()  const;
        u32                GetHeight() const;

    private:
        Window();
        ~Window() = default;

        void Initialize(const Config& config);
        void DeInitialize();

        void Show() const;
        void SwapBuffers() const;
        void SetTitle(const std::string& title);
        void Resize(u32 width, u32 height, bool recompute = true);

    private:
        SDL_Window* m_Handle;
        std::string m_Title;
        u32         m_Width;
        u32         m_Height;
    };
}