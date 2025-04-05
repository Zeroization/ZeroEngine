#pragma once

#include <CXClass/Cursor/Cursor.h>
#include <Common/SchemaModule.h>
#include <Common/pch.h>
#include <Generator/Generator.h>

namespace MetaParser
{
    class Class;

    class Parser
    {
    public:
        Parser(const std::string& projectInputFile,
               const std::string& includeFilePath,
               const std::string& includePath,
               const std::string& includeSys,
               const std::string& moduleName,
               bool               isShowErrors);
        virtual ~Parser();

        void Finish();
        int  Parse();
        void GenerateFiles();

    private:
        bool        ParseProject();
        void        BuildClassAST(CX::Cursor& cursor, std::vector<std::string>& curNamespace);
        std::string GetIncludeFile(std::string_view name);

    private:
        std::string              m_projectInputFile;
        std::string              m_moduleName;
        std::string              m_includeSys;
        std::string              m_includeSrcFileName;
        std::vector<std::string> m_workPaths;

        std::unordered_map<std::string, std::string>           m_typeTable;     // <ClassName, filePath>
        std::unordered_map<std::string, SchemaModule>          m_schemaModules; // <filePath, SchemaModule>
        std::vector<std::unique_ptr<Generator::BaseGenerator>> m_generators;    // 根据模板生成对应反射&序列化注入文件

        CXIndex                  m_index;
        CXTranslationUnit        m_translationUnit;
        bool                     m_isShowErrors;
        std::vector<const char*> m_clangArgs = {"-x",
                                                "c++",
                                                "-std=c++20",
                                                "-D__REFLECTION_PARSER__",
                                                "-DNDEBUG",
                                                "-D__clang__",
                                                "-w",
                                                "-MG",
                                                "-M",
                                                "-ferror-limit=0",
                                                "-o clangParseLog.txt"};
    };
} // namespace MetaParser