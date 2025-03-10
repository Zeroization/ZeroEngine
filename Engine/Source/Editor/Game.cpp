#include "Game.h"

#include "Function/Input/InputManager.h"
#include "Function/Render/RenderEngine.h"

namespace ZeroEngine
{
    bool Game::Init(const std::string &path)
    {
        Logger::Init();
        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));

        RenderEngine::Create();
        InputManager::Create();

        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));
        return true;
    }

    void Game::Tick()
    {
        while (!RenderEngine::GetInstance()->WindowShouldClose())
        {
            LogicTick(1.0f);
            RenderTick();
        }
    }

    void Game::ShutDown()
    {
        LOG_INFO(std::format("[{}] Engine Shutdown...", __FUNCTION__));
    }

    void Game::LogicTick(float deltaTime)
    {
        InputManager::GetInstance()->Update();
    }

    void Game::RenderTick()
    {
        RenderEngine::GetInstance()->BeginRender();
        RenderEngine::GetInstance()->Render();
        RenderEngine::GetInstance()->EndRender();
    }
}
