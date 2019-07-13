#include "Precompiled.h"

#include "Graphic/BO/BufferObjectPool.h"

namespace Framework
{
    void BufferObjectPool::AddBO(Reference<BufferObject> go)
    {
        m_BufferObjectsToLoad.emplace(go);
    }

    void BufferObjectPool::RemoveBO(Reference<BufferObject> go)
    {
        m_BufferObjectsToUnload.emplace(go);
    }

    void BufferObjectPool::Update()
    {
        while (!m_BufferObjectsToLoad.empty())
        {
            m_BufferObjectsToLoad.front()->Initialize();
            m_BufferObjectsToLoad.pop();
        }

        while (!m_BufferObjectsToUnload.empty())
        {
            m_BufferObjectsToUnload.front()->DeInitialize();
            const auto it = std::remove(m_BufferObjects.begin(), m_BufferObjects.end(), m_BufferObjectsToUnload.front());
            ASSERT(it != m_BufferObjects.end());
            m_BufferObjects.erase(it);
            m_BufferObjectsToUnload.pop();
        }
    }

    void BufferObjectPool::Release()
    {
        Update();

        for (const auto& bo : m_BufferObjects)
            bo->DeInitialize();

        m_BufferObjects.clear();
    }
 }
