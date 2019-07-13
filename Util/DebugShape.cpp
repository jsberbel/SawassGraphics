#include "Precompiled.h"

#ifdef _DEBUG

#include "Graphic/Util/DebugShape.h"
#include "Graphic/Shader/GraphicsTechnique.h"

namespace Framework
{
    DebugShape::DebugShape(const std::shared_ptr<GraphicsTechnique>& technique)
        : nbInstances(0)
    {
        geometryVBO.SetDataMode(BufferDataMode::Static);
        geometryVBO.Register(technique);
        geometryVBO.SetInputLocations(technique->GetInputLocationsByName("position"));

        instanceVBO.SetDataMode(BufferDataMode::Dynamic);
        instanceVBO.Register(technique);
        instanceVBO.SetInputLocations(technique->GetInputLocationsByNames("color", "transform"));
    }

    void DebugShape::UpdateToGPU()
    {
        geometryVBO.UpdateToGPU();
        instanceVBO.UpdateToGPU();
    }

    void DebugShape::Clear()
    {
        instanceVBO.Clear();
    }
}

#endif