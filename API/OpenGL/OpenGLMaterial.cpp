#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include "Graphic/Content/Material.h"
#include "Graphic/Content/Texture.h"
#include "Graphic/API/OpenGL/OpenGLUtils.h"
#include "Graphic/Shader/GraphicsTechnique.h"

#include <glad/glad.h>

namespace Framework
{
    constexpr u32 GetOpenGLFilterMode(FilterMode filterMode)
    {
        switch (filterMode)
        {
            case FilterMode::Nearest:  return GL_NEAREST;
            case FilterMode::Linear:   return GL_LINEAR;
            default: ASSERT_INVALID(); return 0;
        }
    }

    constexpr u32 GetOpenGLWrapMode(WrapMode wrapMode)
    {
        switch (wrapMode)
        {
            case WrapMode::Clamp:        return GL_CLAMP_TO_EDGE;
            case WrapMode::Repeat:       return GL_REPEAT;
            case WrapMode::MirrorRepeat: return GL_MIRRORED_REPEAT;
            default: ASSERT_INVALID();   return 0;
        }
    }

    inline void SetOpenGLTextureParameters(const TextureParameters& params)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(params.filtering.min));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(params.filtering.mag));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(params.wrapping.u));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(params.wrapping.v));

        if (params.blending != BlendFunc::None)
        {
            glEnable(GL_BLEND);

            switch (params.blending)
            {
                case BlendFunc::AlphaBlend:
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                case BlendFunc::Additive:
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    break;
                case BlendFunc::Premultiplied:
                    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                default:;
            }
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    inline void SetOpenGLTexture(u32 location, u32 id)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Material::UpdateToGPU()
    {
        if (!m_Technique)
            return;

        const u32 shaderID = GetGraphicsTechniqueID();
        glUseProgram(shaderID);

        u32 textureCounter = 0;
        for (const auto& [name, sampler] : m_Samplers)
        {
            SetOpenGLTexture(textureCounter++, sampler.texture ? sampler.texture->GetID() : 0);
            SetOpenGLTextureParameters(sampler.params);
        }

        if (!textureCounter)
            SetOpenGLTexture(0, 0);

        for (const auto& [name, param] : m_ShaderConstantsData)
        {
            const ShaderConstant& constant = m_Technique->GetShaderConstant(name);
            switch (constant.type)
            {
                case ShaderElementType::Bool   : glUniform1i(constant.location, std::get<int>(param));                                           break;
                case ShaderElementType::Int    : glUniform1i(constant.location, std::get<int>(param));                                           break;
                case ShaderElementType::Float  : glUniform1f(constant.location, std::get<float>(param));                                         break;
                case ShaderElementType::Vector2: glUniform2fv(constant.location, 1, glm::value_ptr(std::get<glm::vec2>(param)));                 break;
                case ShaderElementType::Vector3: glUniform3fv(constant.location, 1, glm::value_ptr(std::get<glm::vec3>(param)));                 break;
                case ShaderElementType::Vector4: glUniform4fv(constant.location, 1, glm::value_ptr(std::get<glm::vec4>(param)));                 break;
                case ShaderElementType::Matrix2: glUniformMatrix2fv(constant.location, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat2>(param))); break;
                case ShaderElementType::Matrix3: glUniformMatrix3fv(constant.location, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat3>(param))); break;
                case ShaderElementType::Matrix4: glUniformMatrix4fv(constant.location, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4>(param))); break;
                default: ASSERT_INVALID();
            }

        #ifdef _DEBUG
            const GLenum errorCode = glGetError();
            const char* errorName = OpenGLUtils::GetErrorName( errorCode );
            ASSERT( errorCode == GL_NO_ERROR );
        #endif
        }
    }
}

#endif