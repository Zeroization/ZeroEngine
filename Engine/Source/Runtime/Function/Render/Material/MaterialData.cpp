#include "MaterialData.h"

#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine {
    MaterialData::MaterialData(MaterialType type)
        : mType(type)
    {
        if (type == MaterialType::Forward || type == MaterialType::Deferred)
        {
            mID = RHI::GetInstance().CreateMaterialData();
        }
        else if (type == MaterialType::RayTracing)
        {
            ZERO_CORE_ASSERT(false, "TODO...")
        }
    }

    MaterialData::~MaterialData()
    {
        if (mType == MaterialType::Forward || mType == MaterialType::Deferred)
        {
            RHI::GetInstance().DeleteMaterialData(mID);
        }
        else if (mType == MaterialType::RayTracing)
        {
            ZERO_CORE_ASSERT(false, "TODO...")
        }

        for (auto& [name, texture] : mTextures)
        {
            texture.reset();
        }
    }

    void MaterialData::Use() const
    {
        RHI::GetInstance().UseMaterialData(mID);
    }

    uint32_t MaterialData::GetID() const
    {
        return mID;
    }

    std::shared_ptr<Texture> MaterialData::GetTexture(const std::string& name) const
    {
        for (auto& [_name, _texture] : mTextures)
        {
            if (name == _name)
            {
                return _texture;
            }
        }
    }
} // ZeroEngine