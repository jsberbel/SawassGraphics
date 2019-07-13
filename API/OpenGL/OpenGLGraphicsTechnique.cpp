#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include "Graphic/Shader/GraphicsTechnique.h"
#include "Graphic/Shader/Shader.h"

#include <glad/glad.h>

namespace Framework
{
    void GraphicsTechnique::BuildAndLink()
    {
        ASSERT(!m_Shaders.empty());

        m_ID = glCreateProgram();

        for (const auto& shader : m_Shaders)
            glAttachShader(m_ID, shader->GetID());

        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
            ALERT("GraphicsAPI", "Shader linking failed: %s", infoLog);
        }
    }
}

#endif // OPENGL_GRAPHICS