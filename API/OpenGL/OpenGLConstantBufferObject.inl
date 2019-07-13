#ifdef OPENGL_GRAPHICS

#include "Graphic/API/OpenGL/OpenGLUtils.h"

#include <glad/glad.h>

namespace Framework
{
    template <typename PODStruct>
    class ConstantBufferObject;
    
    template <typename PODStruct>
    void ConstantBufferObject<PODStruct>::Initialize()
    {
        ASSERT(!IsValid());

        glGenBuffers(1, &m_ID);

        ASSERT(IsValid());

        glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
        glBufferData(GL_UNIFORM_BUFFER, 0, nullptr, m_DataMode == BufferDataMode::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    template <typename PODStruct>
    void ConstantBufferObject<PODStruct>::DeInitialize()
    {
        if (IsValid())
        {
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            glDeleteBuffers(1, &m_ID);
            m_ID = 0;
        }
    }

    template <typename PODStruct>
    void ConstantBufferObject<PODStruct>::UpdateToGPU()
    {
        ASSERT(IsValid());

        if (IsEmpty())
            return;
        
        glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
        glBufferData(GL_UNIFORM_BUFFER, k_Size, static_cast<GLvoid*>(&m_Data), m_DataMode == BufferDataMode::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

#ifdef _DEBUG
        const GLenum errorCode = glGetError();
        const char* errorName = OpenGLUtils::GetErrorName(errorCode);
        ASSERT(errorCode == GL_NO_ERROR || errorName);
#endif
    }
}

#endif