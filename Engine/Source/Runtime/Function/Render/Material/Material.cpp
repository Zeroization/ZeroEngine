#include "Material.h"
#include "MaterialData.h"
#include "Function/Render/RHI/RHI.h"
#include "Function/Render/Shader/Shader.h"
#include "Function/Render/Texture/Texture.h"
#include "Resource/ResourceSystem.h"

namespace ZeroEngine
{
    Material::Material(MaterialStruct* matStruct)
    {
        ZERO_CORE_ASSERT(matStruct != nullptr, "matStruct shouldn't be null");

        mName = matStruct->propName;
        mPath = matStruct->path;
        mType = matStruct->type;
        mData = std::make_shared<MaterialData>(matStruct->type);

        CopyMaterialStructTOMaterialData(matStruct, mData);

        if (mType == MaterialType::Forward)
        {
            mShader = std::make_shared<Shader>(matStruct->shaderPath, matStruct->shaderCode, FrameBufferType::Normal);
            mRenderQueue = static_cast<int>(mShader->reference->info.stateSet.renderQueueType);
            mRenderQueueTy = mShader->reference->info.stateSet.renderQueueType;
            RHI::GetInstance().SetupMaterial(this);
        }
        else if (mType == MaterialType::Deferred)
        {
            mRenderQueue = static_cast<int>(RenderQueueType::Deferred);
            mRenderQueueTy = RenderQueueType::Deferred;
        }
    }

    Material::~Material()
    {
        mData.reset();
        mShader.reset();
    }

    void Material::Use()
    {
        mData->Use();
        mShader->Use();
    }

    void Material::SetProperties()
    {
        if (mData->mIsDirty)
        {
            for (auto& [propName, propTyAndData]: mData->mDatas)
            {
                switch (propTyAndData.first)
                {
                    case ShaderPropertyType::Bool:
                        SetScalarProp(propName, propTyAndData.second.cast<bool>());
                        break;
                    case ShaderPropertyType::Int:
                        SetScalarProp(propName, propTyAndData.second.cast<int32_t>());
                        break;
                    case ShaderPropertyType::UInt:
                        SetScalarProp(propName, propTyAndData.second.cast<uint32_t>());
                        break;
                    case ShaderPropertyType::Float:
                        SetScalarProp(propName, propTyAndData.second.cast<float>());
                        break;
                    case ShaderPropertyType::Vec2:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::vec2>());
                        break;
                    case ShaderPropertyType::Vec3:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::vec3>());
                        break;
                    case ShaderPropertyType::Vec4:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::vec4>());
                        break;
                    case ShaderPropertyType::IVec2:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::ivec2>());
                        break;
                    case ShaderPropertyType::IVec3:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::ivec3>());
                        break;
                    case ShaderPropertyType::IVec4:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::ivec4>());
                        break;
                    case ShaderPropertyType::UVec2:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::uvec2>());
                        break;
                    case ShaderPropertyType::UVec3:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::uvec3>());
                        break;
                    case ShaderPropertyType::UVec4:
                        SetVectorProp(propName, propTyAndData.second.cast<glm::uvec4>());
                        break;
                    case ShaderPropertyType::Mat2x2:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat2>());
                        break;
                    case ShaderPropertyType::Mat2x3:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat2x3>());
                        break;
                    case ShaderPropertyType::Mat2x4:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat2x4>());
                        break;
                    case ShaderPropertyType::Mat3x2:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat3x2>());
                        break;
                    case ShaderPropertyType::Mat3x3:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat3>());
                        break;
                    case ShaderPropertyType::Mat3x4:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat3x4>());
                        break;
                    case ShaderPropertyType::Mat4x2:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat4x2>());
                        break;
                    case ShaderPropertyType::Mat4x3:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat4x3>());
                        break;
                    case ShaderPropertyType::Mat4x4:
                        SetMatrixProp(propName, propTyAndData.second.cast<glm::mat4>());
                        break;
                    default:
                        LOG_ERROR(
                            std::format("[{}] Unsupported type: {}", __FUNCTION__, static_cast<uint16_t>(propTyAndData.
                                first)));
                        break;
                }
            }

            mData->mIsDirty = false;
        }

        uint32_t textureNum = mData->mTextures.size();
        for (uint32_t i = 0; i < textureNum; ++i)
        {
            if (mData->mTextures[i].second->mType == TextureType::Single2D)
            {
                SetTextureProp(mData->mTextures[i].first, mData->mTextures[i].second->GetID(), i, false);
            }
            else if (mData->mTextures[i].second->mType == TextureType::CubeMap)
            {
                SetCubemapProp(mData->mTextures[i].first, mData->mTextures[i].second->GetID(), i, false);
            }
        }
        mTextureIdx = textureNum;
    }

    int Material::GetRenderQueue() const
    {
        return mRenderQueue;
    }

    RenderQueueType Material::GetRenderQueueType() const
    {
        return mRenderQueueTy;
    }

    void Material::CopyMaterialStructTOMaterialData(MaterialStruct* matStruct,
                                                    const std::shared_ptr<MaterialData>& data)
    {
        ZERO_CORE_ASSERT(matStruct != nullptr, "matStruct shouldn't be null");

        data->mDatas = matStruct->properties;
        for (const auto& textureStruct: matStruct->textures)
        {
            data->mTextures.push_back({textureStruct->propName, std::make_shared<Texture>(textureStruct->data)});
        }
        for (const auto& cubeMapStruct: matStruct->cubeMaps)
        {
            data->mTextures.push_back({cubeMapStruct->propName, std::make_shared<Texture>(cubeMapStruct->data)});
        }
    }

    void Material::SetScalarProp(const std::string& propName, bool value)
    {
        RHI::GetInstance().SetScalarProp(this, propName, value);
    }

    void Material::SetScalarProp(const std::string& propName, float value)
    {
        RHI::GetInstance().SetScalarProp(this, propName, value);
    }

    void Material::SetScalarProp(const std::string& propName, int32_t value)
    {
        RHI::GetInstance().SetScalarProp(this, propName, value);
    }

    void Material::SetScalarProp(const std::string& propName, uint32_t value)
    {
        RHI::GetInstance().SetScalarProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::vec2& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::vec3& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::vec4& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::ivec2& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::ivec3& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::ivec4& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::uvec2& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::uvec3& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetVectorProp(const std::string& propName, const glm::uvec4& value)
    {
        RHI::GetInstance().SetVectorProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat2& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat2x3& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat2x4& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat3& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat3x2& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat3x4& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat4& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat4x2& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetMatrixProp(const std::string& propName, const glm::mat4x3& value)
    {
        RHI::GetInstance().SetMatrixProp(this, propName, value);
    }

    void Material::SetTextureProp(const std::string& propName, uint32_t texID, uint32_t texIdx, bool isBuffer)
    {
        RHI::GetInstance().SetTextureProp(this, propName, texID, texIdx, isBuffer);
    }

    void Material::SetCubemapProp(const std::string& propName, uint32_t texID, uint32_t texIdx, bool isBuffer)
    {
        RHI::GetInstance().SetCubemapProp(this, propName, texID, texIdx, isBuffer);
    }
} // ZeroEngine
