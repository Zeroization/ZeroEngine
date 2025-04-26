#pragma once

#include "pch.h"
#include "FrameBufferObject.h"

// 默认屏幕FBO, id: UINT32_MAX
#define ZERO_BUILTIN_FBO_SCREEN "Builtin_Screen"

namespace ZeroEngine
{
    class FBOManager
    {
    public:
        static void Create();
        static void Destroy();
        static FBOManager& GetInstance();

        FBOManager() = default;
        virtual ~FBOManager() = default;
        FBOManager(const FBOManager&) = delete;
        FBOManager(FBOManager&&) = delete;
        FBOManager& operator=(const FBOManager&) = delete;
        FBOManager& operator=(FBOManager&&) = delete;

        void SwitchFBO(const std::string& name);
        void CreateFBO(const std::string& name, FrameBufferType type, uint32_t width = 0, uint32_t height = 0);
        void CreateFBO(const std::string& name, FrameBufferType type, const ClearInfo& clearInfo, uint32_t width = 0,
                       uint32_t height = 0);
        void RecreateAllFitWindowFBO();
        std::shared_ptr<FrameBufferObject> GetFBOByName(const std::string& name);

    private:
        std::map<std::string, std::shared_ptr<FrameBufferObject>> mNameToFBO;
    };
} // ZeroEngine
