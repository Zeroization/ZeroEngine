#include "WindowManager.h"
#if defined(ZERO_PLATFORM_DESKTOP)
#include "GlfwWindowManager.h"
#endif
namespace ZeroEngine
{
    WindowManager& WindowManager::GetInstance()
    {
#if defined(ZERO_PLATFORM_DESKTOP)
        static GlfwWindowManager sInstance;
#else
        ZERO_CORE_ASSERT(flase, "Other platform TODO");
#endif
        return sInstance;
    }

    void WindowManager::Create()
    {
        GetInstance();
    }

    void* WindowManager::GetCurrentContext()
    {
        return GetWindowPtr();
    }
}
