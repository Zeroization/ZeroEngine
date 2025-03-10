#include <pch.h>
#include "Game.h"

int main()
{
    if (ZeroEngine::Game::Init("TODO"))
    {
        ZeroEngine::Game::Tick();
        ZeroEngine::Game::ShutDown();
    }

    return 0;
}
