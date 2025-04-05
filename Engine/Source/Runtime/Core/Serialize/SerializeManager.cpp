#include "SerializeManager.h"

namespace ZeroEngine
{
    std::shared_ptr<SerializeManager> SerializeManager::sInstance = nullptr;

    void SerializeManager::Create()
    {
        sInstance = std::make_shared<SerializeManager>();
    }

    void SerializeManager::Destroy()
    {
        sInstance.reset();
    }

    std::shared_ptr<SerializeManager> SerializeManager::GetInstance()
    {
        return sInstance;
    }
} // ZeroEngine
