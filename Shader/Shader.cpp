/*
 * Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#include "Graphic/Shader/Shader.h"

#include "Core/Utils/FileUtils.h"
#include "Core/Utils/StringUtils.h"

namespace Framework
{
    inline ShaderType RetrieveShaderSourceType(const std::string& fileName)
    {
        if (StringUtils::StringEndsWith(fileName, ".vs"))
        {
            return ShaderType::Vertex;
        }
        else if (StringUtils::StringEndsWith(fileName, ".fs"))
        {
            return ShaderType::Fragment;
        }
        else if (StringUtils::StringEndsWith(fileName, ".gs"))
        {
            return ShaderType::Geometry;
        }

        ASSERT_INVALID();
        return ShaderType::Undefined;
    }

    Shader::Shader(const std::string& filePath)
        : Resource(filePath)
        , m_SourceCode  (FileUtils::ReadTextFile(filePath.c_str()))
        , m_Type        (RetrieveShaderSourceType(filePath))
        , m_ID          (0)
    {
        CreateAndCompile();
    }

    Shader::~Shader()
    {
        Free();
        m_Type = ShaderType::Undefined;
        m_SourceCode.clear();
    }
}
