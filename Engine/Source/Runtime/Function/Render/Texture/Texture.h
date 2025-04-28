#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class Texture
    {
    public:
        Texture(const std::string& path);
        Texture(const std::shared_ptr<TextureData>& data);
        Texture(const std::vector<std::string>& faces);
        Texture(const std::shared_ptr<CubeMapData>& data);
        virtual ~Texture();

        uint32_t GetID() const;

    public:
        int mWidth = 0;
        int mHeight = 0;
        std::string mPath;
        TextureType mType;

    private:
        uint32_t mID;
    };
} // ZeroEngine
