#pragma once

#include "pch.h"
#include "Core/Config/EngineConfig.h"
#include "Core/Config/ProjectConfig.h"

namespace ZeroEngine
{
    struct GlobalData
    {
        EngineConfig engineCfg;
    };

    class GlobalDataManager
    {
    public:
        static bool Create(const std::filesystem::path& engineWorkDir);
        static void Destroy();
        static std::shared_ptr<GlobalDataManager> GetInstance();

        GlobalData* GetGlobalDataRef() const;
        bool InitGlobalData();
        bool SaveGlobalData() const;

    private:
        static std::shared_ptr<GlobalDataManager> sInstance;
        std::unique_ptr<GlobalData> mpGlobalData;
    };
}
