#pragma once
// pre-compiled header file

#pragma region 平台侦测 Platform Detection
// OS定义==============================
// ZERO_OS_WINDOWS: Windows系统
// ZERO_OS_LINUX:   Linux系统     TODO
#if defined(_WIN32) || defined(_WIN64)
    #define ZERO_OS_WINDOWS
#elif defined(__linux__)
    #define ZERO_OS_LINUX
#else
    #error "Pch Error: Unsupport OS!!!!!"
#endif

// 平台定义=============================
// ZERO_PLATFORM_DESKTOP: 桌面端
#if defined(ZERO_OS_WINDOWS) || defined(ZERO_OS_LINUX)
    #define ZERO_PLATFORM_DESKTOP
#else
    #error "Pch Error: Unsupport platform!!!!!"
#endif

// 图形API定义=========================
// 0: OpenGL
// 1: Vulkan    TODO
// 2: D3D12     TODO
#if defined(ZERO_PLATFORM_DESKTOP)
    #define ZERO_GRAPHIC_SELECTOR 0
#endif

#if ZERO_GRAPHIC_SELECTOR == 0
    #define ZERO_GRAPHIC_OPENGL
#elif ZERO_GRAPHIC_SELECTOR == 1
    #define ZERO_GRAPHIC_VULKAN
#elif ZERO_GRAPHIC_SELECTOR == 2
    #define ZERO_GRAPHIC_D3D12
#else
    #error "Pch Error: Unknown Graphic API selector"
#endif

// 图形API兼容性检查=====================
// Windows系统:   OpenGL, Vulkan, D3D12
// Linux系统:     OpenGL, Vulkan
#if defined(ZERO_OS_LINUX) && defined(ZERO_GRAPHIC_D3D12)
    #error "Pch Error: Linux OS & D3D12 Not Supported!!!!!"
#endif

// 编译器检测===========================
#if defined(__GNUC__)
    #if defined(__clang__)
        #define ZERO_COMPILER_CLANG
    #else
        #define ZERO_COMPILER_GCC
    #endif
#elif defined(_MSC_VER)
    #define ZERO_COMPILER_MSVC
#endif

#ifdef ZERO_COMPILER_MSVC
    #define ZERO_FORCE_INLINE __forceinline
    #define ZERO_EXPLICIT_STATIC static
#elif defined(__GNUC__)
    #define ZERO_FORCE_INLINE __attribute((always_inline)) inline
    #define ZERO_EXPLICIT_STATIC
#else
    #define ZERO_FORCE_INLINE inline
    #define ZERO_EXPLICIT_STATIC
#endif
#pragma endregion

// 是否启用编辑器模式, 后续用于游戏打包=====
#define ZERO_EDITOR

// 第三方库===========================
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

// 标准库=============================
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <functional>
#include <cstdint>

// 辅助模板===========================
// 禁止模板参数类型自动推导, 让用户显式指定参数
template <typename T>
struct NoDeduce
{
    using type = T;
};

template <typename T>
using NoDeduce_t = typename NoDeduce<T>::type;

// 自己的库===========================
#include "Function/Input/InputDefines.h"
#include "Function/Render/RenderDefines.h"
#include "Component/ComponentDefines.h"
#include "Core/Assert/Assert.h"
#include "Core/Hash/Hash.h"
