#include "RenderEngine.h"

#include "FrameBuffer/FBOManager.h"
#include "Function/Render/Window/WindowManager.h"
#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    void RenderEngine::Create()
    {
        WindowManager::Create();
        RHI::Create();
        FBOManager::Create();
    }

    void RenderEngine::Destroy()
    {
        FBOManager::Destroy();
    }

    RenderEngine& RenderEngine::GetInstance()
    {
        static RenderEngine sInstance;
        return sInstance;
    }

    void RenderEngine::BeginRender()
    {
        RHI::GetInstance().BeginFrame();
    }

    void RenderEngine::Render()
    {
    }

    void RenderEngine::EndRender()
    {
        RHI::GetInstance().EndFrame();
    }

    void RenderEngine::CloseWindow()
    {
        WindowManager::GetInstance().CloseWindow();
    }

    bool RenderEngine::WindowShouldClose()
    {
        return WindowManager::GetInstance().WindowShouldClose();
    }
} // ZeroEngine
