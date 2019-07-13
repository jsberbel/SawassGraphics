#pragma once

#include "Graphic/BO/BufferObject.h"

namespace Framework
{
    template <class IndexType>
    class IndexBufferObject : public BufferObject
    {
    public:
        constexpr IndexBufferObject();
        ~IndexBufferObject() override = default;

        void UpdateToGPU() override; // <GraphicsAPI specific>

        void Clear();

        constexpr void Push(IndexType index);

        bool IsEmpty() const { return m_Indices.empty(); }
        u32 GetIndexCount() const { return m_Indices.size(); }

        static constexpr u32 GetIndexTypeSize() { return sizeof(IndexType); }

    private:
        void Initialize() override; // <GraphicsAPI specific>
        void DeInitialize() override; // <GraphicsAPI specific>

    private:
        std::vector<IndexType> m_Indices;
    };

    template class IndexBufferObject<u8>;
    template class IndexBufferObject<u16>;
    template class IndexBufferObject<u32>;
}