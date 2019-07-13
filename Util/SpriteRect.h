#pragma once

#include "Core/Types.h"

namespace Framework
{
    struct SpriteRect
    {
        glm::vec2 topLeft  { 0.f, 1.f };
        glm::vec2 topRight { 1.f, 1.f };
        glm::vec2 botRight { 1.f, 0.f };
        glm::vec2 botLeft  { 0.f, 0.f };

        f32& operator[]( u32 index )       { return ( &(topLeft[0]) )[index]; }
        f32  operator[]( u32 index ) const { return ( &(topLeft[0]) )[index]; }
    };
 }