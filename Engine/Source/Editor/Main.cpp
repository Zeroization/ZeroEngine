#include <iostream>
#include <Log/Logger.h>

int main()
{
    std::cout << "Hello Editor!\n";

    ZeroEngine::Logger::Init();
    LOG_DEBUG("Hello Logger!");
    return 0;
}
