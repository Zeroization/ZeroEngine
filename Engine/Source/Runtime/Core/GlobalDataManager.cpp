#include "GlobalDataManager.h"

#include "FileSystem/FileSystem.h"

namespace ZeroEngine
{
    std::shared_ptr<GlobalDataManager> GlobalDataManager::sInstance = nullptr;

    bool GlobalDataManager::Create(const std::filesystem::path& engineWorkDir)
    {
        FileSystem::SetWorkingDir(engineWorkDir);

        sInstance = std::make_shared<GlobalDataManager>();
        return sInstance->InitGlobalData();
    }

    void GlobalDataManager::Destroy()
    {
        if (!sInstance->SaveGlobalData())
        {
            LOG_ERROR(std::format("[{}] Errors occurred when saving datas!!!", __FUNCTION__));
        }

        sInstance->mpGlobalData.reset();
        sInstance.reset();
    }

    std::shared_ptr<GlobalDataManager> GlobalDataManager::GetInstance()
    {
        return sInstance;
    }

    GlobalData* GlobalDataManager::GetGlobalDataRef() const
    {
        return mpGlobalData.get();
    }

    bool GlobalDataManager::InitGlobalData()
    {
        bool initResult = true;
        mpGlobalData = std::make_unique<GlobalData>();
        mpGlobalData->engineCfg = EngineConfig::TryGetEngineCfg(FileSystem::GetWorkingDir() / "Source/Editor/Configs");

        return initResult;
    }

    bool GlobalDataManager::SaveGlobalData() const
    {
        bool saveResult = true;
        saveResult &= EngineConfig::TrySaveEngineCfg(sInstance->mpGlobalData->engineCfg);

        return saveResult;
    }
}
