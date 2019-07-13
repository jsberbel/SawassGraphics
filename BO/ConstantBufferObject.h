#pragma once

#include "Graphic/BO/BufferObject.h"

namespace Framework
{
    template <typename PODStruct>
    class ConstantBufferObject : public BufferObject
    {
    public:
        virtual ~ConstantBufferObject() = default;

        void UpdateToGPU() override; // <GraphicsAPI specific>

        PODStruct& GetData() { return m_Data; }
        constexpr u32 GetSize() { return k_Size; }
        constexpr bool IsEmpty() const { return k_Size == 0; }

    private:
        void Initialize() override; // <GraphicsAPI specific>
        void DeInitialize() override; // <GraphicsAPI specific>

    private:
        static constexpr u32 k_Size = sizeof(PODStruct);
        PODStruct m_Data;
    };
}

#ifdef OPENGL_GRAPHICS
#include "Graphic/API/OpenGL/OpenGLConstantBufferObject.inl"
#else
#error Graphics API not defined for SDL Window initialization
#endif