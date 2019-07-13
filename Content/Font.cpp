/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#include "SDL_ttf.h"

#include "Graphic/Content/Font.h"
#include "Engine/Engine.h"

namespace Framework
{
    Font::Font(const std::string& filePath, u32 size)
        : m_InternalFont(nullptr)
        , m_Size(size)
        , Resource(filePath)
    {
        LoadInternal(size);
    }

    Font::~Font()
    {
        if (m_InternalFont)
        {
            Unload();
        }
    }

    void Font::MeasureText(const std::string& text, s32* outWidth, s32* outHeight) const
    {
        ASSERT( IsLoaded() );
        const s32 result = TTF_SizeText(m_InternalFont, text.c_str(), outWidth, outHeight);
        ASSERT( result != -1 );
    }

    GlyphMetrics Font::GetGlyphMetrics(char glyph) const
    {
        GlyphMetrics glyphMetrics;
        TTF_GlyphMetrics(m_InternalFont, glyph, &glyphMetrics.minX, &glyphMetrics.maxX, &glyphMetrics.minY, &glyphMetrics.maxY, &glyphMetrics.advance);
        return glyphMetrics;
    }

    void Font::LoadInternal(const int size)
    {
        const char* currentFilePath = GetFilePath().c_str();
        m_InternalFont = TTF_OpenFont(currentFilePath, size);
        ASSERT_OR_ALERT( m_InternalFont != nullptr, "Couldn't load font: %s.\nError: %s", currentFilePath, SDL_GetError() );
    }

    void Font::Unload()
    {
        TTF_CloseFont(m_InternalFont);
        m_InternalFont = nullptr;
        m_Size = 0;
    }

    bool Font::IsLoaded() const
    {
        return m_InternalFont;
    }

    const TTF_Font* Font::GetInternalFont() const
    {
        return m_InternalFont;
    }

    u32 Font::GetSize() const
    {
        return m_Size;
    }
}