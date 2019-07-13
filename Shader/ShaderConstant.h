#pragma once

#include "Graphic/GraphicsConstants.h"

#define SHADER_CONSTANT_ACCEPTED_TYPES \
    Framework::RGBAColor, \
    bool, \
    int, \
    float, \
    glm::vec2, glm::vec3, glm::vec4, \
    glm::mat2, glm::mat3, glm::mat4

namespace Framework
{
    struct ShaderConstant
    {
        s32 location;
        ShaderElementType type;

        template <typename T>
        static constexpr ShaderElementType GetParsedType();

        inline bool operator==(const ShaderConstant &other) const;
        friend bool inline operator== (const ShaderConstant& lhs, const ShaderConstant& rhs);
        friend bool inline operator!= (const ShaderConstant& lhs, const ShaderConstant& rhs);
    };

    template<typename T>
    constexpr ShaderElementType ShaderConstant::GetParsedType()
    {
        if constexpr (IS_SAME(T, RGBAColor))                return ShaderElementType::Vector4;
        else if      (IS_SAME(T, bool     ))                return ShaderElementType::Bool;
        else if      (IS_ANY_OF(T, int, u8, u32, u16, u64)) return ShaderElementType::Int;
        else if      (IS_ANY_OF(T, float, f32))             return ShaderElementType::Float;
        else if      (IS_SAME(T, glm::vec2))                return ShaderElementType::Vector2;
        else if      (IS_SAME(T, glm::vec3))                return ShaderElementType::Vector3;
        else if      (IS_SAME(T, glm::vec4))                return ShaderElementType::Vector4;
        else if      (IS_SAME(T, glm::mat2))                return ShaderElementType::Matrix2;
        else if      (IS_SAME(T, glm::mat3))                return ShaderElementType::Matrix3;
        else if      (IS_SAME(T, glm::mat4))                return ShaderElementType::Matrix4;
        return ShaderElementType::Invalid;
    }

    inline bool ShaderConstant::operator==(const ShaderConstant &other) const
    {
        return location == other.location &&
               type == other.type;
    }

    inline bool operator== (const ShaderConstant& lhs, const ShaderConstant& rhs)
    {
        return lhs.location == rhs.location &&
               lhs.type == rhs.type;
    }

    inline bool operator!= (const ShaderConstant& lhs, const ShaderConstant& rhs)
    {
        return lhs.location != rhs.location ||
               lhs.type != rhs.type;
    }
}