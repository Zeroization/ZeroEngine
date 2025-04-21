#pragma once

#include "pch.h"

namespace ZeroEngine
{
    class RenderEngine
    {
    public:
        static void Create();
        static void Destroy();
        static RenderEngine& GetInstance();

        RenderEngine() = default;
        virtual ~RenderEngine() = default;
        RenderEngine(const RenderEngine&) = delete;
        RenderEngine(RenderEngine&&) = delete;
        RenderEngine& operator=(const RenderEngine&) = delete;
        RenderEngine& operator=(RenderEngine&&) = delete;

        void BeginRender();
        void Render();
        void EndRender();

        void CloseWindow();
        bool WindowShouldClose();
    };
} // ZeroEngine
