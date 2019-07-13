#include "Precompiled.h"

#include "Graphic/BO/BufferObject.h"
#include "Engine/Engine.h"

namespace Framework
{
    BufferObject::BufferObject(BufferDataMode dataMode /*= BufferDataMode::Static*/)
        : m_ID(0)
        , m_DataMode(dataMode)
    {
        G_Graphics.GetBufferObjectPool().AddBO(*this);
    }

    BufferObject::~BufferObject()
    {
        G_Graphics.GetBufferObjectPool().RemoveBO(*this);
    }
}
