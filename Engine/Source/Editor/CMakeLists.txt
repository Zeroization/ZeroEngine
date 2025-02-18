cmake_minimum_required(VERSION 3.19)

set(TARGET_NAME ZeroEngineEditor)

add_executable(${TARGET_NAME} Main.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ../Runtime/Core)

target_link_libraries(${TARGET_NAME} PUBLIC ZeroEngineRuntime)