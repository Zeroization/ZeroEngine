#include <Common/pch.h>
#include <CppLanguage/Class.h>
#include <Generator/ReflectionGenerator.h>
#include <Generator/SerializerGenerator.h>
#include <Parser/Parser.h>

namespace MetaParser
{
#define RECURSE_NAMESPACES(kind, cursor, method, namespaces) \
    { \
        if (kind == CXCursor_Namespace) \
        { \
            auto displayName = cursor.GetSpelling(); \
            if (!displayName.empty()) \
            { \
                namespaces.push_back(displayName); \
                method(cursor, namespaces); \
                namespaces.pop_back(); \
            } \
        } \
    }
#define TRY_ADD_CPP_TYPE(handle, container) \
    { \
        if (handle->ShouldCompile()) \
        { \
            auto file                        = handle->GetSourceFile(); \
            std::cerr << "Try add cpp file: " << handle->GetSourceFile() << std::endl; \
            m_schemaModules[file].moduleName = m_moduleName; \
            m_schemaModules[file].container.push_back(handle); \
            m_typeTable[handle->m_displayName] = file; \
        } \
    }

    Parser::Parser(const std::string& projectInputFile,
                   const std::string& includeFilePath,
                   const std::string& includePath,
                   const std::string& includeSys,
                   const std::string& moduleName,
                   bool isShowErrors) : m_projectInputFile(projectInputFile), m_includeSrcFileName(includeFilePath),
                                        m_index(nullptr),
                                        m_translationUnit(nullptr), m_includeSys(includeSys), m_moduleName(moduleName),
                                        m_isShowErrors(isShowErrors)
    {
        m_workPaths = Utils::Split(includePath, ";");

        for (const auto& path: m_workPaths)
        {
            std::cerr << path << std::endl;
        }

        // 反射生成器
        m_generators.push_back(std::make_unique<Generator::ReflectionGenerator>(
            m_workPaths[0], std::bind(&MetaParser::Parser::GetIncludeFile, this, std::placeholders::_1)));
        // 序列化&反序列化生成器
        // m_generators.push_back(std::make_unique<Generator::SerializerGenerator>(
        //     m_workPaths[0], std::bind(&MetaParser::Parser::GetIncludeFile, this, std::placeholders::_1)));
    }

    Parser::~Parser()
    {
        m_generators.clear();
        if (m_translationUnit)
        {
            clang_disposeTranslationUnit(m_translationUnit);
        }
        if (m_index)
        {
            clang_disposeIndex(m_index);
        }
    }

    void Parser::Finish()
    {
        for (const auto& generator: m_generators)
        {
            generator->Finish();
        }
    }

    int Parser::Parse()
    {
        if (!ParseProject())
        {
            std::cerr << std::format("[{}] Parsing project file[ {} ] error!\n", __FUNCTION__, m_projectInputFile);
            return -1;
        }

        std::cout << std::format("[{}] Paring the whole project...", __FUNCTION__) << std::endl;

        int isShowErrors = m_isShowErrors ? 1 : 0;
        m_index = clang_createIndex(1, 1);
        std::string preInclude = "-I";
        std::string sysIncludeTmp;
        if (!(m_includeSys == "*"))
        {
            sysIncludeTmp = preInclude + m_includeSys;
            m_clangArgs.push_back(sysIncludeTmp.c_str());
        }

        auto paths = m_workPaths;
        for (auto& path: paths)
        {
            path = preInclude.append(path);
            m_clangArgs.push_back(path.c_str());
        }

        fs::path inputPath(m_includeSrcFileName);
        if (!fs::exists(inputPath))
        {
            std::cerr << "[" << __FUNCTION__ << "] File " << inputPath << " not exist!\n";
            return -2;
        }

        CXErrorCode errorCode = clang_parseTranslationUnit2(m_index,
                                                            m_includeSrcFileName.c_str(),
                                                            m_clangArgs.data(),
                                                            static_cast<int>(m_clangArgs.size()),
                                                            nullptr,
                                                            0,
                                                            CXTranslationUnit_None,
                                                            &m_translationUnit);
        if (errorCode != CXError_Success)
        {
            std::cerr << std::format(
                "[{}] CXTranslationUnit parse error: {}\n", __FUNCTION__, static_cast<int>(errorCode));
            return -3;
        }

        CX::Cursor cursor = CX::Cursor(clang_getTranslationUnitCursor(m_translationUnit));
        std::vector<std::string> tmpNamespace;
        BuildClassAST(cursor, tmpNamespace);

        tmpNamespace.clear();
        return 0;
    }

    void Parser::GenerateFiles()
    {
        std::cout << std::format("[{}] Start generate reflection & serialization code (total {})...",
                                 __FUNCTION__,
                                 m_schemaModules.size())
                << std::endl;

        for (auto& schema: m_schemaModules)
        {
            for (auto& generatorIter: m_generators)
            {
                generatorIter->Generate(schema.first, schema.second);
            }
        }

        Finish();
    }

    bool Parser::ParseProject()
    {
        std::cout << std::format("[{}] Parsing project file: {}", __FUNCTION__, m_projectInputFile) << std::endl;

        std::fstream includeTxtFile(m_projectInputFile, std::ios::in);
        if (includeTxtFile.fail())
        {
            std::cerr << std::format("[{}] Can't load file: {}\n", __FUNCTION__, m_projectInputFile);
            return false;
        }

        std::stringstream buffer;
        buffer << includeTxtFile.rdbuf();
        std::string context = buffer.str();

        auto includeFiles = Utils::Split(context, ";");
        std::fstream includeFile;
        includeFile.open(m_includeSrcFileName, std::ios::out);
        if (!includeFile.is_open())
        {
            std::cerr << std::format("[{}] Can't open src include file: {}\n", __FUNCTION__, m_includeSrcFileName);
            return false;
        }

        std::cout << std::format("[{}] Generating the src include file: {}", __FUNCTION__, m_includeSrcFileName)
                << std::endl;
        std::string outputFileName = Utils::GetFilename(m_includeSrcFileName);
        if (outputFileName.empty())
        {
            outputFileName = "META_INPUT_HEADER_H";
        }
        else
        {
            outputFileName = Utils::Replace(outputFileName, ".", "_");
            outputFileName = Utils::Replace(outputFileName, " ", "_");
            outputFileName = Utils::ToUpper(outputFileName);
        }
        includeFile << "#ifndef __" << outputFileName << "__" << std::endl;
        includeFile << "#define __" << outputFileName << "__" << std::endl;
        for (const auto& includeItem: includeFiles)
        {
            std::string tmp(includeItem);
            // std::cerr << std::format("Include File: {}", tmp) << std::endl;
            tmp = Utils::Replace(tmp, '\\', '/');
            tmp = Utils::Trim(tmp, "\n");
            includeFile << "#include \"" << tmp << "\"\n";
        }
        includeFile << "#endif" << std::endl;
        includeFile.close();

        return true;
    }

    void Parser::BuildClassAST(CX::Cursor& cursor, std::vector<std::string>& curNamespace)
    {
        for (auto& childNode: cursor.GetChildren())
        {
            auto kind = childNode.GetKind();

            if (childNode.IsDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl))
            {
                auto classPtr = std::make_shared<Class>(childNode, curNamespace);

                TRY_ADD_CPP_TYPE(classPtr, classes);
            }
            else
            {
                RECURSE_NAMESPACES(kind, childNode, BuildClassAST, curNamespace);
            }
        }
    }

    std::string Parser::GetIncludeFile(std::string_view name)
    {
        auto iter = m_typeTable.find(std::string(name));
        return iter == m_typeTable.end() ? "" : iter->second;
    }
} // namespace MetaParser
