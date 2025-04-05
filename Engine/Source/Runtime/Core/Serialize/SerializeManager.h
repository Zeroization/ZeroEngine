#pragma once
#include "pch.h"
#include "Core/Reflection/ReflectionManager.h"
#include "nlohmann/json.hpp"

namespace ZeroEngine
{
    using json = nlohmann::json;

    class SerializeManager
    {
    public:
        static void Create();
        static void Destroy();
        static std::shared_ptr<SerializeManager> GetInstance();

        SerializeManager() = default;
        virtual ~SerializeManager() = default;

    private:
        static std::shared_ptr<SerializeManager> sInstance;

        // 模板部分=============================================
    public:
        template<typename T, typename = void>
        struct has_adl_serializer : std::false_type
        {
        };

        template<typename T>
        struct has_adl_serializer<T, std::void_t<decltype(
                    nlohmann::adl_serializer<T>::to_json(
                        std::declval<json&>(),
                        std::declval<const T&>()
                    )
                )>> : std::true_type
        {
        };

        template<typename T>
        bool SerializeToJSON(json& ctx, const NoDeduce_t<T>& instance)
        {
            Reflection::CustomData* customData = entt::resolve<T>().custom();
            if constexpr (has_adl_serializer<T>::value)
            {
                if (customData != nullptr)
                {
                    ctx[customData->specifier] = instance;
                }
                else
                {
                    ctx = instance;
                }
                return true;
            }
            else
            {
                LOG_WARN(std::format("[{}] Unsupported type.", __FUNCTION__));
                return false;
            }
        }

        template<typename T>
        bool DeserializeFromJSON(const json& ctx, NoDeduce_t<T>& instance)
        {
            if constexpr (has_adl_serializer<T>::value)
            {
                instance = ctx.get<T>();
                return true;
            }
            else
            {
                LOG_WARN(std::format("[{}] Unsupported type.", __FUNCTION__));
                return false;
            }
        }
    };
} // ZeroEngine
