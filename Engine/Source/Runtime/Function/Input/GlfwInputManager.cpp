#include "GlfwInputManager.h"
#include "Function/Render/Window/GlfwWindowManager.h"

namespace ZeroEngine
{
    void GlfwInputManager::Update()
    {
        // 处理上一帧
        TransitionPressed();
        ClearReleased();

        // 更新这一帧
        WindowManager::GetInstance()->Update();
    }
} // ZeroEngine
