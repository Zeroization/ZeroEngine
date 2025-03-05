#pragma once
// pre-compiled header file

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

// 是否启用编辑器模式, 后续用于游戏打包=====
#define ZERO_EDITOR

// 标准库=============================
#include <iostream>

// 自己的库===========================
#include "Core/Log/Logger.h"
