#pragma once

#include "glm/vec4.hpp"

namespace Framework
{
    struct RGBAColorComponent
    {
        explicit RGBAColorComponent();
        explicit RGBAColorComponent(const float value);

        operator float() const { return m_Data / 255.f; }

        RGBAColorComponent& operator=(const float& value);
        friend RGBAColorComponent operator*(const RGBAColorComponent& color, const float scalar);
        void operator*=(const float& value);
        friend RGBAColorComponent operator/(const RGBAColorComponent& color, const float scalar);
        void operator/=(const float& value);
        friend RGBAColorComponent operator+(const RGBAColorComponent& color, const float scalar);
        void operator+=(const float& value);
        friend RGBAColorComponent operator-(const RGBAColorComponent& color, const float scalar);
        void operator-=(const float& value);

        // Convert RGBA color to values in range of [0,1]
        float AsFloat() const { return m_Data / 255.f; }

        // Convert RGBA color to values in range of [0,255]
        u8 As8Bit() const { return m_Data; }

        // Convert RGBA color to values in range of [0,65535]
        u16 As16Bit() const { return static_cast<u16>(m_Data * (65535.f / 255.f)); }

        // Convert RGBA color to values in range of 0% to 100%
        float AsPercentage() const { return m_Data * (100.f / 255.f); }

    private:
        u8 m_Data;
    };

    struct RGBAColor
    {
        RGBAColorComponent R, G, B, A;

        RGBAColor();
        RGBAColor(const float value, const float alpha = 1.f);
        RGBAColor(const float red, const float green, const float blue, const float alpha = 1.f);
        RGBAColor(const RGBAColor& color);

        RGBAColor& operator=(const RGBAColor& color);

        friend RGBAColor operator*(const RGBAColor& color, const float scalar);
        void operator*=(const float scalar);

        friend RGBAColor operator/(const RGBAColor& color, const float scalar);
        void operator/=(const float scalar);

        SDL_Color AsSDLColor() const;
        glm::vec4 AsNormalizedVector() const;
        glm::vec4 As8BitVector() const;
        glm::vec4 As16BitVector() const;

        friend bool operator== (const RGBAColor& lhs, const RGBAColor& rhs);
        friend bool operator!= (const RGBAColor& lhs, const RGBAColor& rhs);

        static const RGBAColor White;
        static const RGBAColor Black;
        static const RGBAColor Red;
        static const RGBAColor Green;
        static const RGBAColor Blue;
        static const RGBAColor Yellow;
        static const RGBAColor Cyan;
        static const RGBAColor Pink;
    };
}
