#pragma once

#include <CppLanguage/TypeInfo.h>

namespace MetaParser
{
    class Class;

    class Method : public TypeInfo
    {
    public:
        Method(CX::Cursor& cursor, const std::vector<std::string>& curNamespace, Class* parent = nullptr);

        bool ShouldCompile() const;
        bool IsAccessible() const;

    public:
        Class*                                           m_parent;
        std::string                                      m_name;
        std::string                                      m_returnType;
        std::vector<std::pair<std::string, std::string>> m_typeAndNameArgs;
    };
} // namespace MetaParser