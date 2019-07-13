#include "Precompiled.h"

#include "Graphic/BO/IndexBufferObject.h"

namespace Framework
{
    template <class IndexType>
    constexpr IndexBufferObject<IndexType>::IndexBufferObject()
        : BufferObject()
        , m_Indices()
    {
    }

    template <class IndexType>
    void IndexBufferObject<IndexType>::Clear()
    {
        m_Indices.clear();
    }

    template <class IndexType>
    constexpr void IndexBufferObject<IndexType>::Push(IndexType index)
    {
        ASSERT(IsValid() && GetIndexTypeSize() == sizeof(IndexType));
        m_Indices.push_back(index);
    }
}