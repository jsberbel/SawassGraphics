#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include "Graphic/BO/IndexBufferObject.h"
#include "Graphic/API/OpenGL/OpenGLUtils.h"

#include <glad/glad.h>

namespace Framework
{
    template <class IndexType>
    void IndexBufferObject<IndexType>::Initialize()
    {
        ASSERT(!IsValid());

        glGenBuffers(1, &m_ID);

        ASSERT(IsValid());
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, m_DataMode == BufferDataMode::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    template <class IndexType>
    void IndexBufferObject<IndexType>::DeInitialize()
    {
        if (IsValid())
        {
            Clear();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
            glDeleteBuffers(1, &m_ID);
            m_ID = 0;
        }
    }

    template <class IndexType>
    void IndexBufferObject<IndexType>::UpdateToGPU()
    {
        ASSERT(IsValid());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

        const GLsizeiptr dataSize = m_Indices.size() * GetIndexTypeSize();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, &m_Indices[0], m_DataMode == BufferDataMode::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    #ifdef _DEBUG
        const GLenum errorCode = glGetError();
        const char* errorName = OpenGLUtils::GetErrorName( errorCode );
        ASSERT( errorCode == GL_NO_ERROR );
    #endif
    }
}

#endif // OPENGL_GRAPHICS