/* 
 * Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#ifdef OPENGL_GRAPHICS

#include <glad/glad.h>

#include "Core/Config.h"
#include "Graphic/GraphicsAPI.h"
#include "Graphic/Window.h"
#include "Graphic/Content/Texture.h"
#include "Graphic/GraphicsConstants.h"

namespace Framework
{
    u32             g_VAO       = 0;
    SDL_GLContext   g_GLContext = nullptr;

    constexpr GLuint GetGLPrimitiveType(PrimitiveType type)
    {
        switch (type)
        {
            case PrimitiveType::TriangleList :  return GL_TRIANGLES;
            case PrimitiveType::LineList     :  return GL_LINES;
            case PrimitiveType::PointList    :  return GL_POINTS;
            case PrimitiveType::TriangleStrip:  return GL_TRIANGLE_STRIP;
            case PrimitiveType::LineStrip    :  return GL_LINE_STRIP;
            case PrimitiveType::TriangleFan  :  return GL_TRIANGLE_FAN;
            // ...
            default: ASSERT_INVALID(); return 0u;
        }
    };

    constexpr GLuint GetGLIndexType(u32 indexSize)
    {
        switch (indexSize)
        {
            case sizeof(u8)  : return GL_UNSIGNED_BYTE;
            case sizeof(u16) : return GL_UNSIGNED_SHORT;
            case sizeof(u32) : return GL_UNSIGNED_INT;
            // ...
            default: ASSERT_INVALID(); return 0u;
        }
    };

    inline u32 CreateVertexArrayObject()
    {
        u32 vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        return vao;
    }

    inline void DestroyVertexArrayObject(u32 vao)
    {
        if (vao != 0)
        {
            glBindVertexArray(0);
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    }

    inline void SetOpenGLContextVersion(u32 majorVersion, u32 minorVersion, SDL_GLprofile glProfile)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, glProfile); // OpenGL core profile - deprecated functions are disabled
    }

    void GraphicsAPI::Initialize(const Config& config, const Window& window)
    {
        SetOpenGLContextVersion(4, 3, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Use hardware acceleration
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Set output to be double buffered
       
        g_GLContext = SDL_GL_CreateContext(window.GetHandle());
        ASSERT_OR_ALERT(g_GLContext != nullptr, SDL_GetError());

        SDL_GL_SetSwapInterval(config.IsVSyncEnabled() ? 1 : 0); // 0 = immediate updates, 1 = updates synchronized with the vertical retrace, -1 = late swap tearing

        const bool gladLoaded = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        ASSERT(gladLoaded);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        LOG_INFO("Graphics", (const char*)(glGetString(GL_VENDOR)));
        LOG_INFO("Graphics", (const char*)(glGetString(GL_RENDERER)));
        LOG_INFO("Graphics", (const char*)(glGetString(GL_VERSION)));

        g_VAO = CreateVertexArrayObject();
    }

    void GraphicsAPI::DeInitialize()
    {
        DestroyVertexArrayObject(g_VAO);

        SDL_GL_DeleteContext(g_GLContext);
        g_GLContext = nullptr;
    }

    template <GLuint type, GLenum usage = GL_STATIC_DRAW>
    static constexpr GLuint
    CreateBuffer(const void *data, GLsizei size)
    {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type,   // type of buffer
                     size,   // size of the buffer content
                     data,   // content of the buffer
                     usage); // usage of the buffer (DYNAMIC -> will change frequently, DRAW -> from CPU to GPU)
        //glBindBuffer(type, 0);
        return buffer;
    }

    void BindVertexArrayObject(u32 vao)
    {
        glBindVertexArray(vao);
    }

    void UnbindVertexArrayObject()
    {
        glBindVertexArray(0);
    }

    void GraphicsAPI::ClearRenderBuffer()
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GraphicsAPI::BindIntUniform(u32 shaderID, const std::string& uniformName, int uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniform1i(uniformLocation, uniformValue); 
    }

    void GraphicsAPI::BindFloatUniform(u32 shaderID, const std::string& uniformName, float uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniform1f(uniformLocation, uniformValue); 
    }

    void GraphicsAPI::BindVectorUniform(u32 shaderID, const std::string& uniformName, const glm::vec2& uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniform2fv(uniformLocation, 1, &uniformValue[0]);
    }

    void GraphicsAPI::BindVectorUniform(u32 shaderID, const std::string& uniformName, const glm::vec3& uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniform3fv(uniformLocation, 1, &uniformValue[0]);
    }

    void GraphicsAPI::BindVectorUniform(u32 shaderID, const std::string& uniformName, const glm::vec4& uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniform4fv(glGetUniformLocation(shaderID, uniformName.c_str()), 1, &uniformValue[0]);
    }

    void GraphicsAPI::BindMatrixUniform(u32 shaderID, const std::string& uniformName, const glm::mat2& uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, &uniformValue[0][0]);
    }

    void GraphicsAPI::BindMatrixUniform(u32 shaderID, const std::string& uniformName, const glm::mat3& uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &uniformValue[0][0]);
    }

    void GraphicsAPI::BindMatrixUniform(u32 shaderID, const std::string& uniformName, const glm::mat4& uniformValue)
    {
        const GLint uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
        ASSERT(uniformLocation != -1);
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &uniformValue[0][0]);
    }

    void GraphicsAPI::BindTextureUniform(u32 shaderID, u32 layerID, const u32 textureID)
    {
        glActiveTexture(GL_TEXTURE0 + layerID);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    s32 GraphicsAPI::GetShaderConstantLocation(u32 shaderID, const std::string& name)
    {
        return glGetUniformLocation(shaderID, name.c_str());
    }

    void GraphicsAPI::SetRasterizedLines(f32 lineWidth)
    {
        glLineWidth(lineWidth);
    }

    void GraphicsAPI::DrawElements(PrimitiveType primitiveType, u32 indexCount, u32 indexTypeSize)
    {
        const GLuint glPrimitiveType = GetGLPrimitiveType(primitiveType);
        const u32 glIndexType     = GetGLIndexType(indexTypeSize);
        glDrawElements(glPrimitiveType, indexCount, glIndexType, nullptr);
    }

    void GraphicsAPI::DrawElementsInstanced(PrimitiveType primitiveType, u32 indexCount, u32 indexTypeSize, u32 nbInstances)
    {
        const GLuint glPrimitiveType = GetGLPrimitiveType(primitiveType);
        const u32 glIndexType     = GetGLIndexType(indexTypeSize);
        glDrawElementsInstanced(glPrimitiveType, indexCount, glIndexType, nullptr, nbInstances);
    }

    void GraphicsAPI::DrawArrays(PrimitiveType primitiveType, u32 startElement, u32 vertexCount)
    {
        const GLuint glPrimitiveType = GetGLPrimitiveType(primitiveType);
        glDrawArrays(glPrimitiveType, startElement, vertexCount);
    }

    void GraphicsAPI::DrawArraysInstanced(PrimitiveType primitiveType, u32 startElement, u32 vertexCount, u32 nbInstances)
    {
        const GLuint glPrimitiveType = GetGLPrimitiveType(primitiveType);
        glDrawArraysInstanced(glPrimitiveType, startElement, vertexCount, nbInstances);
    }

    void GraphicsAPI::OnWindowResized(u32 width, u32 height)
    {
        glViewport(0, 0, width, height);
    }

    void ConfigureTextureAttributes(u32 textureID, const FilterMode& filterMode, const WrapMode& wrapMode)
    {
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

#endif