#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class RHI
    {
    public:
        static void Create();
        static std::shared_ptr<RHI> GetInstance();

        RHI() = default;
        virtual ~RHI() = default;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        /// 光栅化渲染管线的RHI


    private:
        static std::shared_ptr<RHI> Instance;
    };
} // ZeroEngine
