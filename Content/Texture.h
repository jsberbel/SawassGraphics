/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include "Core/Serialization/ISerializableObject.h"

#include "Engine/Resource.h"

#include "Graphic/GraphicsConstants.h"
#include "Graphic/Content/Surface.h"

namespace Framework
{
    class Texture
    {
    public:
        Texture();
        Texture(const Surface& surface);
        Texture(Surface&& surface);
        virtual ~Texture();

        void LoadFromSurface(const Surface& surface);
        void LoadFromSurface(Surface&& surface);
        void Unload();

        bool IsLoaded()              const { return m_ID != 0u;     }
        u32 GetID()                  const { return m_ID;           }
        u32 GetWidth()               const { return m_Width;        }
        u32 GetHeight()              const { return m_Height;       }
        PixelFormat GetPixelFormat() const { return m_PixelFormat;  }
        const Surface& GetSurface()  const { return m_Surface;      }

    protected:
        void LoadInternal();   // <GraphicsAPI specific>
        void UnloadInternal(); // <GraphicsAPI specific>

    protected:
        u32 m_ID;
        u32 m_Width;
        u32 m_Height;
        PixelFormat m_PixelFormat;
        Surface m_Surface;

        DEBUG_EXP( std::string m_Name; )

    private:
        static inline u32 s_NumTexturesFromSurfaces = 0;
    };

    class TextureFile : public Resource, public Texture
    {
        friend class ResourceManager;

    public:
        DEBUG_EXP( float GetUsedVRam() const override; )

    private:
        explicit TextureFile(const std::string& filePath);
    };
}
