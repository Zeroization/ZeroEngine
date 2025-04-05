cmake_minimum_required(VERSION 3.19)

set(TARGET_NAME ZeroEngineRuntime)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.hpp")

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(ZEROENGINE_RUNTIME_HEADS "${HEADER_FILES}" PARENT_SCOPE)

target_precompile_headers(${TARGET_NAME} PUBLIC pch.h)
target_include_directories(${TARGET_NAME} PUBLIC .)
target_link_libraries(${TARGET_NAME}
        PUBLIC spdlog::spdlog_header_only
        PUBLIC EnTT::EnTT
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC nfd
        PUBLIC glad
        PUBLIC glfw
        PUBLIC ImGUI
)

target_compile_definitions(${TARGET_NAME}
        PUBLIC $<$<CONFIG:Debug>:ZERO_DEBUG_ENABLE>
        PUBLIC ZERO_ASSET_DIR="${ENGINE_ASSET_DIR}"
        # EnTT
        PUBLIC ENTT_USE_ATOMIC
)