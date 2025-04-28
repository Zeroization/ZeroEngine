#pragma once

#include <cstdint>

namespace ZeroEngine
{
    enum class ComponentType :uint8_t
    {
        BaseComponent,
        Transform,
        MeshRenderer,
        Camera,
        Light
    };
}
