#pragma once
#include "pch.h"
#include "Core/Reflection/ReflectionManager.h"

namespace ZeroEngine
{
    ZERO_INTERNAL_CLASS(EngineConfig)
    {
        ZERO_BODY(EngineConfig)

    public:
        static EngineConfig TryGetEngineCfg(const std::filesystem::path& engineCfgDir);
        static bool TrySaveEngineCfg(const EngineConfig& engineCfg);

        explicit EngineConfig() = default;
        virtual ~EngineConfig() = default;

    public:
        /// Engine Setting ==============================
        ZERO_PROPERTY()
        uint32_t mWindowWidth;
        ZERO_PROPERTY()
        uint32_t mWindowHeight;

        /// Editor Setting ==============================
        ZERO_PROPERTY()
        uint32_t mGameViewWidth;
        ZERO_PROPERTY()
        uint32_t mGameViewHeight;
    };
}
