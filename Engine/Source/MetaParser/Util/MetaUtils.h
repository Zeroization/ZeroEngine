#pragma once

#include <CXClass/Cursor/Cursor.h>
#include <CXClass/Type/Type.h>

namespace MetaParser::Utils
{
    // ============ String utils ===============
    inline std::string ToString(const CXString& cxstr)
    {
        std::string str = clang_getCString(cxstr);
        clang_disposeString(cxstr);
        return str.empty() ? "" : str;
    }

    inline std::string ToUpper(const std::string& srcString)
    {
        std::string ret(srcString);
        std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
        return ret;
    }

    inline std::vector<std::string> Split(const std::string& input, const std::string& pattern)
    {
        std::vector<std::string> res;

        std::string inputCp(input);
        while (true)
        {
            size_t idx = inputCp.find(pattern);
            std::string sub = inputCp.substr(0, idx);
            if (!sub.empty())
            {
                res.push_back(sub);
            }
            inputCp.erase(0, idx + pattern.size());
            if (idx == std::string::npos)
            {
                break;
            }
        }

        return res;
    }

    inline std::string Trim(const std::string& srcString)
    {
        std::string result(srcString);
        result = result.erase(0, result.find_first_not_of(" "));
        result = result.erase(result.find_last_not_of(" ") + 1);
        return result;
    }

    inline std::string Trim(const std::string& srcString, const std::string& trimChars)
    {
        std::string result(srcString);

        size_t leftPos = result.find_first_not_of(trimChars);
        if (leftPos == std::string::npos)
        {
            result = std::string();
        }
        else
        {
            size_t rightPos = result.find_last_not_of(trimChars);
            result = result.substr(leftPos, rightPos - leftPos + 1);
        }

        return result;
    }

    inline std::string Replace(const std::string& srcString, const std::string& before, const std::string& after)
    {
        std::string ret(srcString);
        std::string::size_type pos = 0;
        while ((pos = ret.find(before)) != std::string::npos)
        {
            ret.replace(pos, before.length(), after);
        }
        return ret;
    }

    inline std::string Replace(const std::string& srcString, char before, char after)
    {
        std::string ret(srcString);
        std::replace(ret.begin(), ret.end(), before, after);
        return ret;
    }

    inline std::string ReplaceAll(const std::string& srcString, const std::string& before, const std::string& after)
    {
        std::string result(srcString);

        std::string::size_type pos = 0;
        while ((pos = result.find(before)) != std::string::npos)
        {
            result = result.replace(pos, before.length(), after);
        }

        return result;
    }

    inline std::string GetQualifiedName(const CX::Type& type) { return type.GetSpelling(); }

    inline std::string GetQualifiedName(const std::string& spelling, const std::vector<std::string>& curNamespace)
    {
        std::string name;
        for (const auto& iter: curNamespace)
        {
            name += (iter + "::");
        }
        name += spelling;

        return name;
    }

    inline std::string GetQualifiedName(const CX::Cursor& cursor, const std::vector<std::string>& curNamespace)
    {
        return GetQualifiedName(cursor.GetSpelling(), curNamespace);
    }

    // 将<, >, *, :等影响编译的符号替换成字母
    inline std::string FormatQualifiedName(const std::string& srcString)
    {
        std::string ret(srcString);
        ret = Utils::Replace(ret, '<', 'L');
        ret = Utils::Replace(ret, ':', 'S');
        ret = Utils::Replace(ret, '>', 'R');
        ret = Utils::Replace(ret, '*', 'P');
        return ret;
    }

    inline std::string GetTypeNameWithoutNamespace(const CX::Type& type)
    {
        std::string&& typeName = type.GetSpelling();
        return typeName;
    }

    // aaa::bbb<xxxxx> -> xxxxx
    inline std::string GetNameWithoutContainer(const std::string& name)
    {
        size_t left = name.find_first_of('<') + 1;
        size_t right = name.find_last_of('>');
        if (left > 0 && right < name.size() && left < right)
        {
            return name.substr(left, right - left);
        }
        else
        {
            return name;
        }
    }

    // m_xxx -> xxx
    inline std::string GetNameWithoutFirstM(const std::string& name)
    {
        std::string res(name);
        if (res.size() > 2 && res.starts_with("m_"))
        {
            res = res.substr(2);
        }
        return res;
    }

    inline std::string GetStringWithoutQuot(const std::string& input)
    {
        std::string ret(input);
        size_t left = ret.find_first_of('\"') + 1;
        size_t right = ret.find_last_of('\"');
        if (left > 0 && right < ret.size() && left < right)
        {
            ret = ret.substr(left, right - left);
        }
        return ret;
    }

    // aaa[Spliter]bbb[Spliter] -> AaaBbb
    inline std::string ConvertToUpperCamelCase(const std::string& name, const std::string& splitter)
    {
        std::string ret;

        auto&& nameSplits = Utils::Split(name, splitter);
        for (auto& splitStr: nameSplits)
        {
            splitStr[0] = static_cast<char>(std::toupper(splitStr[0]));
            ret.append(splitStr);
        }
        return ret;
    }

    // ============ String utils ===============

    // ============ File utils ===============
    inline std::string FormatPathString(const std::string& pathString)
    {
        std::string localPathString(pathString);
        fs::path localPath = localPathString;

        if (localPath.is_relative())
        {
            localPathString = fs::current_path().string() + "/" + localPathString;
        }

        localPathString = ReplaceAll(localPathString, "\\", "/");
        std::vector<std::string> subString = Split(localPathString, "/");
        std::vector<std::string> outSubString;
        for (auto& p: subString)
        {
            if (p == "..")
            {
                outSubString.pop_back();
            }
            else if (p != ".")
            {
                outSubString.push_back(p);
            }
        }

        std::string ret;
        for (auto& part: outSubString)
        {
            ret.append(part + "/");
        }
        ret = Trim(ret, "/");

        return ret;
    }

    inline fs::path MakeRelativePath(const fs::path& from, const fs::path& to)
    {
        fs::path fromComplete = FormatPathString(from.string());
        fs::path toComplete = FormatPathString(to.string());

        auto iterFrom = fromComplete.begin();
        auto iterTo = toComplete.begin();

        while (iterFrom != fromComplete.end() && iterTo != toComplete.end() && (*iterTo) == (*iterFrom))
        {
            ++iterTo;
            ++iterFrom;
        }

        fs::path finalPath;
        while (iterFrom != fromComplete.end())
        {
            finalPath /= "..";
            ++iterFrom;
        }
        while (iterTo != toComplete.end())
        {
            finalPath /= *iterTo;
            ++iterTo;
        }

        return finalPath;
    }

    inline std::string GetFilename(const std::string& path)
    {
        if (path.empty())
        {
            return {};
        }
        std::vector<std::string> result = Split(path, "/");
        if (result.empty())
        {
            return {};
        }
        return result[result.size() - 1];
    }

    inline std::string LoadFile(const std::string& path)
    {
        std::ifstream iFile(path);
        std::string lineStr;
        std::ostringstream oStringStream;

        if (!iFile.is_open())
        {
            iFile.close();
            return "";
        }

        while (std::getline(iFile, lineStr))
        {
            oStringStream << lineStr << std::endl;
        }
        iFile.close();

        return oStringStream.str();
    }

    inline void SaveFile(const std::string& outputContent, const std::string& outputPath)
    {
        const fs::path outPath(outputPath);
        if (!fs::exists(outPath.parent_path()))
        {
            fs::create_directories(outPath.parent_path());
        }

        std::fstream outFileStream(outputPath, std::ios_base::out);
        outFileStream << outputContent << std::endl;
        outFileStream.flush();
        outFileStream.close();
    }

    // ============ File utils ===============

    // ============ Other utils ===============
    inline void FatalError(const std::string& error)
    {
    }

    template<typename A, typename B>
    inline bool RangeEqual(A aStart, A aEnd, B bStart, B bEnd)
    {
    }

    // ============ Other utils ===============
} // namespace MetaParser::Utils
