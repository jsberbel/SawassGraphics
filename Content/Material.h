#pragma once

#include "Core/Serialization/ISerializableObject.h"
#include "Graphic/Shader/Shader.h"
#include "Graphic/Shader/GraphicsTechnique.h"
#include "Graphic/Shader/ShaderConstant.h"
#include "Graphic/Content/TextureSampler.h"
#include "Graphic/Util/RGBAColor.h"

#include <variant>

namespace Framework
{
    class Texture;
    struct TextureSampler;

    class Material : public ISerializableObject
    {
        NON_COPYABLE_CLASS(Material);

    public:
        Material();
        explicit Material(std::shared_ptr<GraphicsTechnique> technique);
        ~Material() = default;

        void SetGraphicsTechnique(std::shared_ptr<GraphicsTechnique> technique) { ASSERT(technique); m_Technique = std::move(technique); }
        std::shared_ptr<GraphicsTechnique> GetGraphicsTechnique() const { return m_Technique; }
        u32 GetGraphicsTechniqueID() const { ASSERT(m_Technique); return m_Technique->GetID(); }

        void SetTextureSampler(const std::string& name, std::shared_ptr<Texture> texture, const TextureParameters& params);
        void SetTextureSampler(const std::string& name, const TextureSampler& textureSampler);
        void SetDefaultTextureSampler(const TextureSampler& textureSampler);

        void SetTexture(const std::string& name, std::shared_ptr<Texture> texture);
        void SetDefaultTexture(std::shared_ptr<Texture> texture);

        bool HasAnyTexture() const { return !m_Samplers.empty(); }
        const TextureSampler& GetTextureSampler(const std::string& name) const;
        Observer<TextureSampler> FindTextureSampler(const std::string& name) const;
        Observer<TextureSampler> FindDefaultTextureSampler() const;
        u32 GetDefaultTextureID() const;

        template <typename T>
        void SetShaderConstant(const std::string& name, const T& value);

        template <typename T>
        const T& GetShaderConstant(const std::string& name);

        void UpdateToGPU(); // <GraphicsAPI specific>

        void Serialize(Json::Value& serializer) const override;
        void Deserialize(const Json::Value& deserializer) override;

        friend bool operator==(const Material& lhs, const Material& rhs);
        friend bool operator!=(const Material& lhs, const Material& rhs);

    private:
        std::shared_ptr<GraphicsTechnique> m_Technique;
        using ShaderConstantData = std::variant<SHADER_CONSTANT_ACCEPTED_TYPES>;
        std::unordered_map<std::string, ShaderConstantData> m_ShaderConstantsData;
        std::unordered_map<std::string, TextureSampler>  m_Samplers;
    };

    class MaterialFile : public Resource, public Material
    {
        friend class ResourceManager;

    public:
        ~MaterialFile() override = default;

    private:
        explicit MaterialFile(const std::string& filePath);

        DEBUG_EXP( std::string m_Name; )
    };

    template <typename T>
    void Material::SetShaderConstant(const std::string& name, const T& value)
    {
        //static_assert(IS_ANY_OF(T, SHADER_CONSTANT_ACCEPTED_TYPES), "T must be any of the shader parameter accepted types");
        constexpr ShaderElementType type = ShaderConstant::GetParsedType<T>();
        static_assert(type != ShaderElementType::Invalid, "T must be any of the shader parameter accepted types");

        if (m_Technique->ContainsShaderConstant(name))
            m_ShaderConstantsData[name] = value;
    }

    template <typename T>
    const T& Material::GetShaderConstant(const std::string& name)
    {
        constexpr ShaderElementType type = ShaderConstant::GetParsedType<T>();
        static_assert(type != ShaderElementType::Invalid, "T must be any of the shader parameter accepted types");

        ASSERT(m_Technique->ContainsShaderConstant(name));
        const auto shaderConstant = m_ShaderConstantsData.find(name);
        ASSERT_OR_ALERT(shaderConstant->second, "Material property %s is corrupted", name.c_str());
        return std::get<T>(shaderConstant->second);
    }
}
