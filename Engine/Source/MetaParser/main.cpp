#include <Common/pch.h>
#include <Parser/Parser.h>

int Parse(const std::string& projectInputFilename,
          const std::string& srcIncludeFilename,
          const std::string& pathInclude,
          const std::string& sysInclude,
          const std::string& moduleName,
          const std::string& showErrors)
{
    std::cout << std::endl;
    std::cout << std::format("[{}] Parsing metadata for module[{}]", __FUNCTION__, moduleName) << std::endl;

    const bool isShowErrors = "0" != showErrors;

    MetaParser::Parser parser(
        projectInputFilename, srcIncludeFilename, pathInclude, sysInclude, moduleName, isShowErrors);

    std::cout << std::format("[{}] Parsing in {}", __FUNCTION__, pathInclude) << std::endl;
    int res = parser.Parse();
    if (0 != res)
    {
        std::cerr << std::format("[{}] Parse error, code [{}]\n", __FUNCTION__, res);
    }
    else
    {
        parser.GenerateFiles();
    }

    return res;
}

int main(int argc, char* argv[])
{
    auto startTime = std::chrono::system_clock::now();
    int result = 0;

#if 1
    if (argv[1] != nullptr && argv[2] != nullptr && argv[3] != nullptr && argv[4] != nullptr && argv[5] != nullptr &&
        argv[6] != nullptr)
    {
        result = Parse(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
        auto durationTime = std::chrono::system_clock::now() - startTime;
        std::cout << std::format("[{}] Completed in {}",
                                 __FUNCTION__,
                                 std::chrono::duration_cast<std::chrono::milliseconds>(durationTime))
                << std::endl;
        return 0;
    }
    else
    {
        std::cerr << std::format("[{}] Input args format error, plz recheck formats.\n", __FUNCTION__);
        return -1;
    }
#else
    // For Debug Use...
    result = Parse(
        "D:/PROGRAMMING/Dev/cpp/Graduate/ZeroEngine/out/build/x64-debug/Engine/Source/MetaParser/precompile.json",
        "D:/PROGRAMMING/Dev/cpp/Graduate/ZeroEngine/out/build/x64-debug/parser_header.h",
        "D:/PROGRAMMING/Dev/cpp/Graduate/ZeroEngine/Engine/Source/Runtime",
        "*",
        "ZeroEngine",
        "0");
    auto durationTime = std::chrono::system_clock::now() - startTime;
    std::cout << std::format("[{}] Completed in {}",
                             __FUNCTION__,
                             std::chrono::duration_cast<std::chrono::milliseconds>(durationTime))
            << std::endl;
    return 0;
#endif
}
