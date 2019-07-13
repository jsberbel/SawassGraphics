#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include "Graphic/Shader/Shader.h"

#include <glad/glad.h>

namespace Framework
{
    inline constexpr GLuint GetOpenGLShaderType(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex   : return GL_VERTEX_SHADER;
            case ShaderType::Fragment : return GL_FRAGMENT_SHADER;
            case ShaderType::Geometry : return GL_GEOMETRY_SHADER;
            //...
            default: ASSERT_INVALID(); return 0u;
        }
    }

    void Shader::CreateAndCompile()
    {
        ASSERT(m_Type != ShaderType::Undefined && !m_SourceCode.empty());

        m_ID = glCreateShader(GetOpenGLShaderType(m_Type));
        const GLchar* sourceCodePtr = m_SourceCode.c_str();
        glShaderSource(m_ID, 1, &sourceCodePtr, nullptr);
        glCompileShader(m_ID);

        GLint success;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(m_ID, 512, nullptr, infoLog);
            ALERT("GraphicsAPI", "Shader compilation failed: %s", infoLog);
        }
    }

    void Shader::Free()
    {
        glDeleteShader(m_ID);
        m_ID = 0;
    }
}

#endif