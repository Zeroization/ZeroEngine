#pragma once
#include "pch.h"

#if defined(ZERO_GRAPHIC_OPENGL) || defined(ZERO_GRAPHIC_VULKAN)
#include <glm/glm.hpp>
#elif defined(ZERO_GRAPHIC_D3D12)
// FIXME: 此预定义未验证
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_MATRIX_ROW_MAJOR
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#endif
