#include "Precompiled.h"

#include "json/json.h"
#include "Core/Serialization/JsonCppUtils.h"

#include "Graphic/Shader/GraphicsTechnique.h"
#include "Graphic/Shader/Shader.h"
#include "Graphic/GraphicsConstants.h"
#include "Engine/ResourceManager.h"
#include "Engine/Engine.h"
#include "Graphic/GraphicsAPI.h"

namespace Framework
{
    GraphicsTechnique::GraphicsTechnique(const std::string& filePath)
        : Resource(filePath)
        , m_ID(k_InvalidID)
        , m_Inputs()
    {
        const Json::Value deserializer = Json::JsonUtils::OpenAndParseJsonFromFile(filePath);
        GraphicsTechnique::Deserialize(deserializer);
    }
    
    void GraphicsTechnique::Serialize(Json::Value& serializer) const
    {
        
    }

    void GraphicsTechnique::Deserialize(const Json::Value& deserializer)
    {
        DEBUG_EXP( m_Name = deserializer["name"].asString(); )

        if (Json::Value shaderFiles = deserializer["shaders"])
        {
            ASSERT(shaderFiles.isArray());
            for (auto& shaderFileName : shaderFiles)
            {
                m_Shaders.emplace_back( G_Resource.GetOrCreate<Shader>(shaderFileName.asString()) );
            }
            BuildAndLink();
        }

        if (Json::Value serializedInputs = deserializer["inputs"])
        {
            ASSERT(serializedInputs.isArray());

            u32 location = 0u;
            u32 offset = 0u;

            for (const auto& serializedInput : serializedInputs)
            {
                const std::string name = serializedInput["name"].asString();

                const std::string typeName = serializedInput["type"].asString();
                const ShaderElementType type = StringUtils::FromString<ShaderElementType>(typeName);
                ASSERT(type != ShaderElementType::Invalid && type != ShaderElementType::Matrix3 && type != ShaderElementType::Sampler2D);
                
                const bool perInstance = serializedInput["perInstance"].asBool();

                constexpr u32 maxNbComponentsPerRegister = 4u; // NOTE(jserrano): Shader registers are limited to 4 components each
                u32 nbComponents = GraphicsAPI::GetShaderElementNbComponents(type);
                const ShaderElementComponentType componentType = GraphicsAPI::GetShaderElementComponentType(type);

                const u32 nbInputsToAdd = (nbComponents - 1u) / maxNbComponentsPerRegister + 1u;
                for (u32 i = 0; i < nbInputsToAdd; ++i)
                {
                    VertexInput vInput;
                    vInput.name          = name;
                    vInput.location      = location++;
                    vInput.nbComponents  = std::min(nbComponents, maxNbComponentsPerRegister);
                    vInput.componentType = componentType;
                    vInput.perInstance   = perInstance;
                    vInput.offset        = offset;
                    m_Inputs.push_back(std::move(vInput));
                    
                    offset += GraphicsAPI::GetShaderElementTypeSize(type);
                    nbComponents -= maxNbComponentsPerRegister;
                }
            }

            m_TotalInputsOffset = offset;
        }

        if (Json::Value serializedConstants = deserializer["constants"])
        {
            ASSERT(serializedConstants.isArray());

            for (const auto& serializedConstant : serializedConstants)
            {
                const std::string name = serializedConstant["name"].asString();
                ShaderConstant& constant = m_Constants[name];

                ASSERT(IsValid());
                constant.location = GraphicsAPI::GetShaderConstantLocation(m_ID, name);
                ASSERT(constant.location != -1);

                const std::string typeName = serializedConstant["type"].asString();
                constant.type = StringUtils::FromString<ShaderElementType>(typeName);
                ASSERT(constant.type != ShaderElementType::Invalid);
            }
        }
    }

    std::set<u32> GraphicsTechnique::GetInputLocationsByName(const std::string& name) const
    {
        std::set<u32> result;
        for (const auto& it : m_Inputs)
        {
            if (it.name == name)
                result.emplace(it.location);
        }
        return result;
    }

    bool GraphicsTechnique::ContainsShaderConstant(const std::string& name) const
    {
        const auto it = m_Constants.find(name);
        return (it != m_Constants.end());
    }

    const ShaderConstant& GraphicsTechnique::GetShaderConstant(const std::string& name) const
    {
        const auto it = m_Constants.find(name);
        return it->second;
    }

    std::optional<ShaderConstant> GraphicsTechnique::FindShaderConstant(const std::string& name) const
    {
        const auto it = m_Constants.find(name);

        if (it != m_Constants.end())
            return it->second;

        return std::nullopt;
    }
}
