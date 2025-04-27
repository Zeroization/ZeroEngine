#pragma once
#include "pch.h"

namespace ZeroEngine
{
    class Texture;

    class MaterialData
    {
    public:
        MaterialData(MaterialType type);
        virtual ~MaterialData();

        void Use() const;
        uint32_t GetID() const;
        std::shared_ptr<Texture> GetTexture(const std::string& name) const;

    public:
        MaterialType mType;
        bool mIsDirty = false;
        bool bIsInitialized = false;

        std::map<std::string, std::pair<ShaderPropertyType, entt::meta_any>> mDatas;
        std::vector<std::pair<std::string, std::shared_ptr<Texture>>> mTextures;

    private:
        uint32_t mID = 0;
    };
} // ZeroEngine
