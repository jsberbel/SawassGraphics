#include "Precompiled.h"

#include "json/json.h"
#include "Core/Serialization/JsonCppUtils.h"

#include "Graphic/Content/Material.h"
#include "Graphic/Content/Texture.h"
#include "Graphic/Shader/GraphicsTechnique.h"
#include "Engine/Engine.h"

#include <utility>

namespace Framework
{
    Material::Material()
        : m_Technique(nullptr)
    { 
    }

    Material::Material(std::shared_ptr<GraphicsTechnique> technique)
        : m_Technique(std::move(technique))
    {
    }

    void Material::SetTextureSampler(const std::string& name, std::shared_ptr<Texture> texture, const TextureParameters& params)
    {
        ASSERT(texture);
        m_Samplers[name] = TextureSampler{ texture, params };
    }

    void Material::SetTextureSampler(const std::string& name, const TextureSampler& textureSampler)
    {
        ASSERT(textureSampler.texture);
        m_Samplers[name] = textureSampler;
    }

    void Material::SetDefaultTextureSampler(const TextureSampler& textureSampler)
    {
        ASSERT(textureSampler.texture);
        if (const auto it = m_Samplers.begin(); it != m_Samplers.end())
        {
            it->second = textureSampler;
        }
        else
        {
            m_Samplers["Default"] = textureSampler;
        }
    }

    void Material::SetTexture(const std::string& name, std::shared_ptr<Texture> texture)
    {
        ASSERT(texture);
        m_Samplers[name].texture = std::move(texture);
    }

    void Material::SetDefaultTexture(std::shared_ptr<Texture> texture)
    {
        ASSERT(texture);
        if (const auto it = m_Samplers.begin(); it != m_Samplers.end())
        {
            it->second.texture = std::move(texture);
        }
        else
        {
            m_Samplers["Default"].texture = std::move(texture);
        }
    }

    const TextureSampler& Material::GetTextureSampler(const std::string& name) const
    {
        const auto it = m_Samplers.find(name);
        ASSERT(it != m_Samplers.end());
        return it->second;
    }

    Observer<TextureSampler> Material::FindTextureSampler(const std::string& name) const
    {
        const auto it = m_Samplers.find(name);

        if (it != m_Samplers.end())
            return Utils::Observe(it->second);

        return nullptr;
    }

    Observer<TextureSampler> Material::FindDefaultTextureSampler() const
    {
        const auto it = m_Samplers.begin();

        if (it != m_Samplers.end())
            return Utils::Observe(it->second);

        return nullptr;
    }

    u32 Material::GetDefaultTextureID() const
    {
        if (const auto oSampler = FindDefaultTextureSampler())
        {
            if (oSampler->texture)
                return oSampler->texture->GetID();
        }
        return static_cast<u32>(-1);
    }

    void Material::Serialize(Json::Value& serializer) const
    {
        serializer["technique"] = m_Technique->GetFileName();

        auto& texturesJson = serializer["textures"];
        for (const auto& sampler : m_Samplers)
        {
            Json::Value textureJson;
            textureJson["name"] = sampler.first;

            if (const auto textureFile = std::dynamic_pointer_cast<TextureFile>(sampler.second.texture))
                textureJson["filePath"] = textureFile->GetFileName();

            sampler.second.params.Serialize(textureJson);
            texturesJson.append(textureJson);
        }
    }

    void Material::Deserialize(const Json::Value& serializer)
    {
        const auto& graphicsTechniqueJson = serializer["technique"];
        m_Technique = G_Resource.GetOrCreate<GraphicsTechnique>(graphicsTechniqueJson.asString());

        if (const auto& texturesJson = serializer["textures"])
        {
            for (const auto& textureJson : texturesJson)
            {
                const auto& textureName = textureJson["name"].asString();
                auto& textureSampler = m_Samplers[textureName];

                if (const auto& textureFilePath = textureJson["filePath"])
                {
                    const std::shared_ptr<TextureFile> textureFile = G_Resource.GetOrCreate<TextureFile>(textureFilePath.asString());
                    textureSampler.texture = std::dynamic_pointer_cast<Texture>(textureFile);
                    textureSampler.params.Deserialize(textureJson);
                }
            }
        }
    }

    bool operator==(const Material& lhs, const Material& rhs)
    {
        return lhs.m_Technique == rhs.m_Technique && lhs.m_ShaderConstantsData == rhs.m_ShaderConstantsData && lhs.m_Samplers == rhs.m_Samplers;
    }

    bool operator!=(const Material& lhs, const Material& rhs)
    {
        return !(lhs == rhs);
    }

    //----------------------------------------------------------------

    MaterialFile::MaterialFile(const std::string& filePath)
        : Resource(filePath)
        , Material()
    {
        DEBUG_EXP( m_Name = GetFileName(); )

        const Json::Value deserializer = Json::JsonUtils::OpenAndParseJsonFromFile(filePath);
        Material::Deserialize(deserializer);
    }
}