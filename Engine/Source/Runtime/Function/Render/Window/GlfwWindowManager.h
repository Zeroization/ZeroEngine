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

        void *GetCurrentContext() override;
        void *GetWindowPtr() override;
        void CloseWindow() override;
        bool WindowShouldClose() override;

        void Update() override;
        std::pair<float, float> GetMousePos() override;
        bool CheckKeyDown(Key key) override;
        bool CheckMouseDown(MouseButton mb) override;

    private:
        GLFWwindow *WindowPtr;
    };
} // ZeroEngine
