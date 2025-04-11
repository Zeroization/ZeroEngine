#include "EventManager.h"

#include "Core/Hash/Identifier.h"

namespace ZeroEngine
{
    std::shared_ptr<EventManager> EventManager::sInstance = nullptr;

    void EventManager::Create()
    {
        sInstance = std::make_shared<EventManager>();

        // EventType
        sInstance->mStrToID["EventType::None"] = Identifier("EventType::None");
        sInstance->mStrToID["EventType::Builtin_KeyboardInput"] = Identifier("EventType::Builtin_KeyboardInput");

        // EventPriority
        sInstance->mStrToID["EventPriority::High"] = Identifier("EventPriority::High");
        sInstance->mStrToID["EventPriority::Medium"] = Identifier("EventPriority::Medium");
        sInstance->mStrToID["EventPriority::Low"] = Identifier("EventPriority::Low");
    }

    void EventManager::Destroy()
    {
        sInstance->mEventDispatcher.clear();
        sInstance->mStrToID.clear();
        sInstance.reset();
    }

    std::shared_ptr<EventManager> EventManager::GetInstance()
    {
        return sInstance;
    }

    uint32_t EventManager::GetEventTypeHash(EventType eventTy)
    {
        switch (eventTy)
        {
            case EventType::None: return TryGetHash("EventType::None");
            /// Builtin_InputEvent
            case EventType::Builtin_KeyboardInput: return TryGetHash("EventType::Builtin_KeyboardInput");
            default:
            {
                LOG_ERROR(std::format("[{}] Unexpected input enum, plz fix src code.", __FUNCTION__));
                return 0;
            }
        }
    }

    uint32_t EventManager::GetEventPriorityHash(EventPriority priority)
    {
        switch (priority)
        {
            case EventPriority::High: return TryGetHash("EventPriority::High");
            case EventPriority::Medium: return TryGetHash("EventPriority::Medium");
            case EventPriority::Low: return TryGetHash("EventPriority::Low");
            default:
            {
                LOG_ERROR(std::format("[{}] Unexpected input enum, plz fix src code.", __FUNCTION__));
                return 0;
            }
        }
    }

    uint32_t EventManager::TryGetHash(const std::string& str)
    {
        if (sInstance->mStrToID.contains(str))
        {
            return sInstance->mStrToID[str].GetHashVal();
        }

        LOG_ERROR(std::format("[{}] Unknown input {}, plz check!", __FUNCTION__, str));
        return 0;
    }
} // ZeroEngine
