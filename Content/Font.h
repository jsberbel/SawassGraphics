/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include "Core/Types.h"
#include "Engine/Resource.h"

struct _TTF_Font;
using TTF_Font = _TTF_Font;

namespace Framework
{
    class Surface;
    class FontFile;

    struct GlyphMetrics
    {
        s32 minX;
        s32 maxX;
        s32 minY;
        s32 maxY;
        s32 advance;
    };

    class Font : public Resource
    {
        friend class ResourceManager;

    public:
        explicit Font(const std::string& filePath, u32 size);
        virtual ~Font();

        void MeasureText(const std::string& text, s32* outWidth, s32* outHeight) const;
        GlyphMetrics GetGlyphMetrics(char glyph) const;

        bool IsLoaded() const;
        const TTF_Font* GetInternalFont() const;
        u32 GetSize() const;

    private:
        void LoadInternal(const int size);
        void Unload();

    private:
        TTF_Font* m_InternalFont;
        u32    m_Size;
    };
}
