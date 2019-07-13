/*
 * Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#include "Graphic/Window.h"
#include "Core/Config.h"

#include <SDL.h>

namespace Framework
{
    Window::Window()
        : m_Handle(nullptr)
        , m_Title()
        , m_Width(0)
        , m_Height(0)
    {
    }

    void Window::Initialize(const Config& config)
    {
#ifdef OPENGL_GRAPHICS
        const u32 windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
#elif VULKAN_GRAPHICS
        const u32 windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN;
#else
        #error Graphics API not defined for SDL Window initialization
#endif

        m_Title = config.GetGameName();

        if (config.GetFullScreen())
        {
            m_Handle = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, windowFlags | SDL_WINDOW_FULLSCREEN);
            SDL_GetWindowSize(m_Handle, reinterpret_cast<int*>(&m_Width), reinterpret_cast<int*>(&m_Height));
        }
        else
        {
            m_Width = config.GetResolutionWidth();
            m_Height = config.GetResolutionHeight();
            m_Handle = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, windowFlags);
        }

        ASSERT(m_Handle != nullptr);
    }

    void Window::DeInitialize()
    {
        ASSERT(m_Handle != nullptr);

        SDL_DestroyWindow(m_Handle);
        m_Handle = nullptr;
    }

    void Window::Show() const
    {
        SDL_ShowWindow(m_Handle);
    }

    void Window::SwapBuffers() const
    {
        SDL_GL_SwapWindow(m_Handle);
    }

    void Window::SetTitle(const std::string& title)
    {
        m_Title = title;
        SDL_SetWindowTitle(m_Handle, m_Title.c_str());
    }

    void Window::Resize(u32 width, u32 height, bool recompute /*= true*/)
    {
        ASSERT(width > 0 && height > 0);
        m_Width = width;
        m_Height = height;

        if (recompute)
            SDL_SetWindowSize(m_Handle, m_Width, m_Height);
    }

    SDL_Window* Window::GetHandle() const
    {
        ASSERT(m_Handle != nullptr);
        return m_Handle;
    }

    const std::string& Window::GetTitle() const
    {
        ASSERT(m_Handle != nullptr);
        return m_Title;
    }

    u32 Window::GetWidth() const
    {
        ASSERT(m_Width);
        return m_Width;
    }

    u32 Window::GetHeight() const
    {
        ASSERT(m_Height);
        return m_Height;
    }
}