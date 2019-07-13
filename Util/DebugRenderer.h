#pragma once

#ifdef _DEBUG

#include "Graphic/Content/Material.h"
#include "Graphic/Util/DebugShape.h"

namespace Framework
{
    struct RGBAColor;

    class DebugRenderer
    {
        NON_COPYABLE_CLASS(DebugRenderer);
        friend class GraphicsManager;

    public:
        DebugRenderer() = default;
        ~DebugRenderer() = default;

        void Initialize(const Config& config);
        void DeInitialize();

        void SetLineWidth(f32 lineWidth) { m_LineWidth = lineWidth; }
        f32 GetLineWidth() const         { return m_LineWidth; }

        void PushCircle(f32 centerX, f32 centerY, f32 radius, const RGBAColor& color = RGBAColor::White);
        void PushCircle(const glm::vec2& position, f32 radius, const RGBAColor& color = RGBAColor::White);
        void PushCircle(const glm::mat4& transform, f32 radius, const RGBAColor& color = RGBAColor::White);

        void PushRectangle(f32 centerX, f32 centerY, f32 width, f32 height, const RGBAColor& color = RGBAColor::White);
        void PushRectangle(const glm::vec2& position, f32 width, f32 height, const RGBAColor& color = RGBAColor::White);
        void PushRectangle(const glm::mat4& transform, f32 width, f32 height, const RGBAColor& color = RGBAColor::White);

    private:
        void Render(const glm::mat4& viewProjectionMatrix);

    private:
        std::unordered_map<DebugShapeType, DebugShape> m_Shapes;
        Material m_Material;
        f32 m_LineWidth;
    };
}

#endif