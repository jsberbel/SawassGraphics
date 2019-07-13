#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include "Graphic/Content/Texture.h"
#include "Graphic/Content/Surface.h"
#include "Core/Debug.h"

#include <glad/glad.h>

namespace Framework
{
    constexpr PixelFormat GetPixelFormatFromTextureMode(u32 openGLPixelFormat)
    {
        switch (openGLPixelFormat)
        {
            case GL_RED             : return PixelFormat::Red               ;
            case GL_RG              : return PixelFormat::RG                ;
            case GL_RGB             : return PixelFormat::RGB               ;
            case GL_BGR             : return PixelFormat::BGR               ;
            case GL_RGBA            : return PixelFormat::RGBA              ;
            case GL_BGRA            : return PixelFormat::BGRA              ;
            case GL_DEPTH_COMPONENT : return PixelFormat::DepthComponent    ;
            case GL_DEPTH_STENCIL   : return PixelFormat::DepthStencil      ;
            // ...
            default: ASSERT_INVALID(); return (PixelFormat)0;
        }
    }

    void Texture::LoadInternal()
    {
        ASSERT( !IsLoaded() && m_Surface.IsLoaded() );
        u32 textureMode = 0;
        switch (m_Surface.GetBytesPerPixel())
        {
            case 1: textureMode = GL_RED;  break;
            case 2: textureMode = GL_RG;   break;
            case 3: textureMode = GL_RGB;  break;
            case 4: textureMode = GL_RGBA; break;
            default:
            {
                LOG_ERROR( "Graphics", "Unsupported image format: %d. Loading texture %s aborted", m_Surface.GetPixelFormatName().c_str(), m_Name.c_str() );
                return;
            }
        }

        m_Width  = m_Surface.GetWidth();
        m_Height = m_Surface.GetHeight();

        glGenTextures( 1, &m_ID );
        glBindTexture( GL_TEXTURE_2D, m_ID );

        glTexImage2D( GL_TEXTURE_2D, 0, textureMode, m_Width, m_Height, 0, textureMode, GL_UNSIGNED_BYTE, m_Surface.GetPixelData() );
        //glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, 0 );

        m_PixelFormat = GetPixelFormatFromTextureMode(textureMode);

        ++s_NumTexturesFromSurfaces;
    }

    void Texture::UnloadInternal()
    {
        glDeleteTextures( 1, &m_ID );
    }
}

#endif