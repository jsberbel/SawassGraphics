#pragma once

#ifdef _DEBUG

#include "Graphic/BO/VertexBufferObject.h"

namespace Framework
{
    class GraphicsTechnique;
    struct RGBAColor;

    enum class DebugShapeType
    {
        Circle,
        Rectangle,
        Max
    };

    struct DebugShape
    {
        VertexBufferObject geometryVBO; // stores the original vertex positions of the shape
        VertexBufferObject instanceVBO; // stores the color & transform for each instance of the shape
        u32 nbInstances;

        DebugShape() = delete;
        DebugShape(const std::shared_ptr<GraphicsTechnique>& technique);

        void UpdateToGPU();
        void Clear();
    };
}

#endif