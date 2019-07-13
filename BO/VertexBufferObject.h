#pragma once

#include "Graphic/BO/BufferObject.h"
#include "Graphic/Shader/VertexInput.h"
#include "Graphic/Shader/GraphicsTechnique.h"
#include "Graphic/Shader/ShaderBuffer.h"

#include <set>

namespace Framework
{
    class VertexBufferObject : public BufferObject
    {
    public:
        VertexBufferObject();
        virtual ~VertexBufferObject() = default;

        void UpdateToGPU() override; // <GraphicsAPI specific>

        void Clear();
        
        void Register(std::shared_ptr<GraphicsTechnique> technique);
        void EnableInputLocation(u32 location);
        void DisableInputLocation(u32 location);
        void SetInputLocations(std::set<u32>&& locations);
        void ClearInputLocations();
        
        template <class... VertexInputComponents>
        constexpr void Push(VertexInputComponents&&... componentValues);

        template <class VertexInputComponent>
        void PushBuffer(VertexInputComponent* bufferBegin, u32 bufferSize);

        constexpr bool IsEmpty() const              { return m_VertexData.IsEmpty(); }
        constexpr u32 GetVertexDataSize() const     { return m_VertexData.GetSize(); }
        constexpr u32 GetVertexStorageSize() const  { return m_VertexStorageSize; }
        u32 GetVertexCount() const;

    private:
        void Initialize() override; // <GraphicsAPI specific>
        void DeInitialize() override; // <GraphicsAPI specific>
        inline void UpdateInputToGPU(const VertexInput& vInput, s32 vOverridedOffset = -1) const; // <GraphicsAPI specific>
        inline void UpdateInputsToGPU() const; // <GraphicsAPI specific>
        void UpdateVertexStorageSize() const;

    private:
        ShaderBuffer m_VertexData;
        std::shared_ptr<GraphicsTechnique> m_Technique;
        std::set<u32> m_InputLocations;
        mutable u32 m_VertexStorageSize;
        mutable bool m_Dirty;
    };

    template <class... VertexInputComponents>
    constexpr void VertexBufferObject::Push(VertexInputComponents&&... componentValues)
    {
        ( m_VertexData.Push(std::forward<VertexInputComponents>(componentValues)), ... );
    }
    
    template <class VertexInputComponent>
    void VertexBufferObject::PushBuffer(VertexInputComponent* bufferBegin, u32 bufferSize)
    {
        m_VertexData.PushBuffer(std::forward<VertexInputComponent*>(bufferBegin), bufferSize);
    }
}
