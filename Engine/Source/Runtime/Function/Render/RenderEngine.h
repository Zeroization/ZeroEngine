#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class RenderEngine
    {
    public:
        static void Create();
        static std::shared_ptr<RenderEngine> GetInstance();

        RenderEngine() = default;
        virtual ~RenderEngine() = default;

        void BeginRender();
        void Render();
        void EndRender();

        void CloseWindow();
        bool WindowShouldClose();

    private:
        static std::shared_ptr<RenderEngine> Instance;
    };
} // ZeroEngine
