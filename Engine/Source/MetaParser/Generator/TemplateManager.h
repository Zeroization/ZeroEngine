#pragma once

#include <Common/pch.h>

namespace MetaParser::Generator
{
    class TemplateManager
    {
    public:
        TemplateManager(const TemplateManager&)            = delete;
        TemplateManager(TemplateManager&&)                 = delete;
        TemplateManager& operator=(const TemplateManager&) = delete;
        TemplateManager& operator=(TemplateManager&&)      = delete;

        static TemplateManager& GetInstance()
        {
            static TemplateManager instance;
            return instance;
        }

        void        LoadTemplate(const std::string& path, const std::string& templateName);
        std::string RenderTemplate(const std::string& templateName, const Inja::json& data);

    private:
        TemplateManager() = default;

    private:
        std::unordered_map<std::string, inja::Template> m_templates;
        std::unique_ptr<inja::Environment>              m_injaEnvPtr = nullptr;
    };
} // namespace MetaParser::Generator