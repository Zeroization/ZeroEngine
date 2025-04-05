#pragma once
#include <Generator/Generator.h>
namespace MetaParser::Generator
{
    // 为要反射的类生成序列化/反序列化文件
    class SerializerGenerator final : public BaseGenerator
    {
    public:
        SerializerGenerator() = delete;
        SerializerGenerator(std::string_view                             srcDirectory,
                            std::function<std::string(std::string_view)> getIncludeFunction);
        ~SerializerGenerator() override;

        int  Generate(std::string_view path, const SchemaModule& schema) override;
        void Finish() override;

    protected:
        void        PrepareStatus(std::string_view path) override;
        std::string ProcessFileName(std::string_view path) override;

    private:
        Inja::json m_classDefines;
        Inja::json m_includeHeaderFiles;
    };

} // namespace MetaParser::Generator