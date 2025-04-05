#pragma once

#include <CppLanguage/Field.h>
#include <CppLanguage/Method.h>
#include <CppLanguage/TypeInfo.h>

namespace MetaParser
{
    struct BaseClass
    {
        std::string name;

        BaseClass(CX::Cursor& cursor);
    };

    class Class : public TypeInfo
    {
        // 让其他类能访问m_qualifiedName
        friend class Field;
        friend class Method;
        friend class Parser;

    public:
        Class(CX::Cursor& cursor, const std::vector<std::string>& curNamespace);

        // TODO: 应该按情况分, 例如ShouldCompileInSerialize(), ShouldCompileInReflect()
        bool ShouldCompile() const;
        bool ShouldCompileFields() const;
        bool ShouldCompileMethods() const;
        bool IsAccessible() const;

        std::string GetClassName() const;

    public:
        std::string m_name;
        std::string m_qualifiedName;
        std::string m_displayName;

        template<typename T>
        using SharedPtrVector = std::vector<std::shared_ptr<T>>;

        SharedPtrVector<BaseClass> m_baseClasses;
        SharedPtrVector<Field>     m_fields;
        SharedPtrVector<Method>    m_methods;
        SharedPtrVector<Method>    m_constructors;
    };
} // namespace MetaParser