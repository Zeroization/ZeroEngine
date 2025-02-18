cmake_minimum_required(VERSION 3.19)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project(
        ZeroEngine
        LANGUAGES CXX C
        DESCRIPTION "一个短小精悍的游戏引擎(希望如此)"
        VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)

set(ZERO-ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX  ${ZERO-ENGINE_ROOT_DIR}/bin)
set(BINARY_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/)

# 添加子模块
add_subdirectory(Engine)