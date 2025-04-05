#include "Game.h"

#include "Core/Reflection/ReflectionManager.h"
#include "Function/Input/InputManager.h"
#include "Function/Render/RenderEngine.h"
#include "Function/Render/Window/WindowManager.h"
#include "GUI/EditorGUIManager.h"

// TODO: 测试完记得删掉
#include <_generated/reflection/Student.reflgen.h>

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
        RenderEngine::Create();
        InputManager::Create();
#ifdef ZERO_EDITOR
        EditorGUIManager::Create();
#endif

        LOG_INFO(std::format("[{}] Engine Init =====================================", __FUNCTION__));

        // TODO: 做完Init相关测试后删掉
        {
            Reflection::TypeWrapperRegister::Register_Student();

            auto res = entt::resolve<Student>();

            auto ReflectMgr = Reflection::ReflectionManager::GetInstance();
            auto s = entt::resolve<Student>().construct(2333, true).cast<Student>();
            // 成员测试
            LOG_DEBUG("We got Gender: {}", *ReflectMgr->GetVariable<Student, bool>("Student::mGender", &s));
            ReflectMgr->SetVariable<Student, bool>("Student::mGender", &s, false);
            LOG_DEBUG("We got Gender: {}", *ReflectMgr->GetVariable<Student, bool>("Student::mGender", &s));

            // 方法测试
            ReflectMgr->InvokeFunction<Student, void, int>("Student::SetClassID", &s, 233);
            auto classIDAny = ReflectMgr->InvokeFunction<Student, int>("Student::GetClassID", &s);
            LOG_DEBUG("We got ClassID: {}", *classIDAny);
            ReflectMgr->InvokeFunction<Student, void, float>("Student::SetClassID", &s, 666.6);
            classIDAny = ReflectMgr->InvokeFunction<Student, int>("Student::GetClassID", &s);
            LOG_DEBUG("We got ClassID: {}", *classIDAny);
            ReflectMgr->InvokeFunction<Student, void, int, int>("Student::SetClassID", &s, 90, 10);
            classIDAny = ReflectMgr->InvokeFunction<Student, int>("Student::GetClassID", &s);
            LOG_DEBUG("We got ClassID: {}", *classIDAny);
            ReflectMgr->InvokeFunction<Student, void>("Student::staticTest", nullptr);

            // 序列化测试
            auto SerializeMgr = SerializeManager::GetInstance();
            std::vector<int> t = {1, 2, 3};
            json test;
            SerializeMgr->SerializeToJSON<std::vector<int>>(test, t);
            LOG_DEBUG("JSON: {}", test.dump());

            std::vector<Student> tt = {{123, true}, {456, false}};
            test.clear();
            SerializeMgr->SerializeToJSON<std::vector<Student>>(test, tt);
            LOG_DEBUG("JSON: {}", test.dump());

            std::vector<Student> ddd;
            SerializeMgr->DeserializeFromJSON<std::vector<Student>>(test, ddd);
            LOG_DEBUG("JSON: {}", test.dump());
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
