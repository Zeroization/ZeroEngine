#include <Generator/TemplateManager.h>

namespace MetaParser::Generator
{
    void TemplateManager::LoadTemplate(const std::string& path, const std::string& templateName)
    {
        if (m_injaEnvPtr == nullptr)
        {
            // TODO: 这里初始化可能要改成Env(模板全局路径, 输出文件全局路径), 通过target_compile_definitions来搞
            m_injaEnvPtr = std::make_unique<Inja::Environment>(path);
        }

        if (m_templates.contains(templateName))
        {
            return;
        }

        m_templates.insert_or_assign(templateName,
                                     m_injaEnvPtr->parse_template(
                                         "/../CodeGenTemplates/" + templateName + ".inja"));
    }

    std::string TemplateManager::RenderTemplate(const std::string& templateName, const Inja::json& data)
    {
        if (m_injaEnvPtr == nullptr)
        {
            std::cerr << std::format(
                "[{}] m_injaEnvPtr is nullptr, please use LoadTemplate() first, return empty data...\n", __FUNCTION__);
            return "";
        }

        if (!m_templates.contains(templateName))
        {
            std::cerr << std::format(
                "[{}] Cannot find template \"{}\", return empty data...\n", __FUNCTION__, templateName);
            return "";
        }

        return m_injaEnvPtr->render(m_templates[templateName], data);
    }
} // namespace MetaParser::Generator
