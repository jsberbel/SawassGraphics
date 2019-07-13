#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include "Graphic/BO/VertexBufferObject.h"
#include "Graphic/Shader/VertexInput.h"
#include "Graphic/API/OpenGL/OpenGLUtils.h"

#include <glad/glad.h>

namespace Framework
{
    constexpr GLuint GetGLElementType(ShaderElementComponentType componentType)
    {
        switch (componentType)
        {
            case ShaderElementComponentType::U8:  return GL_UNSIGNED_BYTE;
            case ShaderElementComponentType::S8:  return GL_BYTE;
            case ShaderElementComponentType::U16: return GL_UNSIGNED_SHORT;
            case ShaderElementComponentType::S16: return GL_SHORT;
            case ShaderElementComponentType::U32: return GL_UNSIGNED_INT;
            case ShaderElementComponentType::S32: return GL_INT;
            case ShaderElementComponentType::F32: return GL_FLOAT;
            case ShaderElementComponentType::F64: return GL_DOUBLE;
            default: ASSERT_INVALID(); return 0u;
        }
    };

    void VertexBufferObject::Initialize()
    {
        ASSERT(!IsValid());

        glGenBuffers(1, &m_ID);

        ASSERT(IsValid());

        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, m_DataMode == BufferDataMode::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    void VertexBufferObject::DeInitialize()
    {
        if (IsValid())
        {
            m_VertexData.Release();

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &m_ID);
            m_ID = 0;
        }
    }

    inline void VertexBufferObject::UpdateInputToGPU(const VertexInput& vInput, s32 vOverridedOffset /*= -1*/) const
    {
        glEnableVertexAttribArray(vInput.location);

        const u32 offset = vOverridedOffset == -1 ? vInput.offset : vOverridedOffset;

        const GLenum glElementType = GetGLElementType(vInput.componentType);
        // NOTE(jserrano): add normalization as parameter if needed
        glVertexAttribPointer(vInput.location, vInput.nbComponents, glElementType, GL_FALSE, m_VertexStorageSize, reinterpret_cast<void*>(offset));

        glVertexAttribDivisor(vInput.location, vInput.perInstance ? 1 : 0);

#ifdef _DEBUG
        const GLenum errorCode = glGetError();
        const char* errorName = OpenGLUtils::GetErrorName(errorCode);
        ASSERT(errorCode == GL_NO_ERROR || errorName);
#endif
    }

    inline void VertexBufferObject::UpdateInputsToGPU() const
    {
        UpdateVertexStorageSize();

        ASSERT(m_Technique);
        const std::vector<VertexInput>& vInputs = m_Technique->GetInputs();

        if (m_InputLocations.empty())
        {
            for (const auto& vInput : vInputs)
                UpdateInputToGPU(vInput);
        }
        else
        {
            u32 offset = 0;

            for (const auto& location : m_InputLocations)
            {
                const VertexInput& vInput = vInputs[location];
                UpdateInputToGPU(vInput, offset);
                offset += vInput.GetStorageSize();
            }
        }
    }

    void VertexBufferObject::UpdateToGPU()
    {
        ASSERT(IsValid());

        if (IsEmpty())
            return;

        s32 currentBoundVBO = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBoundVBO);

        if (m_Dirty || static_cast<u32>(currentBoundVBO) != m_ID)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_ID);
            UpdateInputsToGPU();
        }
        
        glBufferData(GL_ARRAY_BUFFER, m_VertexData.GetSize(), m_VertexData.GetRawData(), m_DataMode == BufferDataMode::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

#ifdef _DEBUG
        const GLenum errorCode = glGetError();
        const char* errorName = OpenGLUtils::GetErrorName(errorCode);
        ASSERT(errorCode == GL_NO_ERROR || errorName);
#endif
    }
}

#endif // OPENGL_GRAPHICS