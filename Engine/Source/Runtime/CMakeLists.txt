cmake_minimum_required(VERSION 3.19)

set(TARGET_NAME ZeroEngineRuntime)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.hpp")

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_precompile_headers(${TARGET_NAME} PUBLIC pch.h)
target_include_directories(${TARGET_NAME} PUBLIC .)
target_link_libraries(${TARGET_NAME}
        PUBLIC spdlog::spdlog_header_only
        PUBLIC glad
        PUBLIC glfw
        PUBLIC ImGUI
)

target_compile_definitions(${TARGET_NAME} PUBLIC $<$<CONFIG:Debug>:ZERO_ENABLE_DEBUG>)