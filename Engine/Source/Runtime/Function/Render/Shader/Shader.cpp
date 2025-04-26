#include "Shader.h"

#include "Core/FileSystem/FileSystem.h"
#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    std::vector<std::shared_ptr<ShaderReference>> Shader::mLoadedShaders;

    Shader::Shader(const std::string& path, FrameBufferType type)
    {
        name = std::filesystem::path(path).filename().string();
        for (auto& shaderRef: mLoadedShaders)
        {
            if (path == shaderRef->path && type == shaderRef->targetFrameBufferTy)
            {
                reference = shaderRef;
                break;
            }
        }
        if (reference == nullptr)
        {
            reference = RHI::GetInstance().LoadAndSetupShader(path, type);
            reference->path = path;
            mLoadedShaders.push_back(reference);
        }
    }

    Shader::Shader(const std::string& path, const std::string& shaderCode, FrameBufferType type)
    {
        name = std::filesystem::path(path).filename().string();
        for (auto& shaderRef: mLoadedShaders)
        {
            if (path == shaderRef->path && type == shaderRef->targetFrameBufferTy)
            {
                reference = shaderRef;
                break;
            }
        }
        if (reference == nullptr)
        {
            reference = RHI::GetInstance().SetupShader(path, shaderCode, type);
            reference->path = path;
            mLoadedShaders.push_back(reference);
        }
    }

    Shader::~Shader()
    {
        // TODO: debug的时候看一下
        LOG_DEBUG(
            std::format("[{}] Shader dereference when ref_count = {}, should be 2?", __FUNCTION__, reference.use_count()
            ));
        if (reference.use_count() == 2)
        {
            size_t pos = SIZE_MAX;
            for (size_t i = 0; i < mLoadedShaders.size(); ++i)
            {
                if (mLoadedShaders[i]->id == reference->id)
                {
                    pos = i;
                    break;
                }
            }
            if (pos != SIZE_MAX)
            {
                mLoadedShaders.erase(mLoadedShaders.begin() + pos);
                RHI::GetInstance().DeleteShader(reference->id);
                reference.reset();
            }
        }
    }

    void Shader::Use() const
    {
        RHI::GetInstance().UseShader(reference->id);
    }

    uint32_t Shader::GetID() const
    {
        return reference->id;
    }

    LightType Shader::GetLightType() const
    {
        return reference->info.lightType;
    }

    ShadowType Shader::GetShadowType() const
    {
        return reference->info.shadowType;
    }
} // ZeroEngine
