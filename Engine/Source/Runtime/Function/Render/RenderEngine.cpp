#include "RenderEngine.h"

#include "Feature/RenderPass/RenderPassBase.h"
#include "Feature/RenderPass/RenderPassManager.h"
#include "FrameBuffer/FBOManager.h"
#include "Function/Render/Window/WindowManager.h"
#include "Function/Render/RHI/RHI.h"
#include "Shader/SlangShaderParser.h"

namespace ZeroEngine
{
    void RenderEngine::Create()
    {
        WindowManager::Create();
        SlangShaderParser::Create();
        RHI::Create();
        FBOManager::Create();
        RenderPassManager::Create();
    }

    void RenderEngine::Destroy()
    {
        RenderPassManager::Destroy();
        FBOManager::Destroy();
        SlangShaderParser::Destroy();
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
        // TODO: SceneManager, Camera

        auto& renderPassMgr = RenderPassManager::GetInstance();
        for (const auto& pass: renderPassMgr.mCurPasses)
        {
            // pass->Render(camera);
        }
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
