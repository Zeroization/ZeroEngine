#pragma once

#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    class RHI_OpenGLImpl final : public RHI
    {
    public:
        RHI_OpenGLImpl();

        void BeginFrame() override;
        void EndFrame() override;
    };
} // ZeroEngine
