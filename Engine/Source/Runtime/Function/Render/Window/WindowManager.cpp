#include "WindowManager.h"
#if defined(ZERO_PLATFORM_DESKTOP)
#include "GlfwWindowManager.h"
#endif
namespace ZeroEngine
{
    std::shared_ptr<WindowManager> WindowManager::Instance;

    std::shared_ptr<WindowManager> WindowManager::GetInstance()
    {
        return Instance;
    }

    void WindowManager::Create()
    {
#if defined(ZERO_PLATFORM_DESKTOP)
        Instance = std::make_shared<GlfwWindowManager>();
#endif
    }

    void *WindowManager::GetCurrentContext()
    {
        return GetWindowPtr();
    }
}
