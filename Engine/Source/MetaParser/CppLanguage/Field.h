#pragma once

#include <CppLanguage/TypeInfo.h>

namespace MetaParser
{
    class Class;

    class Field : public TypeInfo
    {
    public:
        Field(CX::Cursor& cursor, const std::vector<std::string>& curNamespace, Class* parent = nullptr);

        bool ShouldCompile() const;
        bool IsAccessible() const;

    public:
        Class*      m_parent;
        std::string m_name;
        std::string m_displayName;
        std::string m_type;
        std::string m_default;

        bool m_isConst;
    };
} // namespace MetaParser
