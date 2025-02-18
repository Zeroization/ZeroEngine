cmake_minimum_required(VERSION 3.19)

set(ENGINE_ROOT_DIR ${ZERO-ENGINE_ROOT_DIR}/Engine)
set(ENGINE_ASSET_DIR ${ENGINE_ROOT_DIR}/BuiltinAssets)
set(THIRD_PARTY_DIR ${ENGINE_ROOT_DIR}/ThirdParty)

#### TODO:  0. 生成反射代码
#### TODO:  1. 编译Shader, Slang -> glsl(ogl), spv(ogl/vk), hlsl(dx12)
#### 2. 编译第三方库
add_subdirectory(ThirdParty)
#### 3. 编译本体
add_subdirectory(Source/Runtime)
add_subdirectory(Source/Editor)
