#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class Camera;

    class RenderPassBase
    {
    public:
        RenderPassBase()
        {
        };

        virtual ~RenderPassBase()
        {
        };

        virtual void Render(Camera* camera) = 0;
    };
}
