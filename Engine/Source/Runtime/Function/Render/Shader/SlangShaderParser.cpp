#include "SlangShaderParser.h"

#include "Core/GlobalDataManager.h"
#include "Core/FileSystem/FileSystem.h"

namespace ZeroEngine
{
    void SlangShaderParser::Create()
    {
        // 建立全局会话
        GetInstance().mGlobalSession = std::make_unique<Internal_SlangGlobalSession>();
        GetInstance().mGlobalSession->GlobalSessionDesc = std::make_unique<SlangGlobalSessionDesc>();
#ifdef ZERO_GRAPHIC_OPENGL
        GetInstance().mGlobalSession->GlobalSessionDesc->enableGLSL = true;
#endif
        slang::createGlobalSession(GetInstance().mGlobalSession->GlobalSessionDesc.get(),
                                   GetInstance().mGlobalSession->GlobalSession.writeRef());

        GetInstance().InitSession();
    }

    void SlangShaderParser::Destroy()
    {
        GetInstance().mSession.reset();
        GetInstance().mGlobalSession.reset();
    }

    SlangShaderParser& SlangShaderParser::GetInstance()
    {
        static SlangShaderParser sInstance;
        return sInstance;
    }

    void SlangShaderParser::InitSession()
    {
        if (mSession != nullptr)
        {
            return;
        }
        mSession = std::make_unique<Internal_SlangSession>();

        auto& session = GetInstance().mSession;
        session->SessionDesc = std::make_unique<slang::SessionDesc>();

        // 目标平台
        session->TargetDesc = std::make_unique<slang::TargetDesc>();
#ifdef ZERO_GRAPHIC_OPENGL
        session->TargetDesc->format = SLANG_SPIRV;
        session->TargetDesc->profile = mGlobalSession->GlobalSession->findProfile("glsl_450");
#else
        ZERO_CORE_ASSERT(false, "TODO......");
#endif
        session->SessionDesc->targets = session->TargetDesc.get();
        session->SessionDesc->targetCount = 1;

        // 搜索路径
        std::string builtinShaderPath = (GlobalDataManager::GetInstance().GetGlobalDataRef()->BuiltinAssetsDir /
                                         "Shaders").string();
        session->SearchPaths.push_back(builtinShaderPath.c_str());
        session->SessionDesc->searchPaths = session->SearchPaths.data();
        session->SessionDesc->searchPathCount = session->SearchPaths.size();

        // TODO: 可能存在的预定义宏 PreprocessorMacroDesc

        // 编译器附加设置
#if defined(ZERO_DEBUG_ENABLE)  && !defined(ZERO_GRAPHIC_OPENGL)
        slang::CompilerOptionEntry debugLevelOpt;
        debugLevelOpt.name = slang::CompilerOptionName::DebugInformation;
        debugLevelOpt.value = {
            slang::CompilerOptionValueKind::Int, SlangDebugInfoLevel::SLANG_DEBUG_INFO_LEVEL_MAXIMAL, 0, nullptr,
            nullptr
        };
        mSession->CompilerOpts.push_back(debugLevelOpt);
#endif
        // 让编译器使用最新的预编译结果(如果有的话)
        slang::CompilerOptionEntry useLatestFileOpt;
        useLatestFileOpt.name = slang::CompilerOptionName::UseUpToDateBinaryModule;
        useLatestFileOpt.value = {slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr};
        mSession->CompilerOpts.push_back(useLatestFileOpt);
        mSession->SessionDesc->compilerOptionEntries = mSession->CompilerOpts.data();
        mSession->SessionDesc->compilerOptionEntryCount = mSession->CompilerOpts.size();

        // 向全局会话绑定
        mGlobalSession->GlobalSession->createSession(*session->SessionDesc, session->Session.writeRef());
    }

    void SlangShaderParser::PrintErrorLog(const Slang::ComPtr<slang::IBlob>& errorLogBlob)
    {
        if (errorLogBlob)
        {
            LOG_ERROR(std::format("[{}] Error occurred when parsing slang code: {}", __FUNCTION__,
                static_cast<const char*>(errorLogBlob->getBufferPointer())));
        }
    }

    ShaderInfo SlangShaderParser::GetShaderInfo(const std::string& modulePath, GraphAPI api)
    {
        return {};
    }

    void SlangShaderParser::TranslateSlangToShader(const std::string& modulePath,
                                                   std::vector<Internal_SlangShaderBlob>& shaderBlobs,
                                                   bool isForCompute)
    {
        if (mModuleMap.contains(modulePath))
        {
            shaderBlobs = mModuleMap[modulePath]->ShaderBlobs;
            return;
        }

        Slang::ComPtr<slang::IBlob> errorLog;

        // 1. 加载Shader代码
        slang::IModule* module = mSession->Session->loadModule(modulePath.c_str(), errorLog.writeRef());
        PrintErrorLog(errorLog);

        // 2. 获取程序入口
        Slang::ComPtr<slang::IEntryPoint> vertexEntry;
        module->findEntryPointByName("vertexMain", vertexEntry.writeRef());
        Slang::ComPtr<slang::IEntryPoint> fragmentEntry;
        module->findEntryPointByName("fragmentMain", fragmentEntry.writeRef());
        Slang::ComPtr<slang::IEntryPoint> geometryEntry;
        module->findEntryPointByName("geometryMain", geometryEntry.writeRef());
        Slang::ComPtr<slang::IEntryPoint> computeEntry;
        module->findEntryPointByName("computeMain", computeEntry.writeRef());

        // 3. 组装着色器
        std::vector<slang::IComponentType *> components = {module};
        // isForCompute == true, shaderBlobs仅包含computeShader; 否则包含vert, geom, frag
        if (isForCompute)
        {
            ZERO_CORE_ASSERT(computeEntry, "use compute shader, but it's null");
            components.emplace_back(computeEntry);
        }
        else
        {
            ZERO_CORE_ASSERT(vertexEntry && fragmentEntry, "vert and frag must exist!");
            components.emplace_back(vertexEntry);
            components.emplace_back(fragmentEntry);
            if (geometryEntry)
            {
                components.emplace_back(geometryEntry);
            }
        }
        Slang::ComPtr<slang::IComponentType> programBeforeLink;
        mSession->Session->createCompositeComponentType(components.data(),
                                                        components.size(),
                                                        programBeforeLink.writeRef(),
                                                        errorLog.writeRef());
        PrintErrorLog(errorLog);

        // 4. 链接着色器
        Slang::ComPtr<slang::IComponentType> programAfterLink;
        programBeforeLink->linkWithOptions(programAfterLink.writeRef(),
                                           mSession->CompilerOpts.size(),
                                           mSession->CompilerOpts.data(),
                                           errorLog.writeRef());
        PrintErrorLog(errorLog);

        // 5. 反射
        // 目前只反射了EntryPoint信息, TODO: 反射全局参数信息
        slang::ProgramLayout* programLayout = programAfterLink->getLayout();
        int entryPointCnt = programLayout->getEntryPointCount();

        // 6. 组装ShaderBlob
        for (int i = 0; i < entryPointCnt; ++i)
        {
            slang::EntryPointLayout* ithEntryPointLayout = programLayout->getEntryPointByIndex(i);

            Internal_SlangShaderBlob blob;
            switch (ithEntryPointLayout->getStage())
            {
                case SLANG_STAGE_VERTEX:
                    blob.stage = ShaderStageFlag::Vertex;
                    break;
                case SLANG_STAGE_FRAGMENT:
                    blob.stage = ShaderStageFlag::Fragment;
                    break;
                case SLANG_STAGE_GEOMETRY:
                    blob.stage = ShaderStageFlag::Geometry;
                    break;
                case SLANG_STAGE_COMPUTE:
                    blob.stage = ShaderStageFlag::Compute;
                    break;
                default:
                    LOG_ERROR(std::format("[{}] Unsupported slang shader stage: {}", __FUNCTION__,
                        static_cast<uint32_t>(ithEntryPointLayout->getStage())));
                    break;
            }

            blob.entryName = ithEntryPointLayout->getName();

            programAfterLink->getEntryPointCode(i, 0, blob.Blob.writeRef(), errorLog.writeRef());
            PrintErrorLog(errorLog);

            shaderBlobs.emplace_back(blob);
        }
    }
} // ZeroEngine
