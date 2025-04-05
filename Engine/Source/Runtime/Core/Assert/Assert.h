#pragma once

#include "Core/Log/Logger.h"

// debugbreak()
#ifdef ZERO_OS_WINDOWS
    #define ZERO_DEBUG_BREAK __debugbreak()
#elif defined(ZERO_COMPILER_CLANG)
    #define ZERO_DEBUG_BREAK __builtin_debugtrap()
#else
    #define ZERO_DEBUG_BREAK
#endif

// Assertions
#ifdef ZERO_DEBUG_ENABLE
    #define ZERO_ASSERTS_ENABLE
#endif

#ifdef ZERO_ASSERTS_ENABLE
    #define ZERO_CORE_ASSERT_MESSAGE_INTERNAL(message) LOG_CRITICAL(std::format("[{}] Core Assertion Failed: {}", __FUNCTION__, message))
    #define ZERO_ASSERT_MESSAGE_INTERNAL(message) LOG_CRITICAL(std::format("[{}] Assertion Failed: {}", __FUNCTION__, message))
    #define ZERO_CORE_ASSERT(condition, message) { if(!(condition)) { ZERO_CORE_ASSERT_MESSAGE_INTERNAL(message); ZERO_DEBUG_BREAK; } }
    #define ZERO_ASSERT(condition, message) { if(!(condition)) { ZERO_ASSERT_MESSAGE_INTERNAL(message); ZERO_DEBUG_BREAK; } }
#else
    #define ZERO_CORE_ASSERT(condition, message)
    #define ZERO_ASSERT(condition, message)
#endif
