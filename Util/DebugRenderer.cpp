#include "Precompiled.h"

#ifdef _DEBUG

#include "Graphic/Util/DebugRenderer.h"
#include "Graphic/Shader/GraphicsTechnique.h"
#include "Graphic/GraphicsAPI.h"
#include "Engine/ResourceManager.h"
#include "Engine/Transform.h"
#include "Engine/Engine.h"

namespace Framework
{
    inline void FillGeometryVBO(VertexBufferObject& geometryVBO, DebugShapeType type)
    {
        switch (type)
        {
            case DebugShapeType::Circle:
            {
                constexpr u32 circleSegments = 50;
                const auto ComputeCirclePoint = [&](u32 i)
                {
                    const f32 theta = 2.0f * Math::PI * f32(i) / f32(circleSegments);
                    return glm::vec2(cosf(theta), sinf(theta));
                };

                const glm::vec2 firstPoint = ComputeCirclePoint(0);
                glm::vec2 prevPoint = firstPoint;
                for (u32 i = 1; i < circleSegments; ++i)
                {
                    const glm::vec2 curPoint = ComputeCirclePoint(i);
                    geometryVBO.Push(prevPoint.x, prevPoint.y);
                    geometryVBO.Push(curPoint.x, curPoint.y);
                    prevPoint = curPoint;
                }

                geometryVBO.Push(prevPoint.x, prevPoint.y);
                geometryVBO.Push(firstPoint.x, firstPoint.y);
            }
            break;

            case DebugShapeType::Rectangle:
            {
                constexpr f32 pos[4][2]
                {
                    { 0.5f, 0.5f },   // top right
                    { 0.5f, -0.5f },  // bottom right
                    { -0.5f, -0.5f }, // bottom left
                    { -0.5f, 0.5f }   // top left 
                };

                geometryVBO.Push(pos[0][0], pos[0][1]);
                geometryVBO.Push(pos[1][0], pos[1][1]);
                geometryVBO.Push(pos[1][0], pos[1][1]);
                geometryVBO.Push(pos[2][0], pos[2][1]);
                geometryVBO.Push(pos[2][0], pos[2][1]);
                geometryVBO.Push(pos[3][0], pos[3][1]);
                geometryVBO.Push(pos[3][0], pos[3][1]);
                geometryVBO.Push(pos[0][0], pos[0][1]);
            }
            break;

            default:;
        }
    }

    void DebugRenderer::Initialize(const Config& config)
    {
        const std::string& debugTechniquePath = config.GetGraphicsDefault("debugTechnique");
        const auto debugTechnique = G_Resource.GetOrCreate<GraphicsTechnique>(debugTechniquePath);
        m_Material.SetGraphicsTechnique(debugTechnique);
        m_LineWidth = 2.f;

        for (u32 i = 0; i < static_cast<u32>(DebugShapeType::Max); ++i)
        {
            const auto type = static_cast<DebugShapeType>(i);
            const auto [it, isNew] = m_Shapes.try_emplace(type, debugTechnique);
            ASSERT(isNew);
            FillGeometryVBO(it->second.geometryVBO, type);
        }
    }

    void DebugRenderer::DeInitialize()
    {
        m_Shapes.clear();
    }

    void DebugRenderer::PushCircle(f32 centerX, f32 centerY, f32 radius, const RGBAColor& color)
    {
        PushCircle(glm::translate(glm::mat4(), glm::vec3(centerX, centerY, 0.0f)), radius, color);
    }

    void DebugRenderer::PushCircle(const glm::vec2& position, f32 radius, const RGBAColor& color)
    {
        PushCircle(glm::translate(glm::mat4(), glm::vec3(position, 0.0f)), radius, color);
    }

    void DebugRenderer::PushCircle(const glm::mat4& transform, f32 radius, const RGBAColor& color)
    {
        auto it = m_Shapes.find(DebugShapeType::Circle);
        ASSERT(it != m_Shapes.end());
        const glm::mat4 scaledTransform = glm::scale(transform, glm::vec3(radius));
        it->second.instanceVBO.PushBuffer(glm::value_ptr(color.AsNormalizedVector()), sizeof(glm::vec4) / sizeof(f32));
        it->second.instanceVBO.PushBuffer(glm::value_ptr(scaledTransform), sizeof(glm::mat4) / sizeof(f32));
        ++it->second.nbInstances;
    }

    void DebugRenderer::PushRectangle(f32 centerX, f32 centerY, f32 width, f32 height, const RGBAColor& color)
    {
        PushRectangle(glm::translate(glm::mat4(), glm::vec3(centerX, centerY, 0.0f)), width, height, color);
    }

    void DebugRenderer::PushRectangle(const glm::vec2& position, f32 width, f32 height, const RGBAColor& color)
    {
        PushRectangle(glm::translate(glm::mat4(), glm::vec3(position, 0.0f)), width, height, color);
    }

    void DebugRenderer::PushRectangle(const glm::mat4& transform, f32 width, f32 height, const RGBAColor& color)
    {
        auto it = m_Shapes.find(DebugShapeType::Rectangle);
        ASSERT(it != m_Shapes.end());
        const glm::mat4 scaledTransform = glm::scale(transform, glm::vec3(width, height, 1.f));
        it->second.instanceVBO.PushBuffer(glm::value_ptr(color.AsNormalizedVector()), sizeof(glm::vec4) / sizeof(f32));
        it->second.instanceVBO.PushBuffer(glm::value_ptr(scaledTransform), sizeof(glm::mat4) / sizeof(f32));
        ++it->second.nbInstances;
    }

    void DebugRenderer::Render(const glm::mat4& viewProjectionMatrix)
    {
        GraphicsAPI::SetRasterizedLines(m_LineWidth);

        m_Material.SetShaderConstant("viewProjection", viewProjectionMatrix);
        m_Material.UpdateToGPU();

        for (auto& [type, data] : m_Shapes)
        {
            data.UpdateToGPU();

            if (data.instanceVBO.IsEmpty())
                continue;

            const u32 vertexCount = data.geometryVBO.GetVertexCount();
            GraphicsAPI::DrawArraysInstanced(PrimitiveType::LineList, 0, vertexCount, data.nbInstances);

            data.Clear();
        }
    }
}

#endif