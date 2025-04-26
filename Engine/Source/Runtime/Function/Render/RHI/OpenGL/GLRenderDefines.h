#pragma once
#include "glad/glad.h"
#include "GLVertexBufferLayout.hpp"
#include "GLVertexArray.h"

namespace ZeroEngine::RHI_GL
{
    struct GLMaterialData
    {
        bool IsInUse = false;

        std::map<std::string, std::pair<ShaderPropertyType, entt::meta_any>> properties;
        std::map<std::string, std::array<uint32_t, 2>> textures;
        std::map<std::string, std::array<uint32_t, 2>> cubemaps;
    };
}
