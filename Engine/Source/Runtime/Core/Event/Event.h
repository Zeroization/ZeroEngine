#pragma once
#include "pch.h"
#include "Function/Render/Window/WindowManager.h"

namespace ZeroEngine
{
    /// 事件类型
    enum class EventType : uint8_t
    {
        /// 范围: 0 ~ 255
        None = 0,
        /// Builtin_InputEvent
        Builtin_KeyboardInput,
        /// Builtin_WindowEvent
        Builtin_WindowResize,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(EventType, {
                                 {EventType::None, "None"},
                                 {EventType::Builtin_KeyboardInput, "Builtin_KeyboardInput"},
                                 {EventType::Builtin_WindowResize, "Builtin_WindowResize"},
                                 })

    /// 事件大类
    enum class EventCategory : uint8_t
    {
        Builtin_OtherEvent  = 0b00000001,
        Builtin_InputEvent  = 0b00000010,
        Builtin_WindowEvent = 0b00000100,
        UserDefinedEvent    = 0b10000000
    };

    inline uint8_t operator|(EventCategory lhs, EventCategory rhs)
    {
        return static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs);
    }

    inline uint8_t operator&(EventCategory lhs, EventCategory rhs)
    {
        return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
    }

    NLOHMANN_JSON_SERIALIZE_ENUM(EventCategory, {
                                 {EventCategory::Builtin_OtherEvent, "Builtin_OtherEvent"},
                                 {EventCategory::Builtin_InputEvent, "BuiltIn_InputEvent"},
                                 {EventCategory::Builtin_WindowEvent, "BuiltIn_WindowEvent"},
                                 {EventCategory::UserDefinedEvent, "UserDefinedEvent"}
                                 })

    /// 事件优先级
    enum class EventPriority : uint8_t
    {
        High   = 1,
        Medium = 3,
        Low    = 5,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(EventPriority, {
                                 {EventPriority::High, "High"},
                                 {EventPriority::Medium, "Medium"},
                                 {EventPriority::Low, "Low"},
                                 })

    struct EventMetaData
    {
        EventType Type;         // 事件类型
        uint8_t Category;       // 事件大类分类
        EventPriority Priority; // 事件优先级
    };

    class Event
    {
    public:
        Event(EventType eventType, uint8_t eventCategory, EventPriority eventPriority = EventPriority::Medium)
            : mMetadata(eventType, eventCategory, eventPriority),
              mCurFrameTime(WindowManager::GetInstance().GetCurFrameTime())
        {
            nlohmann::json tmpJson = eventType;
            mEventTypeStr = tmpJson.dump();
        }

        virtual ~Event() = default;

        friend std::ostream& operator<<(std::ostream& os, const Event& e)
        {
            return os << e.ToString();
        }

        std::string_view ToString() const
        {
            return mEventTypeStr;
        };

        bool HasCategory(EventCategory category) const
        {
            return mMetadata.Category & static_cast<uint8_t>(category);
        }

    public:
        bool mIsHandled = false;                               // 该事件是否被处理, 处理了就直接跳过/停止向下传递
        EventMetaData mMetadata;                               // 事件元数据 (类型, 大类, 优先级)
        float mCurFrameTime = 0.f;                             // 事件的时间戳
        std::string mEventTypeStr;                             // 事件的类型字符串
        std::unordered_map<std::string, entt::meta_any> mArgs; // 事件携带的参数

        // TODO: 序列化与反序列化
    };
}
