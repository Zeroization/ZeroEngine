#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace ZeroEngine
{
    std::shared_ptr<spdlog::logger> Logger::CoreLogger;

    void Logger::Init()
    {
        auto ConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Runtime.log", true);

        spdlog::logger Logger("", {ConsoleSink, FileSink});
        CoreLogger = std::make_shared<spdlog::logger>(Logger);
#ifdef ZERO_DEBUG_ENABLE
        CoreLogger->set_level(spdlog::level::debug);
#else
        CoreLogger->set_level(spdlog::level::info);
#endif
    }
}
