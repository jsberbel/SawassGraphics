/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#include "Graphic/Content/Surface.h"
#include "Graphic/Content/Font.h"
#include "Graphic/Util/RGBAColor.h"

#include "Core/Debug.h"

#include "SDL_ttf.h"
#include "SDL_image.h"

namespace Framework
{
    Surface::Surface()
        : m_Handle(nullptr)
    {
    }

    Surface::Surface(const std::string& filePath)
        : Surface()
    {
        LoadFromFile(filePath);
    }

    Surface::Surface(const Font& font, const std::string& text, const RGBAColor& color, u32 wrapLength)
        : Surface()
    {
        LoadFromText(font, text, color, wrapLength);
    }

    Surface::Surface(const Surface& other)
    {
        m_Handle = SDL_CreateRGBSurface(0, other.GetWidth(), other.GetHeight(), other.GetBitsPerPixel(), 0, 0, 0, 0);
        SDL_BlitSurface(other.m_Handle, nullptr, m_Handle, nullptr);
    }

    Surface& Surface::operator=(const Surface& other)
    {
        Unload();
        m_Handle = SDL_CreateRGBSurface(0, other.GetWidth(), other.GetHeight(), other.GetBitsPerPixel(), 0, 0, 0, 0);
        SDL_BlitSurface(other.m_Handle, nullptr, m_Handle, nullptr);
        return *this;
    }

    Surface::Surface(Surface&& other) noexcept
        : m_Handle(other.m_Handle)
    {
        other.m_Handle = nullptr;
    }

    Surface& Surface::operator=(Surface&& other) noexcept
    {
        Unload();
        m_Handle = other.m_Handle;
        other.m_Handle = nullptr;
        return *this;
    }

    Surface::~Surface()
    {
        Unload();
    }

    void Surface::LoadFromFile(const std::string& filePath)
    {
        Unload();

        m_Handle = IMG_Load( filePath.c_str() );
        ASSERT_OR_ALERT( IsLoaded(), "Unable to load image from file %s", filePath.c_str() );
    }

    void Surface::LoadFromText(const Font& font, const std::string& text, const RGBAColor& color, u32 wrapLength)
    {
        Unload();

        m_Handle = TTF_RenderText_Blended_Wrapped( const_cast<TTF_Font*>(font.GetInternalFont()), text.c_str(), color.AsSDLColor(), wrapLength );
        ASSERT_OR_ALERT( IsLoaded(), "Unable to load surface with font %s and text: %s", font.GetFileName().c_str(), text.c_str() );
    }

    void Surface::Unload()
    {
        if (IsLoaded())
        {
            SDL_FreeSurface(m_Handle);
            m_Handle = nullptr;
        }
    }

#ifdef _DEBUG
    void Surface::SaveToFileAsPNG(const std::string& filePath) const
    {
        ASSERT(IsLoaded() && !filePath.empty());

        const s32 result = IMG_SavePNG(m_Handle, filePath.c_str());
        ASSERT_OR_ALERT(result != -1, "Unable to save surface to %s. SDL Error: %s", filePath.c_str(), SDL_GetError());
    }
#endif

    u32 Surface::GetWidth() const
    {
        ASSERT(IsLoaded());
        return m_Handle->w;
    }

    u32 Surface::GetHeight() const
    {
        ASSERT(IsLoaded());
        return m_Handle->h;
    }

    u32 Surface::GetBitsPerPixel() const
    {
        ASSERT(IsLoaded());
        return m_Handle->format->BitsPerPixel;
    }

    u32 Surface::GetBytesPerPixel() const
    {
        ASSERT(IsLoaded());
        return m_Handle->format->BytesPerPixel;
    }

    std::string Surface::GetPixelFormatName() const
    {
        ASSERT(IsLoaded());
        return SDL_GetPixelFormatName(m_Handle->format->format );
    }

    const void* Surface::GetPixelData() const
    {
        ASSERT(IsLoaded());
        return m_Handle->pixels;
    }

}