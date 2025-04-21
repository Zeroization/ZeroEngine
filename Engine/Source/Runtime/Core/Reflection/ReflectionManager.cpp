#include "ReflectionManager.h"
#include "_generated/reflection/all_reflection.h"

namespace ZeroEngine::Reflection
{
    void ReflectionManager::Create()
    {
        // 注册类型反射
        TypeMetaRegister::DoMetaRegister();
    }

    void ReflectionManager::Destroy()
    {
        // 注销所有类型
        entt::meta_reset();
    }

    ReflectionManager& ReflectionManager::GetInstance()
    {
        static ReflectionManager sInstance;
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
