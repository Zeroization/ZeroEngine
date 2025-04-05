#pragma once
#include <Generator/Generator.h>
namespace MetaParser::Generator
{
    class ReflectionGenerator final : public BaseGenerator
    {
    public:
        ReflectionGenerator() = delete;
        ReflectionGenerator(std::string_view                             srcDirectory,
                            std::function<std::string(std::string_view)> getIncludeFunction);
        ~ReflectionGenerator() override;

        int  Generate(std::string_view path, const SchemaModule& schema) override;
        void Finish() override;

    protected:
        void        PrepareStatus(std::string_view path) override;
        std::string ProcessFileName(std::string_view path) override;

    private:
        std::vector<std::string> m_headerFileList;
        std::vector<std::string> m_sourceFileList;
    };
} // namespace MetaParser::Generator