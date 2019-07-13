#pragma once

#include "Core/Serialization/ISerializableObject.h"
#include "Engine/Resource.h"
#include "Graphic/Util/SpriteRect.h"

namespace Framework
{
    struct TextCharacterData
    {
        TextCharacterData();

        SpriteRect m_UV;
        float m_width;
        float m_height;
        float m_xoffset;
        float m_yoffset;
        float m_xadvance;
    };

    class BitmapFont : public Resource
    {
        friend class ResourceManager;

    public:
        virtual ~BitmapFont();

        const TextCharacterData* GetTextCharacterData(unsigned int index);
        const float GetDefaultLineSpacing() const { return m_DefaultLineSpacing; }

        DEBUG_EXP( float GetUsedRam() const override; )

    private:
        explicit BitmapFont(const std::string& filePath);

        void Load();

    protected:
        float m_DefaultLineSpacing;
        std::map<unsigned int, TextCharacterData> m_charactersData;
    };
}
