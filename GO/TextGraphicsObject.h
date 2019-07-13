#pragma once

#include "Graphic/GO/GraphicsObject.h"

namespace Framework
{
    struct TextGraphicsObject : public GraphicsObject2D
    {
        struct CharacterData
        {
            SpriteRect position;
            SpriteRect uv;
            CharacterData(SpriteRect p, SpriteRect u) : position(p), uv(u) {}
        };

        std::vector<CharacterData> characters;

    protected:
        friend class GraphicsObjectPool;
        inline void UpdateToBuffers(VertexBufferObject& vbo, IndexBufferObject<u16>& ibo) override;
    };

    inline void TextGraphicsObject::UpdateToBuffers(VertexBufferObject& vbo, IndexBufferObject<u16>& ibo)
    {
        for (const auto& character : characters)
        {
            const u32 vertexCount = vbo.GetVertexCount();

            for (const auto& idx : k_QuadIdxs)
                ibo.Push(static_cast<u16>(idx + vertexCount));

            for (u32 i = 0; i < 4; ++i)
            {
                vbo.Push(character.position[i*2], character.position[i*2+1], static_cast<f32>(renderLayer)); // position
                vbo.Push(color.R.AsFloat(), color.G.AsFloat(), color.B.AsFloat(), color.A.AsFloat()); // color
                vbo.Push(character.uv[i*2], character.uv[i*2+1]); // texture coordinates
            }
        }
    }
 }