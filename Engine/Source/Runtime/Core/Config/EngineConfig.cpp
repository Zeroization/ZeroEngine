#include "_generated/reflection/EngineConfig.reflgen.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Serialize/SerializeManager.h"

namespace ZeroEngine
{
    EngineConfig EngineConfig::TryGetEngineCfg(const std::filesystem::path& engineCfgDir)
    {
        if (FileSystem::Exists(engineCfgDir / "EngineConfig.json"))
        {
            std::string content;
            if (FileSystem::ReadText(engineCfgDir / "EngineConfig.json", content))
            {
                nlohmann::json cfgJson = nlohmann::json::parse(content);

                auto& seriMgr = SerializeManager::GetInstance();
                if (EngineConfig ret;
                    seriMgr.DeserializeFromJSON<EngineConfig>(cfgJson, ret))
                {
                    return ret;
                }
            }
        }

        LOG_WARN(std::format("[{}] Can't get EngineConfig, use default setting...", __FUNCTION__));

        EngineConfig ret;
        ret.mWindowHeight = 720;
        ret.mWindowWidth = 1280;

        return ret;
    }

    bool EngineConfig::TrySaveEngineCfg(const EngineConfig& engineCfg)
    {
        auto& seriMgr = SerializeManager::GetInstance();
        if (nlohmann::json engineCfgJson;
            seriMgr.SerializeToJSON<EngineConfig>(engineCfgJson, engineCfg))
        {
            std::string content = engineCfgJson.dump(4);

            auto configDirPath = FileSystem::GetWorkingDir() / "Source/Editor/Configs";
            if (!FileSystem::Exists(configDirPath) && !FileSystem::CreateDir(configDirPath))
            {
                LOG_CRITICAL(std::format("[{}] Can't Save engineCfg!", __FUNCTION__));
                return false;
            }

            return FileSystem::WriteText(configDirPath / "EngineConfig.json", content);
        }

        LOG_CRITICAL(std::format("[{}] Can't Serialize engineCfg!", __FUNCTION__));
        return false;
    }
}
