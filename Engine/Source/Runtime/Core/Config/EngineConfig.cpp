#include "_generated/reflection/EngineConfig.reflgen.h"
#include "Core/FileSystem/FileSystem.h"

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
                return cfgJson.get<EngineConfig>();
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
        nlohmann::json jsonData = engineCfg;
        std::string content = jsonData.dump(4);

        auto configDirPath = FileSystem::GetWorkingDir() / "Source/Editor/Configs";
        if (!FileSystem::Exists(configDirPath) && !FileSystem::CreateDir(configDirPath))
        {
            return false;
        }

        return FileSystem::WriteText(configDirPath / "EngineConfig.json", content);
    }
}
