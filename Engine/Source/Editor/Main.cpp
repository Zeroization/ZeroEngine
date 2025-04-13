#include <pch.h>
#include "Game.h"

// GPU定义, 强行应用独立显卡=============
#define ZERO_GPU_VENDOR GPU_VENDOR_NVIDIA
#if defined(ZERO_OS_WINDOWS) && ZERO_GPU_VENDOR == GPU_VENDOR_NVIDIA
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
#elif defined(ZERO_OS_WINDOWS) && ZERO_GPU_VENDOR == GPU_VENDOR_AMD
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif

int main()
{
    if (ZeroEngine::Game::Init("TODO"))
    {
        ZeroEngine::Game::Tick();
        ZeroEngine::Game::ShutDown();
    }

    return 0;
}
