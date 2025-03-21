#pragma once

#include "WindowManager.h"
#include <GLFW/glfw3.h>

namespace ZeroEngine
{
    class GlfwWindowManager final : public WindowManager
    {
    public:
        GlfwWindowManager();
        ~GlfwWindowManager() override;

        void CloseWindow() override;
        bool WindowShouldClose() override;

        void Update() override;
        bool CheckKeyDown(Key key) override;
        bool CheckMouseDown(MouseButton mb) override;

        void *GetCurrentContext() override;
        void *GetWindowPtr() override;
        float GetCurFrameTime() override;
        std::pair<float, float> GetMousePos() override;

    private:
        GLFWwindow* WindowPtr;
    };
} // ZeroEngine
