#include "Game.h"

#include "Core/Reflection/ReflectionManager.h"
#include "Function/Input/InputManager.h"
#include "Function/Render/RenderEngine.h"
#include "Function/Render/Window/WindowManager.h"
#include "GUI/EditorGUIManager.h"

// TODO: 测试完记得删掉
#include <_generated/reflection/Student.reflgen.h>

#include "Core/GlobalDataManager.h"
#include "Core/Event/EventManager.h"
#include "Core/FileSystem/FileSystem.h"

namespace ZeroEngine
{
    bool Game::Init(const std::string& path)
    {
        Logger::Init();
        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));

        auto engineWorkDirPath = FileSystem::GetCurExeFilePath().parent_path().parent_path();
        LOG_INFO(std::format("[{}] Engine Working Dir: {}", __FUNCTION__, engineWorkDirPath.string()));
        if (!GlobalDataManager::Create(engineWorkDirPath))
        {
            LOG_CRITICAL(std::format("[{}] Can't init global config!", __FUNCTION__));
            return false;
        }

        Reflection::ReflectionManager::Create();
        SerializeManager::Create();
        EventManager::Create();
        RenderEngine::Create();
        InputManager::Create();
#ifdef ZERO_EDITOR
        EditorGUIManager::Create();
#endif

        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));

        // TODO: 做完Init相关测试后删掉
        {
        }

        return true;
    }

    void Game::Tick()
    {
        while (!RenderEngine::GetInstance().WindowShouldClose())
        {
            auto gData = GlobalDataManager::GetInstance().GetGlobalDataRef();
            gData->CurFrameTime = WindowManager::GetInstance().GetCurFrameTime();
            gData->EngineDeltaTime = gData->CurFrameTime - gData->LastFrameTime;
            gData->LastFrameTime = gData->CurFrameTime;

            // 避免因调试时断点命中 GetCurFrameTime() 导致 DeltaTime 过长
            if (gData->EngineDeltaTime > 1.0f)
            {
                // TODO: 游戏帧数定长更新, 目前为144Hz
                gData->EngineDeltaTime = 1.0f / 144.0f;
            }

            LogicTick(gData->EngineDeltaTime);
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
        EventManager::Destroy();
        SerializeManager::Destroy();
        Reflection::ReflectionManager::Destroy();
        GlobalDataManager::Destroy();
    }

    void Game::LogicTick(float deltaTime)
    {
        // LOG_DEBUG(std::format("[{}] Engine Delta time: {}", __FUNCTION__, gEngineDeltaTime));

        InputManager::GetInstance().Update();
        EditorGUIManager::GetInstance().Update();
    }

    void Game::RenderTick()
    {
        RenderEngine::GetInstance().BeginRender();

        RenderEngine::GetInstance().Render();
#ifdef ZERO_EDITOR
        EditorGUIManager::GetInstance().Render();
#endif

        RenderEngine::GetInstance().EndRender();
    }
}
