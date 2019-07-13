#pragma once

#include "Engine\Resource.h"
#include "Core/Serialization/ISerializableObject.h"
#include "Graphic/Shader/VertexInput.h"
#include "Graphic/Shader/ShaderConstant.h"

#include <vector>
#include <set>
#include <optional>

namespace Framework
{
    class Shader;

    // TODO(jserrano): Maybe extend this class to allow using multiple passes (lighting, shadow, deferred,...)

    class GraphicsTechnique : public Resource, public ISerializableObject
    {
        NON_COPYABLE_CLASS(GraphicsTechnique);
        friend class ResourceManager;
        friend class Material;

    public:
        ~GraphicsTechnique() override = default;

        void Serialize(Json::Value& serializer) const override;
        void Deserialize(const Json::Value& deserializer) override;

        u32 GetID() const    { return m_ID; }
        bool IsValid() const { return m_ID != k_InvalidID; }

        u32 GetTotalInputsOffset() const { return m_TotalInputsOffset; }

        const std::vector<VertexInput>& GetInputs() const { return m_Inputs; }
        std::set<u32> GetInputLocationsByName(const std::string& name) const;

        template <class... Names, ENABLE_IF(ARE_SAME(Names, std::string))>
        std::set<u32> GetInputLocationsByNames(Names&&... names) const;
        
        bool ContainsShaderConstant(const std::string& name) const;
        const ShaderConstant& GetShaderConstant(const std::string& name) const;
        std::optional<ShaderConstant> FindShaderConstant(const std::string& name) const;

    #ifdef _DEBUG
        const std::string& GetName() const { return m_Name; }
    #endif

    private:
        explicit GraphicsTechnique(const std::string& filePath);

        void BuildAndLink(); // <GraphicsAPI specific>

    private:
        static constexpr u32 k_InvalidID = static_cast<u32>(-1);

        u32 m_ID;
        std::vector<std::shared_ptr<Shader>> m_Shaders;
        std::vector<VertexInput> m_Inputs;
        std::unordered_map<std::string, ShaderConstant> m_Constants;
        u32 m_TotalInputsOffset;
        DEBUG_EXP( std::string m_Name; )
    };

    template <class... Names, typename>
    std::set<u32> GraphicsTechnique::GetInputLocationsByNames(Names&&... names) const
    {
        std::set<u32> result;
        const auto PushLocations = [&](std::set<u32>&& inputs){ result.insert(inputs.begin(), inputs.end()); };
        ( PushLocations(GetInputLocationsByName(std::forward<Names>(names))), ... );
        return result;
    }
}
