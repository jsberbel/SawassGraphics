#pragma once

#include "Core/Types.h"
#include "Engine/Resource.h"

#include <string>

namespace Framework
{
    enum class ShaderType
    {
        Undefined   ,
        Vertex      ,
        Fragment    ,
        Geometry    ,
        //...
        Max
    };

    class Shader : public Resource
    {
        friend class ResourceManager;

    public:
        ~Shader() override;

        u32                 GetID()          const { return m_ID; }
        ShaderType          GetType()        const { return m_Type; };
        const std::string&  GetSourceCode()  const { return m_SourceCode; };

    private:
        Shader(const std::string& filePath);

        void CreateAndCompile(); // <GraphicsAPI specific>
        void Free();             // <GraphicsAPI specific>

    private:
        u32      m_ID;
        ShaderType  m_Type;
        std::string m_SourceCode;
    };
}