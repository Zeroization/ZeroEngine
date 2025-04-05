#pragma once
#include "Hash.h"

namespace ZeroEngine
{
    /// 类似于给资产分配的UUID, 对象间也需要一个独特的ID, 即Identifier
    class Identifier
    {
    public:
        explicit Identifier() = default;

        explicit Identifier(const std::string_view name)
            : hash(StrHash::FNVHash(name)), debugName(name)
        {
        }

        explicit Identifier(uint32_t hash)
            : hash(hash)
        {
        }

        constexpr bool operator==(const Identifier& rhs) const noexcept { return hash == rhs.hash; }
        constexpr bool operator!=(const Identifier& rhs) const noexcept { return hash != rhs.hash; }
        /// 重载类型转换运算符
        explicit constexpr operator uint32_t() const noexcept { return hash; }

        constexpr std::string_view GetDebugName() const { return debugName; }
        constexpr uint32_t GetHashVal() const { return hash; }

    private:
        friend struct std::hash<Identifier>;
        uint32_t hash = 0;
        std::string_view debugName{};
    };
}

/// 特化std::hash, 支持将Identifier放到容器中
template<>
struct std::hash<ZeroEngine::Identifier>
{
    /// 直接返回Identifier内部的hash值, 无需重复计算
    size_t operator()(const ZeroEngine::Identifier& uniqueId) const noexcept
    {
        static_assert(noexcept(hash<uint32_t>()(uniqueId.hash)), "Hash<Identifier> should not throw");
        return uniqueId.hash;
    }
};
