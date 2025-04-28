#include "FBOManager.h"

#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    void FBOManager::Create()
    {
        GetInstance();
    }

    void FBOManager::Destroy()
    {
        for (auto& [name, FBO]: GetInstance().mNameToFBO)
        {
            FBO.reset();
        }
    }

    FBOManager& FBOManager::GetInstance()
    {
        static FBOManager sInstance;
        return sInstance;
    }

    void FBOManager::SwitchFBO(const std::string& name)
    {
        if (name == ZERO_BUILTIN_FBO_SCREEN)
        {
            RHI::GetInstance().SwitchFrameBuffer(UINT32_MAX);
            return;
        }

        if (auto FBO = GetFBOByName(name);
            FBO != nullptr)
        {
            RHI::GetInstance().SwitchFrameBuffer(FBO->mID);
        }
        else
        {
            LOG_ERROR(std::format("[{}] FBO \"{}\" Not Found!!!", __FUNCTION__, name));
        }
    }

    void FBOManager::CreateFBO(const std::string& name, FrameBufferType type, uint32_t width, uint32_t height)
    {
        CreateFBO(name, type, {}, width, height);
    }

    void FBOManager::CreateFBO(const std::string& name, FrameBufferType type, const ClearInfo& clearInfo,
                               uint32_t width, uint32_t height)
    {
        if (mNameToFBO.contains(name))
        {
            LOG_WARN(std::format("[{}] Try to create a FBO already exists: \"{}\" !!!", __FUNCTION__, name));
        }
        else
        {
            mNameToFBO.insert({
                name,
                RHI::GetInstance().CreateFBO(type, width, height, clearInfo)
            });
        }
    }

    void FBOManager::RecreateAllFitWindowFBO()
    {
        for (auto& [name, FBO]: mNameToFBO)
        {
            if (FBO->mbIsFitWithScreen)
            {
                auto fboType = FBO->mType;
                auto fboClearInfo = FBO->mClearInfo;

                RHI::GetInstance().DeleteFBO(FBO);
                FBO.reset();
                FBO = RHI::GetInstance().CreateFBO(fboType, 0, 0, fboClearInfo); // TODO: 这里宽度高度可能要改成屏幕的
            }
        }
    }

    std::shared_ptr<FrameBufferObject> FBOManager::GetFBOByName(const std::string& name)
    {
        if (const auto it = mNameToFBO.find(name);
            it != mNameToFBO.end())
        {
            return it->second;
        }

        return nullptr;
    }
} // ZeroEngine
