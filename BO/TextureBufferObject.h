//#pragma once
//
//#include "Graphic/ShaderCore/BufferObject.h"
//#include "Core/Types.h"
//
//#include <vector>
//#include "glad/glad.h"
//
//namespace Framework
//{
//    class TextureBufferObject : public BufferObject
//    {
//        friend class SpriteRenderer;
//
//    public:
//        TextureBufferObject(
//            uint32 shaderLocation,
//            uint32 textureSlot,
//            uint32 maxBufferSizeInBytes,
//            const char* uniformOffsetName,
//            const char* unfiformSamplerName
//        )
//            : m_TextureSlot(textureSlot)
//            , m_MaxSize(maxBufferSizeInBytes)
//            , m_Offset(0)
//            , m_OffsetLocation(glGetUniformLocation(shaderLocation, uniformOffsetName))
//        {
//            m_Location = glGetUniformLocation(shaderLocation, unfiformSamplerName);
//            glGenBuffers(1, &m_ID);
//            glBindBuffer(GL_TEXTURE_BUFFER, m_ID);
//            glBufferData(GL_TEXTURE_BUFFER, m_MaxSize, NULL, GL_STREAM_DRAW);
//            glGenTextures(1, &m_TextureLocation);
//            glBindBuffer(GL_TEXTURE_BUFFER, m_TextureSlot);
//        }
//
//    private:
//        void Clean()
//        {
//            glBindBuffer(GL_TEXTURE_BUFFER, m_ID);
//            glActiveTexture(GL_TEXTURE0 + m_TextureSlot);
//            glBindTexture(GL_TEXTURE_BUFFER, m_TextureLocation);
//            glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_ID);
//            glUniform1i(m_Location, m_TextureSlot);
//        }
//
//    public:
//        inline uint32 MaxSize() const           { return m_MaxSize; }
//        inline uint32 Offset() const            { return m_Offset; }
//        inline uint32 OffsetLocation() const    { return m_OffsetLocation; }
//
//    private:
//        uint32 m_Location;
//        uint32 m_TextureLocation;
//        uint32 m_TextureSlot;
//        uint32 m_MaxSize;
//        uint32 m_Offset;
//        uint32 m_OffsetLocation;
//    };
//}