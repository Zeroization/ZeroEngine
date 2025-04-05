#include <Common/pch.h>
#include <Meta/MetaInfo.h>

namespace MetaParser
{
    MetaInfo::MetaInfo(CX::Cursor& cursor)
    {
        for (auto& childNode : cursor.GetChildren())
        {
            if (childNode.GetKind() != CXCursor_AnnotateAttr)
                continue;

            for (auto& [key, value] : ExtractTags(childNode))
            {
                m_tags[key] = value;
            }
        }
    }

    std::string MetaInfo::GetTagValue(const std::string& tag) const
    {
        auto result = m_tags.find(tag);
        return result == m_tags.end() ? "" : result->second;
    }

    bool MetaInfo::ContainsTag(const std::string& tag) const { return m_tags.contains(tag); }

    std::vector<MetaInfo::Tag> MetaInfo::ExtractTags(const CX::Cursor& cursor) const
    {
        std::vector<Tag> res;
        std::string      tagList = cursor.GetSpelling();

        // 先将[reflect-xxx; Xxxx = xxx; xxxx;]变为[reflect-xxx], [Xxxx = xxx], [xxxx]
        auto&& tags = Utils::Split(tagList, ";");

        static const std::string whiteSpaceString = " \t\r\n";
        for (auto& tagItem : tags)
        {
            // 解析Tag
            auto&& itemDetails = Utils::Split(tagItem, "=");
            auto&& tmpString   = Utils::Trim(itemDetails[0], whiteSpaceString);
            if (tmpString.empty())
            {
                continue;
            }
            res.emplace_back(tmpString, itemDetails.size() > 1 ? Utils::Trim(itemDetails[1], whiteSpaceString) : "");
        }

        return res;
    }
} // namespace MetaParser