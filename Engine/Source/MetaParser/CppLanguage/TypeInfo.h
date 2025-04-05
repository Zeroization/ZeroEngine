#pragma once

#include <CXClass/Cursor/Cursor.h>
#include <Meta/MetaInfo.h>

namespace MetaParser
{
    class TypeInfo
    {
    public:
        explicit TypeInfo(CX::Cursor& cursor, const std::vector<std::string>& curNamespace);

        const MetaInfo&          GetMetaData() const;
        std::string              GetSourceFile() const;
        std::vector<std::string> GetCurrentNamespace() const;
        CX::Cursor&              GetCursor();

    protected:
        MetaInfo                 m_metaData;
        bool                     m_enabled;
        std::string              m_CNAlias;
        std::vector<std::string> m_namespace;

    private:
        CX::Cursor m_rootCursor;
    };

} // namespace MetaParser
