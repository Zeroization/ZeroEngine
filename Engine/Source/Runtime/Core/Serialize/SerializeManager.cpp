#include "SerializeManager.h"

namespace ZeroEngine
{
    void SerializeManager::Create()
    {
        GetInstance();
    }

    void SerializeManager::Destroy()
    {
    }

    SerializeManager& SerializeManager::GetInstance()
    {
        static SerializeManager sInstance;
        return sInstance;
    }
} // ZeroEngine
