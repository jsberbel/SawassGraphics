#include "Precompiled.h"

#include "RGBAColor.h"

namespace Framework
{
#define TO8BIT( value ) ( static_cast<byte>( value * 255)  )

    RGBAColorComponent::RGBAColorComponent() : m_Data(0)
    {
    }

    RGBAColorComponent::RGBAColorComponent(const float value)
        : m_Data(TO8BIT(value))
    {
        ASSERT(value >= 0.f && value <= 1.f);
    }

    RGBAColorComponent& RGBAColorComponent::operator=(const float& value)
    {
        ASSERT(value >= 0.f && value <= 1.f);
        m_Data = TO8BIT(value);
        return *this;
    }

    RGBAColorComponent operator*(const RGBAColorComponent& color, const float scalar)
    {
        return RGBAColorComponent(color.m_Data * scalar);
    }

    void RGBAColorComponent::operator*=(const float& value)
    {
        ASSERT(value >= 0.f && value <= 1.f);
        m_Data *= TO8BIT(value);
    }

    RGBAColorComponent operator/(const RGBAColorComponent& color, const float scalar)
    {
        return RGBAColorComponent(color.m_Data / scalar);
    }

    void RGBAColorComponent::operator/=(const float& value)
    {
        ASSERT(value >= 0.f && value <= 1.f);
        m_Data /= TO8BIT(value);
    }

    RGBAColorComponent operator+(const RGBAColorComponent& color, const float scalar)
    {
        return RGBAColorComponent(color.m_Data + scalar);
    }

    void RGBAColorComponent::operator+=(const float& value)
    {
        ASSERT(value >= 0.f && value <= 1.f);
        m_Data += TO8BIT(value);
    }

    RGBAColorComponent operator-(const RGBAColorComponent& color, const float scalar)
    {
        return RGBAColorComponent(color.m_Data - scalar);
    }

    void RGBAColorComponent::operator-=(const float& value)
    {
        ASSERT(value >= 0.f && value <= 1.f);
        m_Data -= TO8BIT(value);
    }

    //----------------------------------------------------------------------

    const RGBAColor RGBAColor::White    { 1.f };
    const RGBAColor RGBAColor::Black    { 0.f };
    const RGBAColor RGBAColor::Red      { 1.f, 0.f, 0.f };
    const RGBAColor RGBAColor::Green    { 0.f, 1.f, 0.f };
    const RGBAColor RGBAColor::Blue     { 0.f, 0.f, 1.f };
    const RGBAColor RGBAColor::Yellow   { 1.f, 1.f, 0.f };
    const RGBAColor RGBAColor::Cyan     { 0.f, 1.f, 1.f };
    const RGBAColor RGBAColor::Pink     { 1.f, 0.f, 1.f };

    RGBAColor::RGBAColor()
        : R(0.f)
        , G(0.f)
        , B(0.f)
        , A(1.f)
    {
    }

    RGBAColor::RGBAColor(const float value, const float alpha)
        : R(value)
        , G(value)
        , B(value)
        , A(alpha)
    {
    }

    RGBAColor::RGBAColor(const float red, const float green, const float blue, const float alpha)
        : R(red)
        , G(green)
        , B(blue)
        , A(alpha)
    {
    }

    RGBAColor::RGBAColor(const RGBAColor& color)
    {
        R = color.R;
        G = color.G;
        B = color.B;
        A = color.A;
    }

    RGBAColor& RGBAColor::operator=(const RGBAColor& color)
    {
        R = color.R;
        G = color.G;
        B = color.B;
        A = color.A;
        return *this;
    }

    RGBAColor operator*(const RGBAColor& color, const float scalar)
    {
        return RGBAColor(color.R * scalar, color.G * scalar, color.B * scalar, color.A * scalar);
    }

    void RGBAColor::operator*=(const float scalar)
    {
        R *= scalar;
        G *= scalar;
        B *= scalar;
        A *= scalar;
    }

    RGBAColor operator/(const RGBAColor& color, const float scalar)
    {
        return RGBAColor(color.R / scalar, color.G / scalar, color.B / scalar, color.A / scalar);
    }

    void RGBAColor::operator/=(const float scalar)
    {
        R /= scalar;
        G /= scalar;
        B /= scalar;
        A /= scalar;
    }

    SDL_Color RGBAColor::AsSDLColor() const
    {
        return SDL_Color{ R.As8Bit(), G.As8Bit(), B.As8Bit(), A.As8Bit() };
    }

    glm::vec4 RGBAColor::AsNormalizedVector() const
    {
        return glm::vec4(R.AsFloat(), G.AsFloat(), B.AsFloat(), A.AsFloat());
    }

    glm::vec4 RGBAColor::As8BitVector() const
    {
        return glm::vec4(R.As8Bit(), G.As8Bit(), B.As8Bit(), A.As8Bit());
    }

    glm::vec4 RGBAColor::As16BitVector() const
    {
        return glm::vec4(R.As16Bit(), G.As16Bit(), B.As16Bit(), A.As16Bit());
    }

    bool operator== (const RGBAColor& lhs, const RGBAColor& rhs)
    {
        return lhs.R == rhs.R && lhs.G == rhs.G && lhs.B == rhs.B && lhs.A == rhs.A;
    }
    
    bool operator!= (const RGBAColor& lhs, const RGBAColor& rhs)
    {
        return !(lhs == rhs);
    }
}
