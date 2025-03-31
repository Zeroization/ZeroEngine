#pragma once
#include "pch.h"
#include "Core/Hash/Identifier.h"

// 用于反射的宏
#ifdef __REFLECTION_PARSER__
#define ZERO_CLASS_API __attribute__((annotate("reflect-class")))
#define ZERO_PROPERTY(...) __attribute__((annotate("reflect-property; " #__VA_ARGS__)))
#define ZERO_FUNCTION(...) __attribute__((annotate("reflect-function; " #__VA_ARGS__)))
#else
#define ZERO_CLASS_API
#define ZERO_PROPERTY(...)
#define ZERO_FUNCTION(...)
#endif

#define ZERO_CLASS(ClassTypeName, ...) \
    namespace ZeroEngine::Reflection \
    { \
        class Type##ClassTypeName##Operator; \
    } \
    class ZERO_CLASS_API ClassTypeName __VA_ARGS__

#define ZERO_BODY(ClassTypeName) \
    friend class ::ZeroEngine::Reflection::ReflectionManager; \
    friend class ::ZeroEngine::Reflection::Type##ClassTypeName##Operator; \
    // TODO: friend class ::ZeroEngine::SerializationManager;

// 仅供代码生成时使用的內部宏
#define ZERO_REFL_NAME_HASH(NameStr) \
    ::ZeroEngine::Reflection::ReflectionManager::GetInstance()->GetTypeNameHash(#NameStr)
#define ZERO_REFL_REGISTER_CLASS(ClassTypeName) \
    entt::meta_factory<ClassTypeName>{} \
        .type(ZERO_REFL_NAME_HASH(ClassTypeName));
#define ZERO_REFL_REGISTER_BASE_CLASS(ClassTypeName, BaseClassTypeName) \
    entt::meta_factory<ClassTypeName>{} \
        .base<BaseClassTypeName>();
#define ZERO_REFL_REGISTER_CTOR_BY_ARGS(ClassTypeName, ...) \
    entt::meta_factory<ClassTypeName>{} \
        .ctor<__VA_ARGS__>();
#define ZERO_REFL_REGISTER_CTOR_BY_FUNC(ClassTypeName, FuncAddr) \
    entt::meta_factory<ClassTypeName>{} \
        .ctor<FuncAddr>();
#define ZERO_REFL_REGISTER_DTOR(CLassTypeName, FuncAddr) \
    entt::meta_factory<CLassTypeName>{} \
        .dtor<FuncAddr>();
#define ZERO_REFL_REGISTER_VARIABLE(ClassTypeName, VariableName, VariableAddr) \
    entt::meta_factory<ClassTypeName>{} \
        .data<VariableAddr>(ZERO_REFL_NAME_HASH(VariableName));
#define ZERO_REFL_REGISTER_VARIABLE_BY_GETTER_AND_SETTER(ClassTypeName, VariableName, GetterVarOrFuncAddr, SetterVarOrFuncAddr) \
    entt::meta_factory<ClassTypeName>{} \
        .data<GetterVarOrFuncAddr, SetterVarOrFuncAddr>(ZERO_REFL_NAME_HASH(VariableName));
#define ZERO_REFL_REGISTER_FUNCTION(ClassTypeName, FuncName, FuncAddr, FuncRetType, ...) \
    entt::meta_factory<ClassTypeName>{} \
        .func<static_cast<FuncRetType(ClassTypeName::*)(__VA_ARGS__)>(FuncAddr)>(ZERO_REFL_NAME_HASH(FuncName));
#define ZERO_REFL_REGISTER_FUNCTION_NO_ARGS(ClassTypeName, FuncName, FuncAddr) \
    entt::meta_factory<ClassTypeName>{} \
        .func<FuncAddr>(ZERO_REFL_NAME_HASH(FuncName));

namespace ZeroEngine::Reflection
{
    class ReflectionManager
    {
    public:
        static void Create();
        static void Destroy();
        static std::shared_ptr<ReflectionManager> GetInstance();

        ReflectionManager() = default;
        virtual ~ReflectionManager() = default;

        /// 向反射系统中注册类型名和它的哈希值
        /// @param typeName 类型名
        /// @return 该类型名对应的哈希值
        uint32_t GetTypeNameHash(std::string_view typeName);

    private:
        static std::shared_ptr<ReflectionManager> sInstance;
        std::unordered_map<std::string_view, Identifier> mTyNameToIdentifier;

        // 模板部分==============================================================
    public:
        // 禁止模板参数类型自动推导, 让用户显式指定参数
        template<typename T>
        struct NoDeduce
        {
            using type = T;
        };

        template<typename T>
        using NoDeduce_t = typename NoDeduce<T>::type;

        /// 调用一个反射函数
        /// @tparam ClassType 类型名
        /// @param funcName 要调用的函数名
        /// @param instance 调用的对象示例
        /// @param args 参数列表
        /// @return 指向该对象的指针, 没有就是nullptr
        template<typename ClassType, typename RetType, typename... Args>
        NoDeduce_t<RetType>* InvokeFunction(std::string_view funcName, NoDeduce_t<ClassType>* instance,
                                            NoDeduce_t<Args>... args)
        {
            if (entt::meta_func func = entt::resolve<ClassType>().func(GetTypeNameHash(funcName));
                func)
            {
                // 寻找函数重载
                while (func)
                {
                    if (entt::meta_any retAny = InternalInvokeFunction<ClassType, RetType, Args...>(
                            func, *instance, std::forward<Args>(args)...);
                        retAny)
                    {
                        return retAny.try_cast<NoDeduce_t<RetType>>();
                    }
                    func = func.next();
                }
            }

            LOG_ERROR("[{}] No such function {}() !", __FUNCTION__, funcName);
            return nullptr;
        }

    private:
        /// 实际调用函数的地方, 仅供内部调用
        /// @tparam ClassType 类型名
        /// @param func 要调用的函数
        /// @param instance 该类型的示例
        /// @param args 函数参数列表
        /// @return 调用的返回值
        template<typename ClassType, typename RetType, typename... Args>
        entt::meta_any InternalInvokeFunction(const entt::meta_func& func, ClassType& instance, Args&&... args)
        {
            // 匹配返回值类型
            if (func.ret() != entt::resolve<RetType>()) { return {}; }

            // 匹配参数数量
            if (func.arity() != sizeof...(args)) { return {}; }

            // 匹配参数类型
            const bool isTypeMatch = [&]<size_t... index>(std::index_sequence<index...>)
            {
                return (
                    [&]
                    {
                        const auto expected = func.arg(index).id();
                        const auto actual = entt::type_hash<std::tuple_element_t<index, std::tuple<Args...>>>::value();
                        // LOG_DEBUG(std::format("参数位置 {}: , 预期类型Hash = {}, 实际类型Hash = {}", index, expected, actual));
                        return (actual == expected);
                    }() && ...
                );
            }(std::make_index_sequence<sizeof...(args)>{});
            if (!isTypeMatch)
            {
                return {};
            }

            // 调用函数
            try
            {
                auto ret = func.invoke(instance, std::forward<Args>(args)...);
                return ret;
            } catch (const std::exception& e)
            {
                LOG_ERROR(
                    std::format("[{}] Error occurred when invoking a reflect function: {}", __FUNCTION__, e.what()));
                return {};
            }
        }
    };
} // ZeroEngine::Reflection
