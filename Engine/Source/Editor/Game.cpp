#include "Game.h"

#include "Core/Reflection/ReflectionManager.h"
#include "Function/Input/InputManager.h"
#include "Function/Render/RenderEngine.h"
#include "Function/Render/Window/WindowManager.h"
#include "GUI/EditorGUIManager.h"

// TODO: 测试完记得删掉
#include <_generated/reflection/Student.reflgen.h>

#include "Core/Event/Event.h"
#include "Core/Event/EventManager.h"

namespace ZeroEngine
{
    float Game::gEngineDeltaTime = 0.0;
    float Game::gLastFrameTime = 0.0;
    float Game::gCurFrameTime = 0.0;

    bool Game::Init(const std::string& path)
    {
        Logger::Init();
        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));

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
            struct TestListener
            {
                void OnT1Event(Event& e)
                {
                    LOG_DEBUG(
                        std::format("T1 Callback, Event timestamp: {}, priority: {}, value: {}",
                            e.mCurFrameTime, e.ToString(), val));
                }

                void OnT2Event(Event& e)
                {
                    LOG_DEBUG(
                        std::format("T2 Callback, Event timestamp: {}, type: {}, value: {}",
                            e.mCurFrameTime, e.ToString(), val));
                }

                int val = 0;
            };
            TestListener t1(3), t2(66);

            auto eventMgr = EventManager::GetInstance();

            EventMetaData t1Meta = {
                EventType::Builtin_KeyboardInput,
                static_cast<uint8_t>(
                    EventCategory::Builtin_InputEvent),
                EventPriority::High
            };
            EventMetaData t2Meta = {
                EventType::None,
                static_cast<uint8_t>(
                    EventCategory::Builtin_OtherEvent),
                EventPriority::Low
            };
            eventMgr->BindListener<TestListener, &TestListener::OnT1Event>(t1, t1Meta);
            eventMgr->BindListener<TestListener, &TestListener::OnT2Event>(t2, t2Meta);

            Event T1Event1(EventType::Builtin_KeyboardInput,
                           static_cast<uint8_t>(EventCategory::Builtin_InputEvent),
                           EventPriority::High);
            Event T1Event2(EventType::Builtin_KeyboardInput,
                           static_cast<uint8_t>(EventCategory::Builtin_InputEvent),
                           EventPriority::High);
            Event T2Event1(EventType::None,
                           static_cast<uint8_t>(EventCategory::Builtin_OtherEvent),
                           EventPriority::Low);
            Event T1Event3(EventType::Builtin_KeyboardInput,
                           static_cast<uint8_t>(EventCategory::Builtin_InputEvent),
                           EventPriority::High);
            Event T2Event2(EventType::None,
                           static_cast<uint8_t>(EventCategory::Builtin_OtherEvent),
                           EventPriority::Low);

            eventMgr->EnqueueEvent(std::move(T2Event1));
            eventMgr->EnqueueEvent(std::move(T2Event2));
            eventMgr->EnqueueEvent(std::move(T1Event1));
            eventMgr->EnqueueEvent(std::move(T1Event2));
            eventMgr->EnqueueEvent(std::move(T1Event3));

            eventMgr->UpdateEventQueue();
        }

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
        EventManager::Destroy();
        SerializeManager::Destroy();
        Reflection::ReflectionManager::Destroy();
    }

    void Game::LogicTick(float deltaTime)
    {
        // LOG_DEBUG(std::format("[{}] Engine Delta time: {}", __FUNCTION__, gEngineDeltaTime));

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
