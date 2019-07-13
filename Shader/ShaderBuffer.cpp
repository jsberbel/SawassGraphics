#include "Precompiled.h"

#include "Graphic/Shader/ShaderBuffer.h"

namespace Framework
{
    ShaderBuffer::ShaderBuffer()
        : m_RawMemory(nullptr)
        , m_Iterator(nullptr)
        , m_Size(0u)
        , m_Capacity(0u)
    {
        Reserve(k_DefaultCapacity);
    }

    void ShaderBuffer::Reserve(u32 capacity)
    {
        m_Capacity = capacity;
        byte* prevData = m_RawMemory;
        m_RawMemory = new byte[m_Capacity];
        m_Iterator = m_RawMemory + m_Size;

        if (prevData)
        {
            std::memcpy(m_RawMemory, prevData, m_Size);
            delete[] prevData;
        }
    }

    void ShaderBuffer::Clear()
    {
        std::memset(m_RawMemory, 0, m_Size);
        m_Iterator = m_RawMemory;
        m_Size = 0;
        DEBUG_EXP(m_DebugContent.clear(); )
    }

    void ShaderBuffer::Release()
    {
        Utils::SafeDeleteArray(m_RawMemory);
        m_Iterator = nullptr;
        m_Size = 0;
        m_Capacity = 0;
    }
}