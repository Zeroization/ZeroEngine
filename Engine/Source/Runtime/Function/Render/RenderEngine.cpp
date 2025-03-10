#include "RenderEngine.h"
#include "Function/Render/Window/WindowManager.h"
#include "Function/Render/RHI/RHI.h"

namespace ZeroEngine
{
    std::shared_ptr<RenderEngine> RenderEngine::Instance = nullptr;

    void RenderEngine::Create()
    {
        Instance = std::make_shared<RenderEngine>();

        WindowManager::Create();
        RHI::Create();
    }

    std::shared_ptr<RenderEngine> RenderEngine::GetInstance()
    {
        return Instance;
    }

    void RenderEngine::BeginRender()
    {
        RHI::GetInstance()->BeginFrame();
    }

    void RenderEngine::Render()
    {
    }

    void RenderEngine::EndRender()
    {
        RHI::GetInstance()->EndFrame();
    }

    void RenderEngine::CloseWindow()
    {
        WindowManager::GetInstance()->CloseWindow();
    }

    bool RenderEngine::WindowShouldClose()
    {
        return WindowManager::GetInstance()->WindowShouldClose();
    }
} // ZeroEngine
