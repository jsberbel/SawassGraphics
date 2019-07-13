#pragma once

#include "Graphic/Util/SpriteRect.h"
#include "Graphic/Content/Material.h"
#include "Graphic/BO/IndexBufferObject.h"
#include "Graphic/BO/VertexBufferObject.h"

namespace Framework
{
    struct GraphicsObject
    {
        virtual ~GraphicsObject() = default;
    protected:
        friend class GraphicsObjectPool;
        inline virtual void UpdateToBuffers(VertexBufferObject& vbo, IndexBufferObject<u16>& ibo) = 0;
    };

    struct GraphicsObject2D : public GraphicsObject
    {
        static constexpr u16 k_QuadIdxs[]
        {
            0, 1, 2,
            0, 2, 3
        };

        std::shared_ptr<Material> material;
        u32 renderLayer;
        RGBAColor color;
    };
 }
