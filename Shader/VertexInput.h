#pragma once

#include "Core/Types.h"
#include "Graphic/Shader/ShaderConstant.h"

namespace Framework
{
    struct VertexInput
    {
        friend class GraphicsTechnique;
        friend class VertexBufferObject;

        VertexInput() : location(0), nbComponents(0), componentType(ShaderElementComponentType::F32), perInstance(false), offset(0) {}

        std::string name;
        s32 location;
        u32 nbComponents;
        ShaderElementComponentType componentType;
        bool perInstance;

        constexpr u32 GetStorageSize() const;

    private:
        u32 offset; // From vertex start
    };
    
    constexpr u32 VertexInput::GetStorageSize() const
    {
        switch (componentType)
        {
            case ShaderElementComponentType::U8:  return sizeof(u8)  * nbComponents;
            case ShaderElementComponentType::S8:  return sizeof(s8)  * nbComponents;
            case ShaderElementComponentType::U16: return sizeof(u16) * nbComponents;
            case ShaderElementComponentType::S16: return sizeof(s16) * nbComponents;
            case ShaderElementComponentType::U32: return sizeof(u32) * nbComponents;
            case ShaderElementComponentType::S32: return sizeof(s32) * nbComponents;
            case ShaderElementComponentType::F32: return sizeof(f32) * nbComponents;
            case ShaderElementComponentType::F64: return sizeof(f64) * nbComponents;
            default: ASSERT_INVALID(); return 0;
        }
    }
}