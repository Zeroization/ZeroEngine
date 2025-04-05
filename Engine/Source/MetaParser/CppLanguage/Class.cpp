#include <Common/pch.h>
#include <CppLanguage/Class.h>

namespace MetaParser
{
    BaseClass::BaseClass(CX::Cursor& cursor) : name(Utils::GetTypeNameWithoutNamespace(cursor.GetType()))
    {
    }

    Class::Class(CX::Cursor& cursor, const std::vector<std::string>& curNamespace) : TypeInfo(cursor, curNamespace),
        m_name(cursor.GetSpelling()),
        m_qualifiedName(Utils::GetTypeNameWithoutNamespace(cursor.GetType())),
        m_displayName(Utils::GetNameWithoutFirstM(m_qualifiedName))
    {
        m_name = Utils::ReplaceAll(m_name, " ", "");
        m_name = Utils::ReplaceAll(m_name, "ZeroEngine::", ""); // TODO

        for (auto& childNode: cursor.GetChildren())
        {
            switch (childNode.GetKind())
            {
                case CXCursor_CXXBaseSpecifier:
                    m_baseClasses.emplace_back(std::make_shared<BaseClass>(childNode));
                    break;
                case CXCursor_FieldDecl:
                {
                    if (auto fieldPtr = std::make_shared<Field>(childNode, curNamespace, this);
                        fieldPtr->IsAccessible())
                    {
                        m_fields.emplace_back(fieldPtr);
                    }
                    break;
                }
                case CXCursor_CXXMethod:
                {
                    if (auto methodPtr = std::make_shared<Method>(childNode, curNamespace, this);
                        methodPtr->IsAccessible())
                    {
                        m_methods.emplace_back(methodPtr);
                    }
                    break;
                }
                case CXCursor_Constructor:
                {
                    if (auto methodPtr = std::make_shared<Method>(childNode, curNamespace, this);
                        methodPtr->IsAccessible())
                    {
                        m_constructors.emplace_back(methodPtr);
                    }
                    break;
                }
                default:
                    // std::cout << std::format("[{}]: Unimplemented cursor type: {}",
                    //                          __FUNCTION__,
                    //                          Utils::ToString(clang_getCursorKindSpelling(childNode.GetKind())))
                    //         << std::endl;
                    break;
            }
        }
    }

    bool Class::ShouldCompile() const
    {
        // 首先看自己该不该被反射
        bool res = m_metaData.ContainsTag(AnnotateAttr::ClassAnnotateAttr);
        // 然后才看成员和方法
        res &= (ShouldCompileFields() || ShouldCompileMethods());
        return res;
    }

    bool Class::ShouldCompileFields() const
    {
        // TODO: 根据Tag决定是否要反射
        return true;
    }

    bool Class::ShouldCompileMethods() const
    {
        // TODO: 根据Tag决定是否要反射
        return true;
    }

    bool Class::IsAccessible() const { return m_enabled; }

    std::string Class::GetClassName() const { return m_name; }
} // namespace MetaParser
