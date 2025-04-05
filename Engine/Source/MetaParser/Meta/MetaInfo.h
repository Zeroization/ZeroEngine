#pragma once

#include <CXClass/Cursor/Cursor.h>

namespace MetaParser
{
    class MetaInfo
    {
    public:
        explicit MetaInfo(CX::Cursor& cursor);

        std::string GetTagValue(const std::string& tag) const;

        bool ContainsTag(const std::string& tag) const;

    private:
        using Tag = std::pair<std::string, std::string>;
        std::vector<Tag> ExtractTags(const CX::Cursor& cursor) const;

    private:
        std::unordered_map<std::string, std::string> m_tags;
    };

} // namespace MetaParser