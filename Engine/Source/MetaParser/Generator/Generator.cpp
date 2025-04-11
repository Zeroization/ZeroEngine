#include "CppLanguage/Class.h"

#include <Generator/Generator.h>

namespace MetaParser::Generator
{
    BaseGenerator::BaseGenerator(std::string_view outPath,
                                 std::string_view rootPath,
                                 std::function<std::string(std::string_view)> getIncludeFunc) : m_outPath(outPath),
        m_rootPath(rootPath), m_getIncludeFunc(getIncludeFunc)
    {
    }

    void BaseGenerator::PrepareStatus(std::string_view path)
    {
        if (!fs::exists(path))
        {
            fs::create_directories(path);
        }
    }

    void BaseGenerator::GenClassRenderData(std::shared_ptr<Class> src, Inja::json& inOutData)
    {
        inOutData["class_name"] = src->GetClassName();
        inOutData["class_base_class_size"] = src->m_baseClasses.size();
        inOutData["class_need_register"] = true;

        inOutData["class_is_internal"] = src->GetMetaData().ContainsTag(BasicTag::EngineInternal);

        inOutData["class_has_base"] = false;
        if (const size_t baseClassSize = inOutData["class_base_class_size"].get<size_t>(); baseClassSize > 0)
        {
            inOutData["class_has_base"] = true;

            Inja::json baseClassDefines;
            for (size_t idx = 0; idx < baseClassSize; ++idx)
            {
                Inja::json baseClassDefine;
                baseClassDefine["class_base_class_name"] = src->m_baseClasses[idx]->name;
                baseClassDefine["class_base_class_index"] = idx;
                baseClassDefines.emplace_back(baseClassDefine);
            }
            inOutData["class_base_class_defines"] = baseClassDefines;
        }

        Inja::json fieldDefines;
        GenClassFieldRenderData(src, fieldDefines);
        inOutData["class_field_defines"] = fieldDefines;

        Inja::json methodDefines;
        GenClassMethodRenderData(src, methodDefines);
        inOutData["class_method_defines"] = methodDefines;

        Inja::json ctorDefines;
        GenClassCtorRenderData(src, ctorDefines);
        inOutData["class_ctor_defines"] = ctorDefines;

        // std::cout << 123 << '\n';
    }

    void BaseGenerator::GenClassFieldRenderData(std::shared_ptr<Class>& parent, Inja::json& inOutData)
    {
        inOutData = inja::json::array();
        for (const auto& field: parent->m_fields)
        {
            Inja::json fieldDefine;
            fieldDefine["class_field_name"] = field->m_name;
            fieldDefine["class_field_type"] = field->m_type;
            fieldDefine["class_field_display_name"] = field->m_displayName;
            // 判断是否是顺序容器
            bool isSequence = field->m_type.starts_with("std::vector<") || field->m_type.starts_with("std::array<") ||
                              field->m_type.starts_with("std::deque<") || field->m_type.starts_with("std::list<");
            fieldDefine["class_field_is_sequence"] = isSequence;
            // 判断是否是关联容器
            bool isAssociate =
                    field->m_type.starts_with("std::map<") || field->m_type.starts_with("std::unordered_map<") ||
                    field->m_type.starts_with("std::set<") || field->m_type.starts_with("std::unordered_set<");
            fieldDefine["class_field_is_associate"] = isAssociate;
            inOutData.emplace_back(fieldDefine);
        }
    }

    void BaseGenerator::GenClassMethodRenderData(std::shared_ptr<Class>& parent, Inja::json& inOutData)
    {
        inOutData = inja::json::array();
        for (const auto& method: parent->m_methods)
        {
            Inja::json methodDefine;
            methodDefine["class_method_name"] = method->m_name;
            methodDefine["class_method_return_type"] = method->m_returnType;

            Inja::json argsDefine;
            bool hasArgs = false;
            for (const auto& [type, var]: method->m_typeAndNameArgs)
            {
                hasArgs = true;
                Inja::json argData;
                argData["arg_type"] = type;
                argData["arg_name"] = var;
                argsDefine.emplace_back(argData);
            }
            methodDefine["class_method_args"] = argsDefine;
            methodDefine["class_method_has_args"] = hasArgs;

            inOutData.emplace_back(methodDefine);
        }
    }

    void BaseGenerator::GenClassCtorRenderData(std::shared_ptr<Class>& parent, Inja::json& inOutData)
    {
        inOutData = Inja::json::array();
        for (const auto& method: parent->m_constructors)
        {
            Inja::json methodDefine;
            Inja::json argsDefine;
            bool hasArgs = false;
            for (const auto& [type, var]: method->m_typeAndNameArgs)
            {
                hasArgs = true;
                Inja::json argData;
                argData["arg_type"] = type;
                argData["arg_name"] = var;
                argsDefine.push_back(argData);
            }
            methodDefine["class_ctor_args"] = argsDefine;
            methodDefine["class_ctor_has_args"] = hasArgs;

            inOutData.push_back(methodDefine);
        }
    }
} // namespace MetaParser::Generator
