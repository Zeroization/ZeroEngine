#include "Texture.h"

#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    Texture::Texture(const std::string& path)
        : mPath(path), mType(TextureType::Single2D)
    {
        mID = RHI::GetInstance().LoadTexture(path, mWidth, mHeight, false);
    }

    Texture::Texture(const std::shared_ptr<TextureData>& data)
        : mWidth(data->width), mHeight(data->height), mType(TextureType::Single2D)
    {
        mID = RHI::GetInstance().CreateTexture(data);
    }

    Texture::Texture(const std::vector<std::string>& faces)
        : mType(TextureType::CubeMap)
    {
        mID = RHI::GetInstance().LoadCubeMap(faces);
    }

    Texture::Texture(const std::shared_ptr<CubeMapData>& data)
        : mType(TextureType::CubeMap)
    {
        mID = RHI::GetInstance().CreateCubeMap(data);
    }

    Texture::~Texture()
    {
        RHI::GetInstance().DeleteTexture(mID);
    }

    uint32_t Texture::GetID() const
    {
        return mID;
    }
} // ZeroEngine
