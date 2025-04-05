#include "SerializerGenerator.h"

#include "CppLanguage/Class.h"
#include "TemplateManager.h"

namespace MetaParser::Generator
{
    SerializerGenerator::SerializerGenerator(std::string_view                             srcDirectory,
                                             std::function<std::string(std::string_view)> getIncludeFunction) :
        BaseGenerator(std::string(srcDirectory).append("/_generated/serializer"), srcDirectory, getIncludeFunction)
    {
        PrepareStatus(m_outPath);
    }

    SerializerGenerator::~SerializerGenerator() = default;

    int SerializerGenerator::Generate(std::string_view path, const SchemaModule& schema)
    {
        std::string filePath = ProcessFileName(path);

        Inja::json srcData;
        Inja::json includeHeaderFiles;
        Inja::json classDefines;

        includeHeaderFiles.emplace_back(Utils::MakeRelativePath(m_rootPath, path).string());

        auto InsertIncludes = [&](const std::string& typeName) {
            const auto includeFile = m_getIncludeFunc(typeName);
            if (!includeFile.empty())
            {
                if (const auto includeFileBase = ProcessFileName(includeFile); filePath != includeFileBase)
                {
                    includeHeaderFiles.emplace_back(Utils::MakeRelativePath(m_rootPath, includeFileBase).string());
                }
            }
        };

        for (const auto& srcClass : schema.classes)
        {
            // if (!srcClass->ShouldCompileClassInSerialize())
            //     continue;

            Inja::json classDefine;
            GenClassRenderData(srcClass, classDefine);

            // 处理BaseClass的头文件
            for (const auto& m_baseClass : srcClass->m_baseClasses)
            {
                InsertIncludes(m_baseClass->name);
            }

            // 处理Field的头文件
            for (const auto& field : srcClass->m_fields)
            {
                // if (!field->ShouldCompileFieldsInSerialize())
                //     continue

                // 处理std容器 TODO: 看看对不对
                if (field->m_type.starts_with("std::"))
                {
                    InsertIncludes(field->m_name);
                }

                // TODO: 处理其他要包括头文件的类型
            }

            classDefines.push_back(classDefine);
            m_classDefines.push_back(classDefine);
        }

        srcData["class_defines"]        = classDefines;
        srcData["include_header_files"] = includeHeaderFiles;

        std::string renderedCode = TemplateManager::GetInstance().RenderTemplate("commonSerializerGenFile", srcData);
        Utils::SaveFile(renderedCode, filePath);

        m_includeHeaderFiles.push_back(Utils::MakeRelativePath(m_rootPath, filePath).string());
        return 0;
    }

    void SerializerGenerator::Finish()
    {
        Inja::json data;
        data["class_defines"]        = m_classDefines;
        data["include_header_files"] = m_includeHeaderFiles;

        std::string renderedCode = TemplateManager::GetInstance().RenderTemplate("allSerializer.h", data);
        Utils::SaveFile(renderedCode, m_outPath + "/all_serializer.h");
        renderedCode = TemplateManager::GetInstance().RenderTemplate("allSerializer.ipp", data);
        Utils::SaveFile(renderedCode, m_rootPath + "/all_serializer.ipp");
    }

    void SerializerGenerator::PrepareStatus(std::string_view path)
    {
        BaseGenerator::PrepareStatus(path);

        TemplateManager::GetInstance().LoadTemplate(m_rootPath, "allSerializer.h");
        TemplateManager::GetInstance().LoadTemplate(m_rootPath, "allSerializer.ipp");
        TemplateManager::GetInstance().LoadTemplate(m_rootPath, "commonSerializerGenFile");
    }

    std::string SerializerGenerator::ProcessFileName(std::string_view path)
    {
        auto relativeDir = fs::path(path).filename().replace_extension("serializer.gen.h").string();
        return m_outPath + "/" + relativeDir;
    }
} // namespace MetaParser::Generator