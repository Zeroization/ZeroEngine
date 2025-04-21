#pragma once
#include "pch.h"
#include "Event.h"

namespace ZeroEngine
{
    class Identifier;

    class EventManager
    {
    public:
        static void Create();
        static void Destroy();
        static EventManager& GetInstance();

        EventManager() = default;
        virtual ~EventManager() = default;
        EventManager(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager& operator=(EventManager&&) = delete;

        uint32_t GetEventTypeHash(EventType eventTy);
        uint32_t GetEventPriorityHash(EventPriority priority);

    private:
        entt::dispatcher mEventDispatcher;
        std::unordered_map<std::string, Identifier> mStrToID;

        uint32_t TryGetHash(const std::string& str);

    private:
        // 模板部分
    public:
        /// 绑定事件监听器, 监听器为一个结构体/类, 包含诸如OnEvent(Event&)的回调
        /// @tparam ListenerTy 监听器类型
        /// @param listener 监听器实例
        /// @param metadata 事件元数据实例
        /// @return 是否成功绑定
        template <typename ListenerTy, auto CallbackFn>
        bool BindListener(NoDeduce_t<ListenerTy>& listener, EventMetaData& metadata)
        {
            // 事件队列: 按优先级分为三个队列
            entt::connection connectDeferred;
            if (metadata.Priority == EventPriority::High)
            {
                connectDeferred = mEventDispatcher
                                  .sink<Event>(GetEventPriorityHash(EventPriority::High))
                                  .connect<CallbackFn, ListenerTy>(listener);
            }
            else if (metadata.Priority == EventPriority::Medium)
            {
                connectDeferred = mEventDispatcher
                                  .sink<Event>(GetEventPriorityHash(EventPriority::Medium))
                                  .connect<CallbackFn, ListenerTy>(listener);
            }
            else
            {
                connectDeferred = mEventDispatcher
                                  .sink<Event>(GetEventPriorityHash(EventPriority::Low))
                                  .connect<CallbackFn, ListenerTy>(listener);
            }

            // 立即事件: 按事件类型分类
            auto connectImmediate = mEventDispatcher
                                    .sink<Event>(GetEventTypeHash(metadata.Type))
                                    .connect<CallbackFn, ListenerTy>(listener);

            return connectDeferred && connectImmediate;
        }

        /// @copydoc BindListener
        template <typename ListenerTy, auto CallbackFn>
        bool BindListener(NoDeduce_t<ListenerTy>* listener, EventMetaData& metadata)
        {
            // 事件队列: 按优先级分为三个队列
            entt::connection connectDeferred;
            if (metadata.Priority == EventPriority::High)
            {
                connectDeferred = mEventDispatcher
                                  .sink<Event>(GetEventPriorityHash(EventPriority::High))
                                  .connect<CallbackFn, ListenerTy>(*listener);
            }
            else if (metadata.Priority == EventPriority::Medium)
            {
                connectDeferred = mEventDispatcher
                                  .sink<Event>(GetEventPriorityHash(EventPriority::Medium))
                                  .connect<CallbackFn, ListenerTy>(*listener);
            }
            else
            {
                connectDeferred = mEventDispatcher
                                  .sink<Event>(GetEventPriorityHash(EventPriority::Low))
                                  .connect<CallbackFn, ListenerTy>(*listener);
            }

            // 立即事件: 按事件类型分类
            auto connectImmediate = mEventDispatcher
                                    .sink<Event>(GetEventTypeHash(metadata.Type))
                                    .connect<CallbackFn, ListenerTy>(*listener);

            return connectDeferred && connectImmediate;
        }

        /// 解绑监听器的所有回调
        /// @tparam ListenerTy 监听器类型
        /// @param listener 监听器实例
        template <typename ListenerTy>
        void UnbindListener(NoDeduce_t<ListenerTy>& listener)
        {
            mEventDispatcher.disconnect<ListenerTy>(listener);
        }

        /// 解绑监听器对特定事件的特定回调
        /// @tparam ListenerTy 监听器类型
        /// @tparam CallbackFn 解绑回调(地址)
        /// @param listener 监听器实例
        /// @param metadata 事件元数据实例
        template <typename ListenerTy, auto CallbackFn>
        void UnbindListenerByCallback(NoDeduce_t<ListenerTy>& listener, EventMetaData& metadata)
        {
            // 事件队列
            mEventDispatcher.sink<Event>(GetEventPriorityHash(EventPriority::High)).disconnect<CallbackFn,
                ListenerTy>(listener);
            mEventDispatcher.sink<Event>(GetEventPriorityHash(EventPriority::Medium)).disconnect<CallbackFn,
                ListenerTy>(listener);
            mEventDispatcher.sink<Event>(GetEventPriorityHash(EventPriority::Low)).disconnect<CallbackFn,
                ListenerTy>(listener);
            // 立即事件
            mEventDispatcher.sink<Event>(GetEventTypeHash(metadata.Type)).disconnect<CallbackFn, ListenerTy>(listener);
        }

        /// 立即触发事件
        /// @param event 事件实例
        void TriggerEvent(Event&& event)
        {
            mEventDispatcher.trigger<Event>(GetEventTypeHash(event.mMetadata.Type), std::forward<Event>(event));
        }

        /// 让事件进入对应优先级的事件队列
        /// @param event 事件实例
        void EnqueueEvent(Event&& event)
        {
            mEventDispatcher.enqueue_hint<Event>(GetEventPriorityHash(event.mMetadata.Priority),
                                                 std::forward<Event>(event));
        }

        /// 按优先级依次更新事件队列
        void UpdateEventQueue()
        {
            mEventDispatcher.update<Event>(GetEventPriorityHash(EventPriority::High));
            mEventDispatcher.update<Event>(GetEventPriorityHash(EventPriority::Medium));
            mEventDispatcher.update<Event>(GetEventPriorityHash(EventPriority::Low));
        }
    };
} // ZeroEngine
