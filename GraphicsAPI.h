#pragma once

#include "Graphic/GraphicsConstants.h"

namespace Framework
{
    class Config;
    class Window;
    class Shader;
    class Surface;
    class RenderStates;
    enum class ShaderType;

    namespace GraphicsAPI
    {
        void Initialize(const Config& config, const Window& window);
        void DeInitialize();

        void ClearRenderBuffer();

        void BindIntUniform(u32 shaderID, const std::string& uniformName, int uniformValue);
        void BindFloatUniform(u32 shaderID, const std::string& uniformName, float uniformValue);
        void BindVectorUniform(u32 shaderID, const std::string& uniformName, const glm::vec2& uniformValue);
        void BindVectorUniform(u32 shaderID, const std::string& uniformName, const glm::vec3& uniformValue);
        void BindVectorUniform(u32 shaderID, const std::string& uniformName, const glm::vec4& uniformValue);
        void BindMatrixUniform(u32 shaderID, const std::string& uniformName, const glm::mat2& uniformValue);
        void BindMatrixUniform(u32 shaderID, const std::string& uniformName, const glm::mat3& uniformValue);
        void BindMatrixUniform(u32 shaderID, const std::string& uniformName, const glm::mat4& uniformValue);
        void BindTextureUniform(u32 shaderID, u32 layerID, const u32 textureID);

        s32 GetShaderConstantLocation(u32 shaderID, const std::string& name);

        void SetRasterizedLines(f32 lineWidth);

        void DrawElements(PrimitiveType primitiveType, u32 indexCount, u32 indexTypeSize);
        void DrawElementsInstanced(PrimitiveType primitiveType, u32 indexCount, u32 indexTypeSize, u32 nbInstances);
        void DrawArrays(PrimitiveType primitiveType, u32 startElement, u32 vertexCount);
        void DrawArraysInstanced(PrimitiveType primitiveType, u32 startElement, u32 vertexCount, u32 nbInstances);

        void OnWindowResized(u32 width, u32 height);

        inline constexpr u32 GetShaderElementTypeSize(ShaderElementType type);
        inline constexpr u32 GetShaderElementNbComponents(ShaderElementType type);
        inline constexpr ShaderElementComponentType GetShaderElementComponentType(ShaderElementType type);
    }

    inline constexpr u32 GraphicsAPI::GetShaderElementTypeSize(ShaderElementType type)
    {
        switch (type)
        {
            case ShaderElementType::Bool:    return sizeof(bool);
            case ShaderElementType::Int:     return sizeof(int);
            case ShaderElementType::Float:   return sizeof(float);
            case ShaderElementType::Vector2: return sizeof(glm::vec2);
            case ShaderElementType::Vector3: return sizeof(glm::vec3);
            case ShaderElementType::Vector4: return sizeof(glm::vec4);
            case ShaderElementType::Matrix2: return sizeof(glm::mat2);
            case ShaderElementType::Matrix3: return sizeof(glm::mat3);
            case ShaderElementType::Matrix4: return sizeof(glm::mat4);
            default: ASSERT_INVALID(); return 0;
        }
    }

    inline constexpr u32 GraphicsAPI::GetShaderElementNbComponents(ShaderElementType type)
    {
        switch (type)
        {
            case ShaderElementType::Bool:
            case ShaderElementType::Int:
            case ShaderElementType::Float:
                return 1;
            case ShaderElementType::Vector2:
                return 2;
            case ShaderElementType::Vector3:
                return 3;
            case ShaderElementType::Vector4:
            case ShaderElementType::Matrix2:
                return 4;
            case ShaderElementType::Matrix3:
                return 9;
            case ShaderElementType::Matrix4:
                return 16;
            default:
                return 0;
        }
    }

    inline constexpr ShaderElementComponentType GraphicsAPI::GetShaderElementComponentType(ShaderElementType type)
    {
        switch (type)
        {
            case ShaderElementType::Int:
                return ShaderElementComponentType::U32;
            case ShaderElementType::Float:
            case ShaderElementType::Vector2: 
            case ShaderElementType::Vector3: 
            case ShaderElementType::Vector4: 
            case ShaderElementType::Matrix2: 
            case ShaderElementType::Matrix3: 
            case ShaderElementType::Matrix4: 
                return ShaderElementComponentType::F32;
            default: ASSERT_INVALID(); return ShaderElementComponentType(0);
        }
    }
}
