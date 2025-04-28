#pragma once
#include "pch.h"
#include "Core/Reflection/ReflectionManager.h"

namespace ZeroEngine
{
    ZERO_INTERNAL_CLASS(ProjectConfig)
    {
        ZERO_BODY(ProjectConfig)

    public:
        static ProjectConfig TryGetProjectConfig(const std::filesystem::path& projectCfgDir);
        static bool TrySaveProjectConfig(const ProjectConfig& projCfg);

        explicit ProjectConfig() = default;
        virtual ~ProjectConfig() = default;
    };
}
