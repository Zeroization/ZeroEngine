#pragma once

#include <Common/SchemaModule.h>
#include <Common/pch.h>

namespace MetaParser::Generator
{
    class BaseGenerator
    {
    public:
        BaseGenerator(std::string_view                             outPath,
                      std::string_view                             rootPath,
                      std::function<std::string(std::string_view)> getIncludeFunc);
        virtual ~BaseGenerator() = default;

        virtual int  Generate(std::string_view path, const SchemaModule& schema) = 0;
        virtual void Finish() {};

    protected:
        virtual void        PrepareStatus(std::string_view path);
        virtual void        GenClassRenderData(std::shared_ptr<Class> src, Inja::json& inOutData);
        virtual void        GenClassFieldRenderData(std::shared_ptr<Class>& parent, Inja::json& inOutData);
        virtual void        GenClassMethodRenderData(std::shared_ptr<Class>& parent, Inja::json& inOutData);
        virtual void        GenClassCtorRenderData(std::shared_ptr<Class>& parent, Inja::json& inOutData);
        virtual std::string ProcessFileName(std::string_view path) = 0;

    protected:
        std::string                                  m_outPath = "gen_src";
        std::string                                  m_rootPath;
        std::function<std::string(std::string_view)> m_getIncludeFunc;
    };
} // namespace MetaParser::Generator