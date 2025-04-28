#include "EventManager.h"

#include "Core/Hash/Identifier.h"

namespace ZeroEngine
{
    void EventManager::Create()
    {
        // EventType
        GetInstance().mStrToID["EventType::None"] = Identifier("EventType::None");
        GetInstance().mStrToID["EventType::Builtin_KeyboardInput"] = Identifier("EventType::Builtin_KeyboardInput");
        GetInstance().mStrToID["EventType::Builtin_WindowResize"] = Identifier("EventType::Builtin_WindowResize");

        // EventPriority
        GetInstance().mStrToID["EventPriority::High"] = Identifier("EventPriority::High");
        GetInstance().mStrToID["EventPriority::Medium"] = Identifier("EventPriority::Medium");
        GetInstance().mStrToID["EventPriority::Low"] = Identifier("EventPriority::Low");
    }

    void EventManager::Destroy()
    {
        GetInstance().mEventDispatcher.clear();
        GetInstance().mStrToID.clear();
    }

    EventManager& EventManager::GetInstance()
    {
        static EventManager sInstance;
        return sInstance;
    }

    uint32_t EventManager::GetEventTypeHash(EventType eventTy)
    {
        switch (eventTy)
        {
            case EventType::None: return TryGetHash("EventType::None");
            /// Builtin_InputEvent
            case EventType::Builtin_KeyboardInput: return TryGetHash("EventType::Builtin_KeyboardInput");
            /// Builtin_WindowEvent
            case EventType::Builtin_WindowResize: return TryGetHash("EventType::Builtin_WindowResize");
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
        if (GetInstance().mStrToID.contains(str))
        {
            return GetInstance().mStrToID[str].GetHashVal();
        }

        LOG_ERROR(std::format("[{}] Unknown input {}, plz check!", __FUNCTION__, str));
        return 0;
    }
} // ZeroEngine
