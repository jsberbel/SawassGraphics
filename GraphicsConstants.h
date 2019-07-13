#pragma once

#include "Core/Types.h"
#include "Core/Utils/StringUtils.h"

namespace Framework
{
    enum class PrimitiveType : u32
    {
        TriangleList    ,
        LineList        ,
        PointList       ,
        TriangleStrip   ,
        LineStrip       ,
        TriangleFan     ,
        Max
    };

    ENUM_WITH_STRING_UTILS(PixelFormat, Red, RG, RGB, BGR, RGBA, BGRA, DepthComponent, DepthStencil);

    ENUM_WITH_STRING_UTILS(FilterMode, Nearest, Linear);

    ENUM_WITH_STRING_UTILS(WrapMode, Clamp, Repeat, MirrorRepeat);

    ENUM_WITH_STRING_UTILS(BlendFunc, None, AlphaBlend, Additive, Premultiplied);

    ENUM_WITH_STRING_UTILS(ShaderElementType, Invalid, Bool, Int, Float, Vector2, Vector3, Vector4, Matrix2, Matrix3, Matrix4, Sampler2D);

    ENUM_WITH_STRING_UTILS(ShaderElementComponentType, U8, S8, U16, S16, U32, S32, F32, F64);
}