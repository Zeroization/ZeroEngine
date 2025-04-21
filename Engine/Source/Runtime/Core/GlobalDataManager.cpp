#include "GlobalDataManager.h"

#include "FileSystem/FileSystem.h"
#include "Serialize/SerializeManager.h"

namespace ZeroEngine
{
    bool GlobalDataManager::Create(const std::filesystem::path& engineWorkDir)
    {
        FileSystem::SetWorkingDir(engineWorkDir);
        return GetInstance().InitGlobalData();
    }

    void GlobalDataManager::Destroy()
    {
        if (!GetInstance().SaveGlobalData())
        {
            LOG_ERROR(std::format("[{}] Errors occurred when saving datas!!!", __FUNCTION__));
        }

        GetInstance().mpGlobalData.reset();
    }

    GlobalDataManager& GlobalDataManager::GetInstance()
    {
        static GlobalDataManager sInstance;
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
        saveResult &= EngineConfig::TrySaveEngineCfg(GetInstance().mpGlobalData->engineCfg);

        return saveResult;
    }
}
