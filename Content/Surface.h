/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

struct SDL_Surface;

namespace Framework
{
    struct RGBAColor;

    class Surface
    {
    public:
        Surface();
        Surface(const std::string& filePath);
        Surface(const Font& font, const std::string& text, const RGBAColor& color, u32 wrapLength = k_DefaultTextWrapLength);
        ~Surface();

        Surface(const Surface& other);
        Surface& operator=(const Surface& other);

        Surface(Surface&& other) noexcept;
        Surface& operator=(Surface&& other) noexcept;

        void LoadFromFile(const std::string& filePath);
        void LoadFromText(const Font& font, const std::string& text, const RGBAColor& color, u32 wrapLength = k_DefaultTextWrapLength);
        void Unload();

        DEBUG_EXP( void SaveToFileAsPNG(const std::string& filePath) const; )

        bool IsLoaded() const { return m_Handle != nullptr; }

        u32         GetWidth()           const;
        u32         GetHeight()          const;
        u32         GetBitsPerPixel()    const;
        u32         GetBytesPerPixel()   const;
        std::string GetPixelFormatName() const;
        const void* GetPixelData()       const;

    private:
        static constexpr u32 k_DefaultTextWrapLength = 1024;

        SDL_Surface* m_Handle;
    };
}
