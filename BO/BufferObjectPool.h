#pragma once

#include "Graphic/BO/BufferObject.h"
#include "Core/DataStructures/Reference.h"
#include "Core/Utils/GenericUtils.h"

#include <vector>
#include <queue>

namespace Framework
{
    class BufferObjectPool
    {
        NON_COPYABLE_CLASS(BufferObjectPool);
        friend class BufferObject;
        friend class GraphicsManager;

    private:
        BufferObjectPool() = default;

        void AddBO(Reference<BufferObject> go);
        void RemoveBO(Reference<BufferObject> go);

        void Update();
        void Release();

    private:
        std::vector<Reference<BufferObject>> m_BufferObjects;

        std::queue<Reference<BufferObject>> m_BufferObjectsToLoad;
        std::queue<Reference<BufferObject>> m_BufferObjectsToUnload;
    };
 }
