cmake_minimum_required(VERSION 3.19)

project(
        MetaParser
        LANGUAGES CXX C
        DESCRIPTION "用libclang获取源文件AST信息, 供反射系统使用"
        VERSION 0.0.1
)

file(GLOB_RECURSE HEADERS "*.h" "*.hpp")
file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(MetaParser ${HEADERS} ${SOURCES})

target_include_directories(MetaParser
        PRIVATE "ThirdParty/Include/LLVM"
        PRIVATE "ThirdParty/Include"
        PRIVATE "."
)

target_link_libraries(MetaParser
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Library/LLVM/libclang.lib"
)

target_precompile_headers(MetaParser PRIVATE "Common/pch.h")
target_compile_features(MetaParser PRIVATE cxx_std_20)
