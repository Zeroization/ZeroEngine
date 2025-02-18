#pragma once

#include <spdlog/spdlog.h>

namespace ZeroEngine
{
    class Logger
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger> &GetLogger() { return CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> CoreLogger;
    };
}

#define LOG_DEBUG(...)      ZeroEngine::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)       ZeroEngine::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       ZeroEngine::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      ZeroEngine::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)   ZeroEngine::Logger::GetLogger()->critical(__VA_ARGS__)
