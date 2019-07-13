#pragma once

#include "Graphic/GraphicsConstants.h"
#include "Core/Serialization/ISerializableObject.h"

#include <memory>

namespace Framework
{
    class Texture;

    struct TextureParameters : public ISerializableObject
    {
        struct { FilterMode min, mag; } filtering = { FilterMode::Linear, FilterMode::Linear };
        struct { WrapMode   u, v; }     wrapping  = { WrapMode::Repeat, WrapMode::Repeat };
        BlendFunc                       blending  = BlendFunc::None;

        // NOTE(jserrano): Add parameters if needed

        void Serialize(Json::Value& serializer) const override;
        void Deserialize(const Json::Value& serializer) override;

        DEBUG_EXP( void DrawOnImGui(); )

        friend bool operator== (const TextureParameters& lhs, const TextureParameters& rhs);
        friend bool operator!= (const TextureParameters& lhs, const TextureParameters& rhs);
    };

    struct TextureSampler
    {
        std::shared_ptr<Texture> texture;
        TextureParameters params;

        TextureSampler() : texture(nullptr) {};
        TextureSampler(std::shared_ptr<Texture> texture) : texture(std::move(texture)) {}
        TextureSampler(std::shared_ptr<Texture> texture, const TextureParameters& params) : texture(std::move(texture)), params(params) {}

        friend bool operator== (const TextureSampler& lhs, const TextureSampler& rhs);
        friend bool operator!= (const TextureSampler& lhs, const TextureSampler& rhs);
    };
}