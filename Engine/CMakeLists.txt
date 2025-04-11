cmake_minimum_required(VERSION 3.19)

set(ENGINE_ROOT_DIR ${ZERO-ENGINE_ROOT_DIR}/Engine)
set(ENGINE_ASSET_DIR ${ENGINE_ROOT_DIR}/BuiltinAssets)
set(THIRD_PARTY_DIR ${ENGINE_ROOT_DIR}/ThirdParty)

# 复制BuiltinAssets文件
file(GLOB_RECURSE BUILTIN_RESOURCE_FILES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/BuiltinAssets/
        ${CMAKE_CURRENT_SOURCE_DIR}/BuiltinAssets/*
)
foreach (BUILTIN_RES_FILE ${BUILTIN_RESOURCE_FILES})
    get_filename_component(TARGET_DIR ${BUILTIN_RES_FILE} DIRECTORY)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/BuiltinAssets/${BUILTIN_RES_FILE}
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/BuiltinAssets/${TARGET_DIR})
endforeach ()

#### TODO: 编译Shader, Slang -> glsl(ogl), spv(ogl/vk), hlsl(dx12)
#### 编译第三方库
add_subdirectory(ThirdParty)
#### 编译本体
add_subdirectory(Source/Runtime)
add_subdirectory(Source/Editor)
# 先编译本体才能生成反射代码
add_subdirectory(Source/MetaParser)

set(CODEGEN_TARGET ZeroEnginePreCompile)
include(Source/Precompile/precompile.cmake)

add_dependencies(ZeroEngineRuntime ${CODEGEN_TARGET})
add_dependencies(${CODEGEN_TARGET} MetaParser)
