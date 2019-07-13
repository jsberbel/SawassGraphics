#include "Precompiled.h"

#include "Graphic/Content/TextureSampler.h"

namespace Framework
{
    void TextureParameters::Serialize(Json::Value& serializer) const
    {
        serializer["filterModeMin"] = StringUtils::ToString<FilterMode>(filtering.min);
        serializer["filterModeMag"] = StringUtils::ToString<FilterMode>(filtering.mag);
        serializer["wrapModeU"] = StringUtils::ToString<WrapMode>(wrapping.u);
        serializer["wrapModeV"] = StringUtils::ToString<WrapMode>(wrapping.v);
        serializer["blendFunc"] = StringUtils::ToString<BlendFunc>(blending);
    }

    void TextureParameters::Deserialize(const Json::Value& serializer)
    {
        if (auto filterModeMin = serializer["filterModeMin"])
        {
            const char* filterModeMinName = filterModeMin.asCString();
            filtering.min = StringUtils::FromString<FilterMode>(filterModeMinName);
        }

        if (auto filterModeMag = serializer["filterModeMag"])
        {
            const char* filterModeMagName = filterModeMag.asCString();
            filtering.mag = StringUtils::FromString<FilterMode>(filterModeMagName);
        }

        if (auto wrapModeU = serializer["wrapModeU"])
        {
            const char* wrapModeUName = wrapModeU.asCString();
            wrapping.u = StringUtils::FromString<WrapMode>(wrapModeUName);
        }

        if (auto wrapModeV = serializer["wrapModeV"])
        {
            const char* wrapModeVName = wrapModeV.asCString();
            wrapping.v = StringUtils::FromString<WrapMode>(wrapModeVName);
        }

        if (auto blendFunc = serializer["blendFunc"])
        {
            const char* blendFuncName = blendFunc.asCString();
            blending = StringUtils::FromString<BlendFunc>(blendFuncName);
        }
    }

#ifdef _DEBUG
    void TextureParameters::DrawOnImGui()
    {
        // TODO
        /*ImGui::LabelText("Render State", "Blend");
        ImGui::Checkbox("Enabled##Blend", &m_RenderStates[(uint32)RenderStateType::Blend].m_Enabled);
        ImGui::Combo("Blend function", (int*)&m_RenderStates[(uint32)RenderStateType::Blend].m_StateParams,
                     k_BlendFuncNames, Utils::ArrayCount(k_BlendFuncNames));*/
    }
#endif

    bool operator== (const TextureParameters& lhs, const TextureParameters& rhs)
    {
        return lhs.filtering.min == rhs.filtering.min && lhs.filtering.mag == rhs.filtering.mag &&
               lhs.wrapping.u == rhs.wrapping.u && lhs.wrapping.v == rhs.wrapping.v &&
               lhs.blending == rhs.blending;
    }

    bool operator!= (const TextureParameters& lhs, const TextureParameters& rhs)
    {
        return !(lhs == rhs);
    }

    bool operator== (const TextureSampler& lhs, const TextureSampler& rhs)
    {
        return lhs.texture == rhs.texture && lhs.params == rhs.params;
    }

    bool operator!= (const TextureSampler& lhs, const TextureSampler& rhs)
    {
        return !(lhs == rhs);
    }

}