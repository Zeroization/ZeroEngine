#include "ReflectionManager.h"
#include "_generated/reflection/all_reflection.h"

namespace ZeroEngine::Reflection
{
    std::shared_ptr<ReflectionManager> ReflectionManager::sInstance = nullptr;

    void ReflectionManager::Create()
    {
        sInstance = std::make_shared<ReflectionManager>();

        // 注册类型反射
        TypeMetaRegister::DoMetaRegister();
    }

    void ReflectionManager::Destroy()
    {
        // 注销所有类型
        entt::meta_reset();

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
