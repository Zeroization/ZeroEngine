#pragma once

#include <Common/pch.h>

namespace MetaParser::CX
{
    class Cursor;

    // 封装CXType
    class Type
    {
    public:
        explicit Type(const CXType& handle);

    public:
        // 获取类型全名
        std::string GetSpelling() const;
        // 获取类型的类型枚举
        CXTypeKind GetKind() const;
        // 函数类型相关
        int  GetArgumentCount() const;
        Type GetArgument(unsigned idx) const;
        // 获取当前节点的原始类型(typedef前的)
        Type GetCanonicalType() const;
        // 获取该类型的声明
        Cursor GetDeclaration() const;

    public:
        bool IsConst() const;

    private:
        CXType m_handle;
    };

} // namespace MetaParser::CX