#pragma once

#include <glad/glad.h>

#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504

namespace OpenGLUtils
{
    constexpr const char* GetErrorName(GLenum errorCode)
    {
        switch( errorCode )
        {
            case GL_INVALID_ENUM:                  return ( "GL_INVALID_ENUM" ); break;
            case GL_INVALID_VALUE:                 return ( "GL_INVALID_VALUE" ); break;
            case GL_INVALID_OPERATION:             return ( "GL_INVALID_OPERATION" ); break;
            case GL_STACK_OVERFLOW:                return ( "GL_STACK_OVERFLOW" ); break;
            case GL_STACK_UNDERFLOW:               return ( "GL_STACK_UNDERFLOW" ); break;
            case GL_OUT_OF_MEMORY:                 return ( "GL_OUT_OF_MEMORY" ); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: return ( "GL_INVALID_FRAMEBUFFER_OPERATION" ); break;
        }
        return nullptr;
    }
}