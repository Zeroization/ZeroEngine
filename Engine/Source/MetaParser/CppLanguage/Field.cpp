#include <Common/pch.h>
#include <CppLanguage/Class.h>
#include <CppLanguage/Field.h>

namespace MetaParser
{
    Field::Field(CX::Cursor& cursor, const std::vector<std::string>& curNamespace, Class* parent) :
        TypeInfo(cursor, curNamespace), m_isConst(cursor.GetType().IsConst()), m_name(cursor.GetSpelling()),
        m_parent(parent), m_displayName(Utils::GetNameWithoutFirstM(m_name)),
        m_type(Utils::GetTypeNameWithoutNamespace(cursor.GetType()))
    {
        m_name = Utils::ReplaceAll(m_name, " ", "");
        m_name = Utils::ReplaceAll(m_name, "ZeroEngine::", ""); // TODO: 替换更多代码

        m_default = Utils::GetStringWithoutQuot(m_metaData.GetTagValue("default"));
        m_enabled = m_metaData.ContainsTag(AnnotateAttr::PropertyAnnotateAttr);
    }

    bool Field::ShouldCompile() const { return IsAccessible(); }
    bool Field::IsAccessible() const { return m_enabled; }
} // namespace MetaParser