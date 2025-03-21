#include "Game.h"

#include "Function/Input/InputManager.h"
#include "Function/Render/RenderEngine.h"
#include "Function/Render/Window/WindowManager.h"
#include "GUI/EditorGUIManager.h"

namespace ZeroEngine
{
    float Game::gEngineDeltaTime = 0.0;
    float Game::gLastFrameTime = 0.0;
    float Game::gCurFrameTime = 0.0;

    bool Game::Init(const std::string& path)
    {
        Logger::Init();
        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));

        RenderEngine::Create();
        InputManager::Create();
#ifdef ZERO_EDITOR
        EditorGUIManager::Create();
#endif

        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));
        return true;
    }

    void Game::Tick()
    {
        while (!RenderEngine::GetInstance()->WindowShouldClose())
        {
            gCurFrameTime = WindowManager::GetInstance()->GetCurFrameTime();
            gEngineDeltaTime = gCurFrameTime - gLastFrameTime;
            gLastFrameTime = gCurFrameTime;

            // 避免因调试时断点命中 GetCurFrameTime() 导致 DeltaTime 过长
            if (gEngineDeltaTime > 1.0f)
            {
                // TODO: 游戏帧数定长更新, 目前为144Hz
                gEngineDeltaTime = 1.0f / 144.0f;
            }

            LogicTick(gEngineDeltaTime);
            RenderTick();
        }
    }

    void Game::FixedTick(float deltaTime)
    {
        // TODO
    }

    void Game::ShutDown()
    {
        LOG_INFO(std::format("[{}] Engine Shutdown...", __FUNCTION__));

        EditorGUIManager::Destroy();
        InputManager::Destroy();
        RenderEngine::Destroy();
    }

    void Game::LogicTick(float deltaTime)
    {
        LOG_DEBUG(std::format("[{}] Engine Delta time: {}", __FUNCTION__, gEngineDeltaTime));

        InputManager::GetInstance()->Update();
        EditorGUIManager::GetInstance()->Update();
    }

    void Game::RenderTick()
    {
        RenderEngine::GetInstance()->BeginRender();

        RenderEngine::GetInstance()->Render();
#ifdef ZERO_EDITOR
        EditorGUIManager::GetInstance()->Render();
#endif

        RenderEngine::GetInstance()->EndRender();
    }
}
