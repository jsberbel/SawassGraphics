#pragma once

#include "Graphic/Shader/VertexInput.h"
#include "Core/Utils/GenericUtils.h"

#include <variant>
#include <vector>

namespace Framework
{
    #define SHADER_BUFFER_COMPONENT_TYPES u8, u16, u32, s8, s16, s32, f32, f64

    class ShaderBuffer
    {
    public:
        ShaderBuffer();

        template <class VertexInputComponent, ENABLE_IF(CAN_CAST_TO_ANY_OF(VertexInputComponent, SHADER_BUFFER_COMPONENT_TYPES))>
        constexpr void Push(VertexInputComponent&& componentValue, std::size_t alignment = alignof(VertexInputComponent));

        template <class VertexInputComponent, ENABLE_IF(CAN_CAST_TO_ANY_OF(VertexInputComponent, SHADER_BUFFER_COMPONENT_TYPES))>
        void PushBuffer(VertexInputComponent* bufferBegin, u32 bufferSize);

        void Reserve(u32 capacity);
        void Clear();
        void Release();

        constexpr const byte* GetRawData() const { return m_RawMemory; }
        constexpr std::size_t GetSize() const    { return m_Size; }
        constexpr std::size_t IsEmpty() const    { return m_Size == 0; }

    private:
        static constexpr u32 k_DefaultCapacity = 32;

        byte* m_RawMemory;
        void* m_Iterator;
        std::size_t m_Size;
        std::size_t m_Capacity;

    #ifdef _DEBUG
        std::vector<std::variant<SHADER_BUFFER_COMPONENT_TYPES>> m_DebugContent;
    #endif
    };

    template <typename VertexInputComponent, typename>
    constexpr void ShaderBuffer::Push(VertexInputComponent&& componentValue, std::size_t alignment /*= alignof(VertexInputComponent)*/)
    {
        using DecayVertexType = std::decay_t<VertexInputComponent>;
        const std::size_t componentSize = sizeof(DecayVertexType);

        if (m_Size + componentSize >= m_Capacity)
            Reserve(m_Size + componentSize + k_DefaultCapacity);

        //const void* alignedMemory = std::align(alignment, vertexSize, m_Iterator, m_Capacity - m_Size);
        //ASSERT(alignedMemory);

        DecayVertexType* allocatedMemory = reinterpret_cast<DecayVertexType*>(m_Iterator);
        *allocatedMemory = std::forward<VertexInputComponent>(componentValue);
        m_Iterator = static_cast<byte*>(m_Iterator) + componentSize;
        m_Size += componentSize;

        DEBUG_EXP( m_DebugContent.push_back(componentValue); )
    }

    template <typename VertexInputComponent, typename>
    void ShaderBuffer::PushBuffer(VertexInputComponent* bufferBegin, u32 bufferSize)
    {
        using DecayVertexType = std::decay_t<VertexInputComponent>;
        const std::size_t componentSize = sizeof(DecayVertexType);
        const std::size_t rawBufferSize = componentSize * bufferSize;

        if (m_Size + rawBufferSize >= m_Capacity)
            Reserve(m_Size + rawBufferSize + k_DefaultCapacity);

        //const void* alignedMemory = std::align(alignment, vertexSize, m_Iterator, m_Capacity - m_Size);
        //ASSERT(alignedMemory);

        DecayVertexType* allocatedMemory = reinterpret_cast<DecayVertexType*>(m_Iterator);
        for (u32 i = 0; i < bufferSize; ++i)
        {
            allocatedMemory[i] = bufferBegin[i];
            DEBUG_EXP( m_DebugContent.push_back(bufferBegin[i]); )
        }
        m_Iterator = static_cast<byte*>(m_Iterator) + rawBufferSize;
        m_Size += rawBufferSize;
    }
}