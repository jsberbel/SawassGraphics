#include "Precompiled.h"

#include "Graphic/Content/BitmapFont.h"
#include "Core/Serialization/JsonCppUtils.h"
#include "Engine/Engine.h"

namespace Framework
{
    TextCharacterData::TextCharacterData()
        : m_UV{
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f } }
        , m_width(1.0f)
        , m_height(1.0f)
        , m_xoffset(0.0f)
        , m_yoffset(0.0f)
        , m_xadvance(0.0f)
    {}

    BitmapFont::BitmapFont(const std::string& filePath)
        : Resource(filePath)
        , m_DefaultLineSpacing(0.0f)
    {
        Load();
    }

    BitmapFont::~BitmapFont()
    {      
    }

    const TextCharacterData* BitmapFont::GetTextCharacterData(unsigned int index)
    {
        TextCharacterData* result = nullptr;

        std::map<unsigned int, TextCharacterData>::iterator it;
        it = m_charactersData.find(index);

        if (it != m_charactersData.end())
        {
            result = &it->second;
        }

        return result;
    }

    void BitmapFont::Load()
    {
        Json::Value fontJson = Json::JsonUtils::OpenAndParseJsonFromFile( GetFilePath() );

        float textureWidth = fontJson["common"]["scaleW"].asFloat();
        float textureHeight = fontJson["common"]["scaleH"].asFloat();

        unsigned int count = fontJson["chars"]["count"].asUInt();

        m_DefaultLineSpacing = fontJson["common"]["lineHeight"].asFloat();

        for( unsigned int i = 0; i < count; ++i )
        {
            Json::Value charJson = fontJson["chars"]["char"][i];

            TextCharacterData charData;

            charData.m_width = charJson["width"].asFloat();
            charData.m_height = charJson["height"].asFloat();
            charData.m_xoffset = charJson["xoffset"].asFloat();
            charData.m_yoffset = charJson["yoffset"].asFloat();
            charData.m_xadvance = charJson["xadvance"].asFloat();

            float charX = charJson["x"].asFloat();
            float charY = charJson["y"].asFloat();

            charData.m_UV[0] = charX / textureWidth;
            charData.m_UV[1] = charY / textureHeight;

            charData.m_UV[2] = ( charX + charData.m_width ) / textureWidth;
            charData.m_UV[3] = charY / textureHeight;

            charData.m_UV[4] = ( charX + charData.m_width ) / textureWidth;
            charData.m_UV[5] = ( charY + charData.m_height ) / textureHeight;

            charData.m_UV[6] = charX / textureWidth;
            charData.m_UV[7] = ( charY + charData.m_height ) / textureHeight;

            unsigned int id = charJson["id"].asUInt();
            std::pair<unsigned int, TextCharacterData> myChar( id, charData );

            m_charactersData.insert( myChar );
        }
    }

#ifdef _DEBUG
    float BitmapFont::GetUsedRam() const
    {
        return static_cast<float>(sizeof(m_charactersData)) / 1024.0f;
    }
#endif
}