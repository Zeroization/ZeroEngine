#pragma once

#include "pch.h"
#include "slang/slang.h"
#include "slang/slang-com-ptr.h"

namespace ZeroEngine
{
    struct Internal_SlangShaderBlob
    {
        Slang::ComPtr<slang::IBlob> Blob;
        std::string entryName;
        ShaderStageFlag stage = ShaderStageFlag::Unknown;
    };

    struct Internal_SlangGlobalSession
    {
        Slang::ComPtr<slang::IGlobalSession> GlobalSession;
        std::unique_ptr<SlangGlobalSessionDesc> GlobalSessionDesc;
    };

    struct Internal_SlangSession
    {
        Slang::ComPtr<slang::ISession> Session;
        std::unique_ptr<slang::SessionDesc> SessionDesc;

        std::unique_ptr<slang::TargetDesc> TargetDesc;
        std::vector<const char *> SearchPaths;
        std::vector<slang::CompilerOptionEntry> CompilerOpts;
    };

    struct Internal_SlangModuleInfo
    {
        Slang::ComPtr<slang::IModule> Module;
        std::vector<Internal_SlangShaderBlob> ShaderBlobs;
    };

    class MaterialData;

    class SlangShaderParser
    {
    public:
        static void Create();
        static void Destroy();
        static SlangShaderParser& GetInstance();

        SlangShaderParser() = default;
        virtual ~SlangShaderParser() = default;
        SlangShaderParser(const SlangShaderParser&) = delete;
        SlangShaderParser(SlangShaderParser&&) = delete;
        SlangShaderParser& operator=(const SlangShaderParser&) = delete;
        SlangShaderParser& operator=(SlangShaderParser&&) = delete;

        ShaderInfo GetShaderInfo(const std::string& modulePath, GraphAPI api);
        void TranslateSlangToShader(const std::string& modulePath, std::vector<Internal_SlangShaderBlob>& shaderBlobs,
                                    bool isForCompute = false);

    private:
        void InitSession();
        void PrintErrorLog(const Slang::ComPtr<slang::IBlob>& errorLogBlob);

    private:
        std::unique_ptr<Internal_SlangGlobalSession> mGlobalSession = nullptr;
        std::unique_ptr<Internal_SlangSession> mSession = nullptr;
        std::map<std::string, std::unique_ptr<Internal_SlangModuleInfo>> mModuleMap;
    };
} // ZeroEngine
