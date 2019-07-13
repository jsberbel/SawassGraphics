/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#include "SDL.h"

#include "Graphic/Content/Surface.h"
#include "Graphic/Content/Texture.h"
#include "Engine/Engine.h"

namespace Framework
{
    Texture::Texture()
        : m_ID          (0u)
        , m_Width       (0)
        , m_Height      (0)
        , m_PixelFormat (PixelFormat::Red)
    #ifdef _DEBUG
        , m_Name        ("Undefined")
    #endif
    {
    }

    Texture::Texture(const Surface& surface)
        : m_ID          (0u)
        , m_Width       (0)
        , m_Height      (0)
        , m_PixelFormat (PixelFormat::Red)
    #ifdef _DEBUG
        , m_Name        ("Undefined")
    #endif
    {
        LoadFromSurface(surface);
    }

    Texture::Texture(Surface&& surface)
        : m_ID          (0u)
        , m_Width       (0)
        , m_Height      (0)
        , m_PixelFormat (PixelFormat::Red)
    #ifdef _DEBUG
        , m_Name        ("Undefined")
    #endif
    {
        LoadFromSurface( std::move(surface) );
    }

    Texture::~Texture()
    {
        Unload();
    }

    void Texture::LoadFromSurface(const Surface& surface)
    {
        ASSERT( surface.IsLoaded() );

        Unload();

        m_Surface = surface;
        LoadInternal();
    }

    void Texture::LoadFromSurface(Surface&& surface)
    {
        ASSERT( surface.IsLoaded() );

        Unload();

        m_Surface = std::move(surface);
        LoadInternal();
    }

    void Texture::Unload()
    {
        if ( IsLoaded() )
        {
            UnloadInternal();

            m_ID     = 0u;
            m_Width  = 0u;
            m_Height = 0u;
            m_PixelFormat = PixelFormat::Red;
            m_Surface.Unload();
        }
    }

    //----------------------------------------------------------------

    TextureFile::TextureFile(const std::string& filePath)
        : Resource(filePath)
        , Texture( Surface(filePath) )
    {
        DEBUG_EXP( m_Name = GetFileName(); )
    }

#ifdef _DEBUG
    float TextureFile::GetUsedVRam() const
    {
        const int bytesPerPixel = m_Surface.GetBytesPerPixel();
        const int numBytes = m_Width * m_Height * bytesPerPixel;
        return static_cast<float>(numBytes) / 1024.0f;
    }
#endif
}
