#pragma once
#include "pch.h"

#if defined(ZERO_GRAPHIC_OPENGL) || defined(ZERO_GRAPHIC_VULKAN)
#include <glm/glm.hpp>
#elif defined(ZERO_GRAPHIC_D3D12)
// TODO: 使用DX专门的数学库
#endif
