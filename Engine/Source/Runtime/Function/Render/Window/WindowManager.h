#pragma once

#include <pch.h>

namespace ZeroEngine
{
    /// 程序窗口管理器, 用于管理程序运行的主窗口
    /// 目前仅支持GLFW作为窗口, 供OpenGL和Vulkan(后续支持)使用
    /// 网上查到DX12也能用GLFW作为窗口, 不过要hook到GLFW窗口的id, 待研究...
    class WindowManager
    {
    public:
        virtual ~WindowManager() = default;

        static std::shared_ptr<WindowManager> GetInstance();
        static void Create();

        virtual void CloseWindow() = 0;
        virtual bool WindowShouldClose() = 0;

        /// 逻辑相关
        virtual void Update() = 0;
        virtual bool CheckKeyDown(Key key) = 0;
        virtual bool CheckMouseDown(MouseButton mb) = 0;

        /// Getters
        virtual void *GetCurrentContext();
        virtual void *GetWindowPtr() = 0;
        virtual float GetCurFrameTime() = 0;
        virtual std::pair<float, float> GetMousePos() = 0;

    private:
        static std::shared_ptr<WindowManager> Instance;
    };
}
