#include "Precompiled.h"

#include "Graphic/BO/VertexBufferObject.h"
#include "Core/Utils/GenericUtils.h"

namespace Framework
{
    VertexBufferObject::VertexBufferObject()
        : BufferObject()
        , m_VertexData()
        , m_VertexStorageSize(0)
        , m_Dirty(false)
    {
    }

    u32 VertexBufferObject::GetVertexCount() const
    {
        UpdateInputsToGPU();
        return GetVertexDataSize() / GetVertexStorageSize();
    }

    void VertexBufferObject::Clear()
    {
        m_VertexData.Clear();
    }

    void VertexBufferObject::Register(std::shared_ptr<GraphicsTechnique> technique)
    {
        if (m_Technique != technique)
        {
            m_Technique = technique;
            m_Dirty = true;
        }
    }
    
    void VertexBufferObject::EnableInputLocation(u32 location)
    {
        const auto [it, isNew] = m_InputLocations.emplace(location);
        if (isNew)
            m_Dirty = true;
    }

    void VertexBufferObject::DisableInputLocation(u32 location)
    {
        const auto it = m_InputLocations.find(location);
        if (it != m_InputLocations.end())
        {
            m_InputLocations.erase(location);
            m_Dirty = true;
        }
    }
    
    void VertexBufferObject::SetInputLocations(std::set<u32>&& locations)
    {
        if (m_InputLocations != locations)
        {
            m_InputLocations = std::move(locations);
            m_Dirty = true;
        }
    }

    void VertexBufferObject::ClearInputLocations()
    {
        if (!m_InputLocations.empty())
        {
            m_InputLocations.clear();
            m_Dirty = true;
        }
    }

    void VertexBufferObject::UpdateVertexStorageSize() const
    {
        if (m_Dirty)
        {
            ASSERT(m_Technique);
            if (m_InputLocations.empty())
            {
                m_VertexStorageSize = m_Technique->GetTotalInputsOffset();
            }
            else
            {
                m_VertexStorageSize = 0;
                const std::vector<VertexInput>& vInputs = m_Technique->GetInputs();
                for (const auto& location : m_InputLocations)
                {
                    const VertexInput& vInput = vInputs[location];
                    m_VertexStorageSize += vInput.GetStorageSize();
                }
            }

            m_Dirty = false;
        }
    }
}