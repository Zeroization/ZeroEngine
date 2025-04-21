#include "RHI_OpenGLImpl.h"

#include "Core/GlobalDataManager.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventManager.h"
#include "Core/UUID/UUID.h"
#include "Function/Render/Window/GlfwWindowManager.h"

namespace ZeroEngine
{
    static std::unordered_map<BlendFactor, int> sGlBlendFactorMp = {
        {BlendFactor::Zero, GL_ZERO},
        {BlendFactor::One, GL_ONE},
        {BlendFactor::SrcColor, GL_SRC_COLOR},
        {BlendFactor::OneMinusSrcColor, GL_ONE_MINUS_SRC_COLOR},
        {BlendFactor::DstColor, GL_DST_COLOR},
        {BlendFactor::OneMinusDstColor, GL_ONE_MINUS_DST_COLOR},
        {BlendFactor::SrcAlpha, GL_SRC_ALPHA},
        {BlendFactor::OneMinusSrcAlpha, GL_ONE_MINUS_SRC_ALPHA},
        {BlendFactor::DstAlpha, GL_DST_ALPHA},
        {BlendFactor::OneMinusDstAlpha, GL_ONE_MINUS_DST_ALPHA},
        {BlendFactor::ConstantColor, GL_CONSTANT_COLOR},
        {BlendFactor::OneMinusConstantColor, GL_ONE_MINUS_CONSTANT_COLOR},
        {BlendFactor::ConstantAlpha, GL_CONSTANT_ALPHA},
        {BlendFactor::OneMinusConstantAlpha, GL_ONE_MINUS_CONSTANT_ALPHA}
    };

    static std::unordered_map<FaceCullingOpt, int> sGLFaceCullOptMp = {
        {FaceCullingOpt::Back, GL_BACK},
        {FaceCullingOpt::Front, GL_FRONT},
        {FaceCullingOpt::FrontAndBack, GL_FRONT_AND_BACK}
    };

    RHI_OpenGLImpl::RHI_OpenGLImpl()
    {
        // 加载OpenGL函数指针
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            LOG_CRITICAL(std::format("[{}] Failed to initialize GLAD!!!!", __FUNCTION__));
            return;
        }

        const char* glVersionStr = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        LOG_INFO(std::format("[{}] OpenGL Version: {}", __FUNCTION__, glVersionStr));

#ifdef ZERO_DEBUG_ENABLE
        // 启用更详细的debug输出
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        }
#endif

        // 设置渲染状态
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mTargetState = std::make_unique<RenderState>();
        mCurRealState = std::make_unique<RenderState>();
        mFBOClearInfoMp[0] = {};
    }

    void RHI_OpenGLImpl::BeginFrame()
    {
        if (mIsWindowSizeChanged)
        {
            DoWindowSizeChange();
        }

        // TODO: 升级到渲染管线后删了
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RHI_OpenGLImpl::EndFrame()
    {
        glfwSwapBuffers(static_cast<GLFWwindow *>(WindowManager::GetInstance().GetWindowPtr()));
    }

    void RHI_OpenGLImpl::OnEvent(Event& e)
    {
        if (!e.mIsHandled)
        {
            if (e.mMetadata.Type == EventType::Builtin_WindowResize)
            {
                mNewWindowWidth = e.mArgs["width"].cast<int>();
                mNewWindowHeight = e.mArgs["height"].cast<int>();
                mIsWindowSizeChanged = true;
                e.mIsHandled = true;
            }
        }
    }

    void RHI_OpenGLImpl::SetRenderState(const RenderState& state)
    {
    }

    void RHI_OpenGLImpl::SetViewPort(uint32_t width, uint32_t height)
    {
    }

    void RHI_OpenGLImpl::SwitchFrameBuffer(uint32_t id)
    {
    }

    void RHI_OpenGLImpl::ClearFrameBuffer(FrameBufferClearFlag flags)
    {
    }

    std::shared_ptr<FrameBufferObject> RHI_OpenGLImpl::CreateFBO(FrameBufferType type, uint32_t width, uint32_t height)
    {
        return nullptr;
    }

    std::shared_ptr<FrameBufferObject> RHI_OpenGLImpl::CreateFBO(FrameBufferType type, uint32_t width, uint32_t height,
                                                                 const ClearInfo& clearInfo)
    {
        return nullptr;
    }

    void RHI_OpenGLImpl::DeleteFBO(const std::shared_ptr<FrameBufferObject>& pFBO)
    {
    }

    uint32_t RHI_OpenGLImpl::GetAttachmentTexture(uint32_t id)
    {
        return 0;
    }

    uint32_t RHI_OpenGLImpl::CreateStaticInstanceBuffer(uint32_t size, const void* data)
    {
        return 0;
    }

    uint32_t RHI_OpenGLImpl::CreateDynamicInstanceBuffer(uint32_t size)
    {
        return 0;
    }

    void RHI_OpenGLImpl::UpdateDynamicInstanceBuffer(uint32_t id, uint32_t size, const void* data)
    {
    }

    void RHI_OpenGLImpl::SetInstanceBufferAttributesByVAO(uint32_t VAO, uint32_t id, uint32_t size, uint32_t offset)
    {
    }

    void RHI_OpenGLImpl::DeleteInstanceBuffer(uint32_t id)
    {
    }

    uint32_t RHI_OpenGLImpl::AllocateDrawCommand(RenderCommandType cmdType, FrameBufferClearFlag clearFlags)
    {
        return 0;
    }

    void RHI_OpenGLImpl::DeleteDrawCommand(uint32_t id)
    {
    }

    void RHI_OpenGLImpl::GenerateDrawCommand(uint32_t id)
    {
    }

    void RHI_OpenGLImpl::Draw(uint32_t VAO)
    {
    }

    void RHI_OpenGLImpl::DrawInstanced(uint32_t VAO, uint32_t instanceCnt, uint32_t instanceBuffer)
    {
    }

    unsigned RHI_OpenGLImpl::LoadTexture(const std::string& path, int& width, int& height, bool needFlip)
    {
        return 0;
    }

    unsigned RHI_OpenGLImpl::CreateTexture(const std::shared_ptr<TextureData>& pData)
    {
        return 0;
    }

    unsigned RHI_OpenGLImpl::LoadCubeMap(const std::string& path)
    {
        return 0;
    }

    unsigned RHI_OpenGLImpl::CreateCubeMap(const std::shared_ptr<CubeMapData>& pData)
    {
        return 0;
    }

    void RHI_OpenGLImpl::DeleteTexture(unsigned id)
    {
    }

    UUID32 RHI_OpenGLImpl::CreateMaterialData()
    {
        return UUID32();
    }

    void RHI_OpenGLImpl::SetupMaterial(const std::shared_ptr<Material>& material)
    {
    }

    void RHI_OpenGLImpl::UseMaterialData(UUID32 ID)
    {
    }

    void RHI_OpenGLImpl::DeleteMaterialData(UUID32 ID)
    {
    }

    void RHI_OpenGLImpl::DeleteMesh(unsigned VAO)
    {
    }

    void RHI_OpenGLImpl::SetupStaticMesh(unsigned& VAO, const std::vector<MeshVertex>& vertices,
                                         const std::vector<uint32_t>& indices)
    {
    }

    void RHI_OpenGLImpl::SetupDynamaticMesh(unsigned& VAO, unsigned vertexSize, unsigned indexSize)
    {
    }

    void RHI_OpenGLImpl::UpdateDynamaticMesh(unsigned VAO, const std::vector<MeshVertex>& vertices,
                                             const std::vector<uint32_t>& indices)
    {
    }

    std::shared_ptr<ShaderReference> RHI_OpenGLImpl::LoadAndSetupShader(const std::string& path, FrameBufferType type)
    {
        return nullptr;
    }

    std::shared_ptr<ShaderReference> RHI_OpenGLImpl::SetupShader(const std::string& path, const std::string& shaderCode,
                                                                 FrameBufferType type)
    {
        return nullptr;
    }

    void RHI_OpenGLImpl::DeleteShader(uint32_t id)
    {
    }

    void RHI_OpenGLImpl::UseShader(uint32_t id)
    {
    }

    void RHI_OpenGLImpl::SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       bool value)
    {
    }

    void RHI_OpenGLImpl::SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       float value)
    {
    }

    void RHI_OpenGLImpl::SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       int32_t value)
    {
    }

    void RHI_OpenGLImpl::SetScalarProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       uint32_t value)
    {
    }

    void RHI_OpenGLImpl::SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::vec2& value)
    {
    }

    void RHI_OpenGLImpl::SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::vec3& value)
    {
    }

    void RHI_OpenGLImpl::SetVectorProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::vec4& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat2& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat2x3& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat2x4& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat3& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat3x2& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat3x4& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat4& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat4x2& value)
    {
    }

    void RHI_OpenGLImpl::SetMatrixProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                       const glm::mat4x3& value)
    {
    }

    void RHI_OpenGLImpl::SetTextureProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                        uint32_t texID, bool isBuffer)
    {
    }

    void RHI_OpenGLImpl::SetCubemapProp(const std::shared_ptr<Material>& material, const std::string& propName,
                                        uint32_t texID, bool isBuffer)
    {
    }

    void RHI_OpenGLImpl::DoWindowSizeChange()
    {
        auto gData = GlobalDataManager::GetInstance().GetGlobalDataRef();
#ifdef ZERO_EDITOR
        uint32_t lastScreenWidth = gData->ScreenWidth;
        uint32_t lastScreenHeight = gData->ScreenHeight;
        gData->engineCfg.mWindowWidth = mNewWindowWidth;
        gData->engineCfg.mWindowHeight = mNewWindowHeight;

        if (lastScreenWidth != gData->ScreenWidth || lastScreenHeight != gData->ScreenHeight)
        {
        }
#else
        gData->ScreenWidth = newWindowWidth;
        gData->ScreenHeight = newWindowHeight;

#endif
        mIsWindowSizeChanged = false;
    }

    void RHI_OpenGLImpl::CheckCompileError()
    {
    }
} // ZeroEngine
