#pragma once

#include <pch.h>

namespace ZeroEngine
{
    class WindowManager
    {
    public:
        virtual ~WindowManager() = default;

        static std::shared_ptr<WindowManager> GetInstance();
        static void Create();

        virtual void *GetWindowPtr() = 0;
        virtual void CloseWindow() = 0;
        virtual bool WindowShouldClose() = 0;

        virtual void Update() = 0;
        virtual std::pair<float, float> GetMousePos() = 0;
        virtual bool CheckKeyDown(Key key) = 0;
        virtual bool CheckMouseDown(MouseButton mb) = 0;

    private:
        static std::shared_ptr<WindowManager> Instance;
    };
}
