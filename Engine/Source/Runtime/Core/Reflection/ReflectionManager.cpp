#include "ReflectionManager.h"

namespace ZeroEngine::Reflection
{
    std::shared_ptr<ReflectionManager> ReflectionManager::sInstance = nullptr;

    void ReflectionManager::Create()
    {
        sInstance = std::make_shared<ReflectionManager>();
    }

    void ReflectionManager::Destroy()
    {
        sInstance.reset();
    }

    std::shared_ptr<ReflectionManager> ReflectionManager::GetInstance()
    {
        return sInstance;
    }

    uint32_t ReflectionManager::GetTypeNameHash(std::string_view typeName)
    {
        if (!mTyNameToIdentifier.contains(typeName))
        {
            mTyNameToIdentifier.insert({typeName, Identifier(typeName)});
            // LOG_DEBUG(std::format("[{}] Registered Type: {}, Hash: {}",
            //     __FUNCTION__, typeName, mTyNameToIdentifier[typeName].GetHashVal()));
        }
        else
        {
            // LOG_DEBUG(std::format("[{}] Get Type: {}, Hash: {}", __FUNCTION__, typeName,
            //     mTyNameToIdentifier[typeName].GetHashVal()));
        }
        return mTyNameToIdentifier[typeName].GetHashVal();
    }
}
