#include <Common/pch.h>
#include <CppLanguage/TypeInfo.h>

namespace MetaParser
{
    TypeInfo::TypeInfo(CX::Cursor& cursor, const std::vector<std::string>& curNamespace) :
        m_metaData(cursor), m_rootCursor(cursor), m_namespace(curNamespace)
    {
        // TODO: 添加一些Tag, 例如Dismiss
        m_enabled = true;
    }

    const MetaInfo&          TypeInfo::GetMetaData() const { return m_metaData; }
    std::string              TypeInfo::GetSourceFile() const { return m_rootCursor.GetSrcFilePath(); }
    std::vector<std::string> TypeInfo::GetCurrentNamespace() const { return m_namespace; }
    CX::Cursor&              TypeInfo::GetCursor() { return m_rootCursor; }
} // namespace MetaParser