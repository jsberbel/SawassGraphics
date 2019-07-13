#pragma once

#include "Graphic/GO/GraphicsObject.h"

namespace Framework
{
    struct SpriteGraphicsObject : public GraphicsObject2D
    {
        SpriteRect position;
        SpriteRect uv;

    protected:
        friend class GraphicsObjectPool;
        inline void UpdateToBuffers(VertexBufferObject& vbo, IndexBufferObject<u16>& ibo) override;
    };

    inline void SpriteGraphicsObject::UpdateToBuffers(VertexBufferObject& vbo, IndexBufferObject<u16>& ibo)
    {
        const u32 vertexCount = vbo.GetVertexCount();

        for (const auto& idx : k_QuadIdxs)
            ibo.Push(static_cast<u16>(idx + vertexCount));

        for (u32 i = 0; i < 4; ++i)
        {
            vbo.Push(position[i*2], position[i*2+1], static_cast<f32>(renderLayer)); // position
            vbo.Push(color.R.AsFloat(), color.G.AsFloat(), color.B.AsFloat(), color.A.AsFloat()); // color
            vbo.Push(uv[i*2], uv[i*2+1]); // texture coordinates
        }
    }
 }
