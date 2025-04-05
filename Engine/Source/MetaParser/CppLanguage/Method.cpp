#include <Common/pch.h>

#include <CppLanguage/Class.h>
#include <CppLanguage/Method.h>

namespace MetaParser
{
    Method::Method(CX::Cursor& cursor, const std::vector<std::string>& curNamespace, Class* parent) :
        TypeInfo(cursor, curNamespace), m_parent(parent), m_name(cursor.GetSpelling()),
        m_returnType(cursor.GetReturnType().GetSpelling())
    {
        m_enabled =
            m_metaData.ContainsTag(AnnotateAttr::FunctionAnnotateAttr) || cursor.GetKind() == CXCursor_Constructor;

        // 获取参数列表
        for (auto& childNode : cursor.GetChildren())
        {
            if (childNode.GetKind() == CXCursor_ParmDecl)
            {
                std::string varName  = childNode.GetSpelling();
                std::string typeName = childNode.GetType().GetSpelling();
                m_typeAndNameArgs.emplace_back(typeName, varName);
            }
        }
    }

    bool Method::ShouldCompile() const { return IsAccessible(); }
    bool Method::IsAccessible() const { return m_enabled; }
} // namespace MetaParser