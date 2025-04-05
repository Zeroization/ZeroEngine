#include "ReflectionGenerator.h"

#include "CppLanguage/Class.h"
#include "TemplateManager.h"

namespace MetaParser::Generator
{
    ReflectionGenerator::ReflectionGenerator(std::string_view srcDirectory,
                                             std::function<std::string(std::string_view)>
                                             getIncludeFunction) : BaseGenerator(
        std::string(srcDirectory).append("/_generated/reflection"), srcDirectory, getIncludeFunction)
    {
        // std::cerr << "New Reflection Generator, outPath: " << m_outPath << std::endl;
        // std::cerr << "RootPath: " << m_rootPath << std::endl;
        PrepareStatus(m_outPath);
    }

    ReflectionGenerator::~ReflectionGenerator() = default;

    int ReflectionGenerator::Generate(std::string_view path, const SchemaModule& schema)
    {
        std::cerr << std::format("[{}] Try generate path: {}, schema name: {}", __FUNCTION__, path, schema.moduleName)
                << std::endl;

        using ArrayMap = std::map<std::string, std::pair<std::string, std::string>>;
        // 用于反射数组类型 TODO: 更多数组类型, Map类型也应该反射
        static const std::string vectorPrefix = "std::vector<";

        std::string filePath = ProcessFileName(path);

        Inja::json renderData;

        std::string rootIncludeHeaderFile = Utils::MakeRelativePath(m_rootPath, path).string();
        renderData["include_header_files"].push_back(rootIncludeHeaderFile);
        m_headerFileList.emplace_back(rootIncludeHeaderFile);

        std::string srcFileCamel = Utils::ConvertToUpperCamelCase(fs::path(path).stem().string(), "_");
        renderData["source_file_name_upper_camel_case"] = srcFileCamel;
        m_sourceFileList.emplace_back(srcFileCamel);

        // 反射类
        Inja::json classDefines;
        for (const auto& srcClass: schema.classes)
        {
            // if (!srcClass->ShouldCompileInReflection())
            //     continue;

            std::vector<std::string> fieldNames;
            ArrayMap arrayMap;

            Inja::json classDefine;
            // 获取类信息
            GenClassRenderData(srcClass, classDefine);
            // 反射类的成员变量
            for (const auto& field: srcClass->m_fields)
            {
                // if (!field->ShouldCompileInReflection())
                //     continue;

                fieldNames.emplace_back(field->m_name);

                // 对数组类型特殊处理 TODO: 更多数组类型
                if (bool isArray = field->m_type.starts_with(vectorPrefix))
                {
                    std::string arrayUsefulName = Utils::FormatQualifiedName(field->m_type);
                    std::string itemType = Utils::GetNameWithoutContainer(field->m_type);
                    arrayMap[field->m_type] = std::make_pair(arrayUsefulName, itemType);
                }
            }

            // 处理数组类型
            classDefine["array_exist"] = false;
            Inja::json arrayDefines;
            if (!arrayMap.empty())
            {
                classDefine["array_exist"] = true;
                for (const auto& arrayItem: arrayMap)
                {
                    std::string arrayTypeName = arrayItem.first;
                    std::string arrayUsefulName = arrayItem.second.first;
                    std::string arrayElemTypeName = arrayItem.second.second;

                    Inja::json arrayDefine;
                    arrayDefine["array_type_name"] = arrayTypeName;
                    arrayDefine["array_useful_name"] = arrayUsefulName;
                    arrayDefine["array_elem_type_name"] = arrayElemTypeName;
                    arrayDefines.emplace_back(arrayDefine);
                }
            }
            classDefine["array_defines"] = arrayDefines;

            // TODO: 处理Map类型

            classDefines.emplace_back(classDefine);
        }
        renderData["class_defines"] = classDefines;

        std::string renderedString = TemplateManager::GetInstance().RenderTemplate("commonReflectionFile", renderData);
        Utils::SaveFile(renderedString, filePath);

        return 0;
    }

    void ReflectionGenerator::Finish()
    {
        BaseGenerator::Finish();

        Inja::json renderData, includeHeaderFiles, sourceFileNames;
        for (const auto& headerFile: m_headerFileList)
        {
            includeHeaderFiles.emplace_back(headerFile);
        }
        renderData["include_header_files"] = includeHeaderFiles;
        for (const auto& sourceFile: m_sourceFileList)
        {
            sourceFileNames["source_file_name_upper_camel_case"].emplace_back(sourceFile);
        }
        renderData["source_file_names"] = sourceFileNames;

        std::string renderedString = TemplateManager::GetInstance().RenderTemplate("allReflectionFile", renderData);
        Utils::SaveFile(renderedString, m_outPath.append("/all_reflection.h"));
    }

    void ReflectionGenerator::PrepareStatus(std::string_view path)
    {
        BaseGenerator::PrepareStatus(path);

        std::cerr << "Before Load Refl Template, rootPath: " << m_rootPath << std::endl;
        TemplateManager::GetInstance().LoadTemplate(m_rootPath, "commonReflectionFile");
        TemplateManager::GetInstance().LoadTemplate(m_rootPath, "allReflectionFile");
        std::cerr << "After Load Refl Template" << std::endl;
    }

    std::string ReflectionGenerator::ProcessFileName(std::string_view path)
    {
        auto relativeDir = fs::path(path).filename().replace_extension("reflgen.h").string();
        return m_outPath + "/" + relativeDir;
    }
} // namespace MetaParser::Generator
