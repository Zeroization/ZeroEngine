#pragma once

#include "Function/Input/InputManager.h"

namespace ZeroEngine
{
    class GlfwInputManager final : public InputManager
    {
    public:
        void Update() override;
    };
} // ZeroEngine
