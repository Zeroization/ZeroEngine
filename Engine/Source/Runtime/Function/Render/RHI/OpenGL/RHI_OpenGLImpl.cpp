#include "RHI_OpenGLImpl.h"

#include <stbi/stb_image.h>

#include "Core/GlobalDataManager.h"
#include "Core/Event/EventManager.h"
#include "Core/UUID/UUID.h"
#include "Function/Render/FrameBuffer/FBOManager.h"
#include "Function/Render/Window/GlfwWindowManager.h"

#include <stbi/stb_image.h>

#include "Core/FileSystem/FileSystem.h"
#include "Function/Render/Material/Material.h"
#include "Function/Render/Material/MaterialData.h"
#include "Function/Render/Shader/SlangShaderParser.h"

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
        mTargetState.reset();
        mTargetState = std::make_unique<RenderState>(state);
        mIsStateDirty = true;
    }

    void RHI_OpenGLImpl::SetViewPort(uint32_t width, uint32_t height, uint32_t xOffset, uint32_t yOffset)
    {
        // 视口原点在左下角
        glViewport(xOffset, yOffset, width, height);
    }

    void RHI_OpenGLImpl::SwitchFrameBuffer(uint32_t id)
    {
        // ZERO_BUILTIN_FBO_SCREEN
        if (id == UINT32_MAX)
        {
            mCurFBOID = 0;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else
        {
            mCurFBOID = id;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void RHI_OpenGLImpl::ClearFrameBuffer(FrameBufferClearFlag flags)
    {
        auto& clearInfo = mFBOClearInfoMp[mCurFBOID];
        if (flags & FrameBufferClearFlag::Color)
        {
            ClearColorBuffer(clearInfo.ClearColor);
        }
        if (flags & FrameBufferClearFlag::Depth)
        {
            ClearDepthBuffer(clearInfo.ClearDepth);
        }
        if (flags & FrameBufferClearFlag::Stencil)
        {
            ClearDepthBuffer(clearInfo.ClearStencil);
        }
    }

    void RHI_OpenGLImpl::BlitFrameBuffer(uint32_t cmd, const std::string& srcName, const std::string& dstName,
                                         FrameBufferPieceFlag flags)
    {
        auto srcFBO = FBOManager::GetInstance().GetFBOByName(srcName);
        auto dstFBO = FBOManager::GetInstance().GetFBOByName(dstName);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, srcFBO->mID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFBO->mID);

        if (flags & FrameBufferPieceFlag::Color)
        {
            glBlitFramebuffer(0, 0, srcFBO->mWidth, srcFBO->mHeight, 0, 0, dstFBO->mWidth, dstFBO->mHeight,
                              GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        if (flags & FrameBufferPieceFlag::Depth)
        {
            glBlitFramebuffer(0, 0, srcFBO->mWidth, srcFBO->mHeight, 0, 0, dstFBO->mWidth, dstFBO->mHeight,
                              GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        }

        // 完成工作后别忘了切回当前FBO
        glBindFramebuffer(GL_FRAMEBUFFER, mCurFBOID);
    }

    std::shared_ptr<FrameBufferObject> RHI_OpenGLImpl::CreateFBO(FrameBufferType type, uint32_t width, uint32_t height)
    {
        return CreateFBO(type, width, height, {});
    }

    std::shared_ptr<FrameBufferObject> RHI_OpenGLImpl::CreateFBO(FrameBufferType type, uint32_t width, uint32_t height,
                                                                 const ClearInfo& clearInfo)
    {
        std::shared_ptr<FrameBufferObject> FBO = std::make_shared<FrameBufferObject>(type);
        FBO->mClearInfo = clearInfo;
        FBO->mbIsFitWithScreen = (width == 0 || height == 0);
        FBO->mbIsValid = true;

        width = (width == 0) ? GlobalDataManager::GetInstance().GetGlobalDataRef()->ScreenWidth : width;
        height = (height == 0) ? GlobalDataManager::GetInstance().GetGlobalDataRef()->ScreenHeight : height;
        FBO->mWidth = width;
        FBO->mHeight = height;

        // 创建默认FBO, 带颜色和深度附件
        if (type == FrameBufferType::Normal)
        {
            uint32_t FBOID;
            glGenFramebuffers(1, &FBOID);

            // Color
            uint32_t colorBuffer;
            glGenTextures(1, &colorBuffer);
            glBindTexture(GL_TEXTURE_2D, colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Depth
            uint32_t depthBuffer;
            glGenRenderbuffers(1, &depthBuffer); // 使用只写的renderbuffer
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

            // 绑定Color和Depth附件
            glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                FBO->mbIsValid = false;
                LOG_ERROR(std::format("[{}] FBO is not complete, type: {}", __FUNCTION__,
                    static_cast<uint8_t>(type)));
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            FBO->mID = FBOID;
            FBO->mColorAttachment0ID = colorBuffer;
            FBO->mDepthAttachmentID = depthBuffer;
        }
        // 高精度浮点FBO (高精度Color + depth)
        else if (type == FrameBufferType::HighPrecision)
        {
            uint32_t FBOID;
            glGenFramebuffers(1, &FBOID);

            // 高精度ColorBuffer
            uint32_t colorBuffer;
            glGenTextures(1, &colorBuffer);
            glBindTexture(GL_TEXTURE_2D, colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // depthbuffer
            uint32_t depthBuffer;
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

            // 绑定附件
            glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                FBO->mbIsValid = false;
                LOG_ERROR(std::format("[{}] FBO is not complete, type: {}", __FUNCTION__,
                    static_cast<uint8_t>(type)));
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            FBO->mID = FBOID;
            FBO->mColorAttachment0ID = colorBuffer;
            FBO->mDepthAttachmentID = depthBuffer;
        }
        // 只有Color附件的FBO
        else if (type == FrameBufferType::Color)
        {
            uint32_t FBOID;
            glGenFramebuffers(1, &FBOID);

            // Color
            uint32_t colorBuffer;
            glGenTextures(1, &colorBuffer);
            glBindTexture(GL_TEXTURE_2D, colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 绑定Color附件
            glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                FBO->mbIsValid = false;
                LOG_ERROR(std::format("[{}] FBO is not complete, type: {}", __FUNCTION__,
                    static_cast<uint8_t>(type)));
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            FBO->mID = FBOID;
            FBO->mColorAttachment0ID = colorBuffer;
            FBO->mDepthAttachmentID = UINT32_MAX;
        }
        // 平行光阴影贴图
        else if (type == FrameBufferType::ShadowMap)
        {
            uint32_t FBOID;
            glGenFramebuffers(1, &FBOID);

            // depthMap
            uint32_t depthMap;
            glGenTextures(1, &depthMap);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = {1.0, 1.0, 1.0, 1.0};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

            // 绑定Depth附件
            glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
            // FBO不会渲染到ColorBuffer
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                FBO->mbIsValid = false;
                LOG_ERROR(std::format("[{}] FBO is not complete, type: {}", __FUNCTION__,
                    static_cast<uint8_t>(type)));
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            FBO->mID = FBOID;
            FBO->mColorAttachment0ID = UINT32_MAX;
            FBO->mDepthAttachmentID = depthMap;
        }
        // 点光源阴影贴图
        else if (type == FrameBufferType::ShadowMapCube)
        {
            uint32_t FBOID;
            glGenFramebuffers(1, &FBOID);

            // depthCubeMap
            uint32_t depthCubeMap;
            glGenTextures(1, &depthCubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
            for (uint32_t i = 0; i < 6; ++i)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0,
                             GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            // 绑定深度附件
            glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
            // FBO不会渲染到ColorBuffer
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                FBO->mbIsValid = false;
                LOG_ERROR(std::format("[{}] FBO is not complete, type: {}", __FUNCTION__,
                    static_cast<uint8_t>(type)));
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            FBO->mID = FBOID;
            FBO->mColorAttachment0ID = UINT32_MAX;
            FBO->mDepthAttachmentID = depthCubeMap;
        }
        // 用于延迟渲染管线的G-Buffer
        else if (type == FrameBufferType::GBuffer)
        {
            uint32_t FBOID;
            glGenFramebuffers(1, &FBOID);

            // 位置Position
            unsigned int posBuffer;
            glGenTextures(1, &posBuffer);
            glBindTexture(GL_TEXTURE_2D, posBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 法线Normal
            unsigned int normalBuffer;
            glGenTextures(1, &normalBuffer);
            glBindTexture(GL_TEXTURE_2D, normalBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 颜色Color
            unsigned int colorBuffer;
            glGenTextures(1, &colorBuffer);
            glBindTexture(GL_TEXTURE_2D, colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 深度Depth
            unsigned int depthBuffer;
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

            // 绑定到FBO上
            glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, posBuffer, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalBuffer, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
            // 3个Render Target
            unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
            glDrawBuffers(3, attachments);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                FBO->mbIsValid = false;
                LOG_ERROR(std::format("[{}] FBO is not complete, type: {}", __FUNCTION__,
                    static_cast<uint8_t>(type)));
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // 对FBO对象赋值
            FBO->mID = FBOID;
            FBO->mColorAttachment0ID = colorBuffer;
            FBO->mColorAttachment1ID = posBuffer;
            FBO->mColorAttachment2ID = normalBuffer;
            FBO->mDepthAttachmentID = depthBuffer;
        }
        else
        {
            FBO->mbIsValid = false;
            LOG_ERROR(std::format("[{}] Unsupported FBO type: {}", __FUNCTION__,
                static_cast<uint8_t>(type)));
        }

        mFBOClearInfoMp[FBO->mID] = clearInfo;
        return FBO;
    }

    void RHI_OpenGLImpl::DeleteFBO(const std::shared_ptr<FrameBufferObject>& pFBO)
    {
        mFBOClearInfoMp.erase(pFBO->mID);

        if (pFBO->mColorAttachment0ID != UINT32_MAX)
        {
            glDeleteTextures(1, &pFBO->mColorAttachment0ID);
        }
        if (pFBO->mColorAttachment1ID != UINT32_MAX)
        {
            glDeleteTextures(1, &pFBO->mColorAttachment1ID);
        }
        if (pFBO->mColorAttachment2ID != UINT32_MAX)
        {
            glDeleteTextures(1, &pFBO->mColorAttachment2ID);
        }
        if (pFBO->mDepthAttachmentID != UINT32_MAX)
        {
            if (pFBO->mType == FrameBufferType::Normal || pFBO->mType == FrameBufferType::HighPrecision ||
                pFBO->mType == FrameBufferType::GBuffer)
            {
                glDeleteRenderbuffers(1, &pFBO->mDepthAttachmentID);
            }
            else
            {
                glDeleteTextures(1, &pFBO->mDepthAttachmentID);
            }
        }
        glDeleteFramebuffers(1, &pFBO->mID);
    }

    uint32_t RHI_OpenGLImpl::GetRenderBufferTexture(uint32_t id)
    {
        return id;
    }

    uint32_t RHI_OpenGLImpl::CreateStaticInstanceBuffer(uint32_t size, const void* data)
    {
        uint32_t buffer;
        GLsizeiptr bufferSize = size;

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return buffer;
    }

    uint32_t RHI_OpenGLImpl::CreateDynamicInstanceBuffer(uint32_t size)
    {
        uint32_t buffer;
        GLsizeiptr bufferSize = size;

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return buffer;
    }

    void RHI_OpenGLImpl::UpdateDynamicInstanceBuffer(uint32_t id, uint32_t size, const void* data)
    {
        GLsizeiptr bufferSize = size;

        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void RHI_OpenGLImpl::SetInstanceBufferAttributesByVAO(uint32_t VAO, uint32_t instanceBufferID,
                                                          const std::initializer_list<std::pair<InstanceBufferLayoutType
                                                             ,
                                                              uint32_t>>&
                                                          RHITypeAndElemCntList)
    {
        // 解析RHITypeAndElemCntList
        std::vector<std::pair<uint32_t, uint32_t>> glTypeAndElemCntList;
        for (const auto& [type, cnt]: RHITypeAndElemCntList)
        {
            switch (type)
            {
                case InstanceBufferLayoutType::Float:
                    glTypeAndElemCntList.emplace_back(GL_FLOAT, cnt);
                    break;
                case InstanceBufferLayoutType::UnsignedInt:
                case InstanceBufferLayoutType::Uint32_t:
                    glTypeAndElemCntList.emplace_back(GL_UNSIGNED_INT, cnt);
                    break;
                case InstanceBufferLayoutType::UnsignedChar:
                case InstanceBufferLayoutType::Uint8_t:
                    glTypeAndElemCntList.emplace_back(GL_UNSIGNED_BYTE, cnt);
                    break;
                case InstanceBufferLayoutType::Int32_t:
                case InstanceBufferLayoutType::Int:
                    glTypeAndElemCntList.emplace_back(GL_INT, cnt);
                    break;
                default:
                    LOG_ERROR(std::format("[{}] Unsupported type: {}", __FUNCTION__, static_cast<uint8_t>(type)));
                    break;
            }
        }

        auto meshBuffer = GetVAOByIdx(VAO);
        meshBuffer->Bind();
        meshBuffer->AddVBO(instanceBufferID, {glTypeAndElemCntList});
        meshBuffer->UnBind();
    }

    void RHI_OpenGLImpl::DeleteInstanceBuffer(uint32_t id)
    {
        glDeleteBuffers(1, &id);
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

    /// 一次 Draw Call
    void RHI_OpenGLImpl::Draw(uint32_t VAO)
    {
        UpdateRenderState();
        UpdateMaterialData();

        auto meshBuffer = GetVAOByIdx(VAO);
        meshBuffer->Bind();
        if (meshBuffer->mIndexed)
        {
            glDrawElements(GL_TRIANGLES, meshBuffer->mCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, meshBuffer->mCount);
        }
        meshBuffer->UnBind();
    }

    /// 一次 Draw Call
    void RHI_OpenGLImpl::DrawInstanced(uint32_t VAO, uint32_t instanceCnt, uint32_t instanceBuffer)
    {
        UpdateRenderState();
        UpdateMaterialData();

        auto meshBuffer = GetVAOByIdx(VAO);
        meshBuffer->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        if (meshBuffer->mIndexed)
        {
            glDrawElementsInstanced(GL_TRIANGLES, meshBuffer->mCount, GL_UNSIGNED_INT, 0, instanceCnt);
        }
        else
        {
            glDrawArraysInstanced(GL_TRIANGLES, 0, meshBuffer->mCount, instanceCnt);
        }
        meshBuffer->UnBind();
    }

    uint32_t RHI_OpenGLImpl::LoadTexture(const std::string& path, int& width, int& height, bool needFlip)
    {
        stbi_set_flip_vertically_on_load(needFlip);

        int nrComponents;
        uint8_t* texBuffer = stbi_load(path.c_str(), &width, &height,
                                       &nrComponents,
                                       0);
        if (texBuffer)
        {
            GLenum format = GL_RGB;
            if (nrComponents == 1)
            {
                format = GL_RED;
            }
            else if (nrComponents == 2)
            {
                format = GL_RG;
            }
            else if (nrComponents == 3)
            {
                format = GL_RGB;
            }
            else if (nrComponents == 4)
            {
                format = GL_RGBA;
            }

            uint32_t textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(texBuffer);

            return textureID;
        }
        else
        {
            stbi_image_free(texBuffer);

            if (mMissingTexID == 0)
            {
                std::string missingPath =
                (GlobalDataManager::GetInstance().GetGlobalDataRef()->BuiltinAssetsDir / "Textures" /
                 "missingTexture.jpg").string();

                ZERO_CORE_ASSERT(missingPath != path, "missingPath should not eq path, otherwise it's not ending loop");
                mMissingTexID = LoadTexture(missingPath, width, height, needFlip);
            }
            else
            {
                // TODO: 硬编码, 可能要改
                width = 476;
                height = 476;
            }
            return mMissingTexID;
        }
    }

    uint32_t RHI_OpenGLImpl::CreateTexture(const std::shared_ptr<TextureData>& pData)
    {
        ZERO_CORE_ASSERT(pData && pData->data, "TextureData and its buffer shouldn't be nullptr");

        GLenum format = GL_RGB;
        if (pData->numChannels == 1)
        {
            format = GL_RED;
        }
        else if (pData->numChannels == 2)
        {
            format = GL_RG;
        }
        else if (pData->numChannels == 3)
        {
            format = GL_RGB;
        }
        else if (pData->numChannels == 4)
        {
            format = GL_RGBA;
        }

        uint32_t textureID;
        glGenTextures(GL_TEXTURE_2D, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, format, pData->width, pData->height, 0, format, GL_UNSIGNED_BYTE, pData->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        return textureID;
    }

    uint32_t RHI_OpenGLImpl::LoadCubeMap(const std::vector<std::string>& facePaths)
    {
        ZERO_CORE_ASSERT(facePaths.size() == 6, "Cubemap should have 6 pieces!");

        uint32_t textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrComponents;
        for (uint32_t i = 0; i < facePaths.size(); ++i)
        {
            uint8_t* data = stbi_load(facePaths[i].c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format = GL_RGB;
                if (nrComponents == 1)
                {
                    format = GL_RED;
                }
                else if (nrComponents == 2)
                {
                    format = GL_RG;
                }
                else if (nrComponents == 3)
                {
                    format = GL_RGB;
                }
                else if (nrComponents == 4)
                {
                    format = GL_RGBA;
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE,
                             data);
                stbi_image_free(data);
            }
            else
            {
                LOG_ERROR(std::format("[{}] Can't load cubemap texture, path: {}", __FUNCTION__, facePaths[i]));
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    uint32_t RHI_OpenGLImpl::CreateCubeMap(const std::shared_ptr<CubeMapData>& pData)
    {
        ZERO_CORE_ASSERT(pData && pData->data[0], "CubeMapData and its buffer shouldn't be nullptr")

        uint32_t textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        for (unsigned int i = 0; i < 6; ++i)
        {
            GLenum format = GL_RGB;
            if (pData->numChannels == 1)
            {
                format = GL_RED;
            }
            else if (pData->numChannels == 2)
            {
                format = GL_RG;
            }
            else if (pData->numChannels == 3)
            {
                format = GL_RGB;
            }
            else if (pData->numChannels == 4)
            {
                format = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, pData->width, pData->height, 0, format,
                         GL_UNSIGNED_BYTE, pData->data[i]);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void RHI_OpenGLImpl::DeleteTexture(uint32_t id)
    {
        glDeleteTextures(1, &id);
    }

    uint32_t RHI_OpenGLImpl::CreateMaterialData()
    {
        const uint32_t idx = GetNextMaterialDataIdx();
        auto materialData = GetMaterialDataByIdx(idx);
        materialData->IsInUse = true;
        return idx;
    }

    void RHI_OpenGLImpl::SetupMaterial(Material* material)
    {
        for (const auto& [name, data]: material->mData->mDatas)
        {
            switch (data.first)
            {
                // Scalar
                case ShaderPropertyType::Bool:
                {
                    SetScalarProp(material, name, data.second.cast<bool>());
                    break;
                }
                case ShaderPropertyType::Int:
                {
                    SetScalarProp(material, name, data.second.cast<int32_t>());
                    break;
                }
                case ShaderPropertyType::UInt:
                {
                    SetScalarProp(material, name, data.second.cast<uint32_t>());
                    break;
                }
                case ShaderPropertyType::Float:
                {
                    SetScalarProp(material, name, data.second.cast<float>());
                    break;
                }

                // Vector
                case ShaderPropertyType::Vec2:
                {
                    SetVectorProp(material, name, data.second.cast<glm::vec2>());
                    break;
                }
                case ShaderPropertyType::Vec3:
                {
                    SetVectorProp(material, name, data.second.cast<glm::vec3>());
                    break;
                }
                case ShaderPropertyType::Vec4:
                {
                    SetVectorProp(material, name, data.second.cast<glm::vec4>());
                    break;
                }
                case ShaderPropertyType::IVec2:
                {
                    SetVectorProp(material, name, data.second.cast<glm::ivec2>());
                    break;
                }
                case ShaderPropertyType::IVec3:
                {
                    SetVectorProp(material, name, data.second.cast<glm::ivec3>());
                    break;
                }
                case ShaderPropertyType::IVec4:
                {
                    SetVectorProp(material, name, data.second.cast<glm::ivec4>());
                    break;
                }
                case ShaderPropertyType::UVec2:
                {
                    SetVectorProp(material, name, data.second.cast<glm::uvec2>());
                    break;
                }
                case ShaderPropertyType::UVec3:
                {
                    SetVectorProp(material, name, data.second.cast<glm::uvec3>());
                    break;
                }
                case ShaderPropertyType::UVec4:
                {
                    SetVectorProp(material, name, data.second.cast<glm::uvec4>());
                    break;
                }
                default:
                    LOG_ERROR(
                        std::format("[{}] Unsupported type: {}, shouldn't be mat, or sampler", __FUNCTION__, static_cast
                            <uint8_t>(data.first)));
                    break;
            }

            material->mData->bIsInitialized = true;
        }
    }

    void RHI_OpenGLImpl::UseMaterialData(uint32_t ID)
    {
        mCurMaterialDataID = ID;
    }

    void RHI_OpenGLImpl::DeleteMaterialData(uint32_t ID)
    {
        auto materialData = GetMaterialDataByIdx(ID);
        materialData->properties.clear();
        materialData->textures.clear();
        materialData->cubemaps.clear();
        materialData->IsInUse = false;
    }

    void RHI_OpenGLImpl::DeleteMesh(unsigned VAO)
    {
        auto meshBuffer = GetVAOByIdx(VAO);
        if (meshBuffer->mVBO)
        {
            glDeleteBuffers(1, &meshBuffer->mVBO);
        }
        if (meshBuffer->mEBO)
        {
            glDeleteBuffers(1, &meshBuffer->mEBO);
        }
        glDeleteVertexArrays(1, &meshBuffer->mID);
        meshBuffer->mInUse = false;
    }

    void RHI_OpenGLImpl::SetupStaticMesh(unsigned& VAO, const std::vector<MeshVertex>& vertices,
                                         const std::vector<uint32_t>& indices)
    {
        VAO = GetNextVAOIdx();
        auto meshBuffer = GetVAOByIdx(VAO);
        meshBuffer->mIndexed = true;
        meshBuffer->mCount = static_cast<uint32_t>(indices.size());

        glGenVertexArrays(1, &meshBuffer->mID);
        glGenBuffers(1, &meshBuffer->mVBO);
        glGenBuffers(1, &meshBuffer->mEBO);

        glBindVertexArray(meshBuffer->mID);
        // 加载顶点信息
        glBindBuffer(GL_ARRAY_BUFFER, meshBuffer->mVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), vertices.data(), GL_STATIC_DRAW);
        // 加载索引信息
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBuffer->mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        // 设置一组顶点的布局
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, Position)));
        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, Normal)));
        // texcoord
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, TexCoords)));
        // tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, Tangent)));
        // bone weights
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, BoneWeights)));
        // bone indices
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(MeshVertex),
                               reinterpret_cast<void *>(offsetof(MeshVertex, BoneIDs)));

        glBindVertexArray(0);

        meshBuffer->mInUse = true;
    }

    void RHI_OpenGLImpl::SetupDynamaticMesh(unsigned& VAO, unsigned vertexSize, unsigned indexSize)
    {
        VAO = GetNextVAOIdx();
        auto meshBuffer = GetVAOByIdx(VAO);
        meshBuffer->mIndexed = true;
        meshBuffer->mCount = indexSize;

        glGenVertexArrays(1, &meshBuffer->mID);
        glGenBuffers(1, &meshBuffer->mVBO);
        glGenBuffers(1, &meshBuffer->mEBO);

        glBindVertexArray(meshBuffer->mID);
        glBindBuffer(GL_ARRAY_BUFFER, meshBuffer->mVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(MeshVertex), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBuffer->mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, Position)));
        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, Normal)));
        // texcoord
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, TexCoords)));
        // tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, Tangent)));
        // bone weights
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, BoneWeights)));
        // bone indices
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(MeshVertex),
                               reinterpret_cast<void *>(offsetof(MeshVertex, BoneIDs)));
        glBindVertexArray(0);

        meshBuffer->mInUse = true;
    }

    void RHI_OpenGLImpl::UpdateDynamaticMesh(unsigned VAO, const std::vector<MeshVertex>& vertices,
                                             const std::vector<uint32_t>& indices)
    {
        auto meshBuffer = GetVAOByIdx(VAO);

        glBindVertexArray(meshBuffer->mID);
        glBindBuffer(GL_ARRAY_BUFFER, meshBuffer->mVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(MeshVertex), vertices.data());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBuffer->mEBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(uint32_t), indices.data());
        glBindVertexArray(0);
    }

    std::shared_ptr<ShaderReference> RHI_OpenGLImpl::LoadAndSetupShader(const std::string& path, FrameBufferType type)
    {
        return SetupShader(path, {}, type);
    }

    std::shared_ptr<ShaderReference> RHI_OpenGLImpl::SetupShader(const std::string& path, const std::string& shaderCode,
                                                                 FrameBufferType type)
    {
        std::vector<ShaderBlob> blobs;
        if (!shaderCode.empty())
        {
            ZERO_CORE_ASSERT(false, "TODO: do slang::loadModuleFromSourceString() in SlangShaderParse::");
        }
        else
        {
            SlangShaderParser::GetInstance().TranslateSlangToShader(path, blobs);
        }

        if (blobs.empty())
        {
            LOG_ERROR(std::format("[{}] Get nothing from slang code: {}", __FUNCTION__, path));
            return nullptr;
        }

        std::vector<uint32_t> shaderIDs;
        for (const auto& blob: blobs)
        {
            ZERO_CORE_ASSERT((blob.buffer == nullptr || blob.entryName || blob.bufferSize <= 0),
                             "They shouldn't be null");

            uint32_t shaderID = 0;
            switch (blob.stage)
            {
                case ShaderStageFlag::Vertex:
                    shaderID = glCreateShader(GL_VERTEX_SHADER);
                    break;
                case ShaderStageFlag::Fragment:
                    shaderID = glCreateShader(GL_FRAGMENT_SHADER);
                    break;
                case ShaderStageFlag::Geometry:
                    shaderID = glCreateShader(GL_GEOMETRY_SHADER);
                    break;
                default:
                    LOG_ERROR(
                        std::format("[{}] Unknown shader stage {}, path: {}", __FUNCTION__, static_cast<uint16_t>(blob.
                            stage), path));
                    glDeleteShader(shaderID);
                    return nullptr;
            }
            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V,
                           blob.buffer,
                           blob.bufferSize);

            int32_t binaryStatus;
            glGetShaderiv(shaderID, GL_SHADER_BINARY_FORMATS, &binaryStatus);
            if (binaryStatus == GL_FALSE)
            {
                LOG_ERROR(
                    std::format("[{}] Failed to load SPIR-V binary shader, path: {}, stage: {}", __FUNCTION__, path,
                        static_cast<uint16_t>(blob.stage)));
                glDeleteShader(shaderID);
                return nullptr;
            }

            glSpecializeShader(shaderID, blob.entryName, 0, nullptr, nullptr);
            int32_t success;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char errorLog[1024];
                glGetShaderInfoLog(shaderID, 1024, nullptr, errorLog);
                LOG_ERROR(std::format("[{}] Can't compile shader, path:{}, stage: {}", __FUNCTION__, path,
                    static_cast<uint16_t>(blob.stage)));
                glDeleteShader(shaderID);
                return nullptr;
            }
            shaderIDs.push_back(shaderID);
        }

        uint32_t program = glCreateProgram();
        for (auto shaderID: shaderIDs)
        {
            glAttachShader(program, shaderID);
        }

        glLinkProgram(program);
        int32_t linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            char errorLog[1024];
            glGetShaderInfoLog(program, 1024, nullptr, errorLog);
            LOG_ERROR(std::format("[{}] Can't link shader program, path:{}", __FUNCTION__, path));
            glDeleteProgram(program);
            return nullptr;
        }

        for (auto shaderID: shaderIDs)
        {
            glDeleteShader(shaderID);
        }

        mShaderToMaterialData[program] = std::make_shared<GLMaterialData>();
        std::shared_ptr<ShaderReference> reference = std::make_shared<ShaderReference>();
        reference->id = program;
        reference->info = SlangShaderParser::GetInstance().GetShaderInfo(path, GraphAPI::OpenGL);
        reference->targetFrameBufferTy = type;
        return reference;
    }

    void RHI_OpenGLImpl::DeleteShader(uint32_t id)
    {
        mShaderToMaterialData[id].reset();
        mShaderToMaterialData.erase(id);
        glDeleteProgram(id);
    }

    void RHI_OpenGLImpl::UseShader(uint32_t id)
    {
        mCurShaderID = id;
        glUseProgram(id);
    }

    void RHI_OpenGLImpl::SetScalarProp(Material* material, const std::string& propName,
                                       bool value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Bool, value};
    }

    void RHI_OpenGLImpl::SetScalarProp(Material* material, const std::string& propName,
                                       float value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Float, value};
    }

    void RHI_OpenGLImpl::SetScalarProp(Material* material, const std::string& propName,
                                       int32_t value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Int, value};
    }

    void RHI_OpenGLImpl::SetScalarProp(Material* material, const std::string& propName,
                                       uint32_t value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::UInt, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::vec2& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Vec2, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::vec3& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Vec3, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::vec4& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Vec4, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::ivec2& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::IVec2, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::ivec3& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::IVec3, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::ivec4& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::IVec4, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::uvec2& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::UVec2, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::uvec3& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::UVec3, value};
    }

    void RHI_OpenGLImpl::SetVectorProp(Material* material, const std::string& propName,
                                       const glm::uvec4& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::UVec4, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat2& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat2x2, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat2x3& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat2x3, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat2x4& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat2x4, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat3& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat3x3, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat3x2& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat3x2, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat3x4& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat3x4, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat4& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat4x4, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat4x2& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat4x2, value};
    }

    void RHI_OpenGLImpl::SetMatrixProp(Material* material, const std::string& propName,
                                       const glm::mat4x3& value)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->properties[propName] = {ShaderPropertyType::Mat4x3, value};
    }

    void RHI_OpenGLImpl::SetTextureProp(Material* material, const std::string& propName,
                                        uint32_t texID, uint32_t texIdx, bool isBuffer)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->textures[propName] = {texID, texIdx};
    }

    void RHI_OpenGLImpl::SetCubemapProp(Material* material, const std::string& propName,
                                        uint32_t texID, uint32_t texIdx, bool isBuffer)
    {
        auto materialData = GetMaterialDataByIdx(material->mData->GetID());
        materialData->cubemaps[propName] = {texID, texIdx};
    }

    uint32_t RHI_OpenGLImpl::CreateSSBO(const void* data, size_t size, GPUBufferType type)
    {
        uint32_t buffer;

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
        switch (type)
        {
            case GPUBufferType::StaticGPURead:
                glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
                break;
            case GPUBufferType::DynamicGPURead:
                glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_DRAW);
                break;
            case GPUBufferType::DynamicGPUWrite:
                glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_READ);
                break;
            case GPUBufferType::DynamicGPUReadWrite:
                glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_COPY);
                break;
        }

        return buffer;
    }

    void RHI_OpenGLImpl::BindSSBO(uint32_t id, uint32_t binding)
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);
    }

    void RHI_OpenGLImpl::UpdateSSBO(uint32_t id, const void* data, size_t size)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, static_cast<GLsizeiptr>(size), data);
    }

    void RHI_OpenGLImpl::DeleteSSBO(uint32_t id)
    {
        glDeleteBuffers(1, &id);
    }

    void RHI_OpenGLImpl::BindWithVAO(uint32_t VAO, uint32_t binding)
    {
        auto meshBuffer = GetVAOByIdx(VAO);
        BindSSBO(meshBuffer->mVBO, binding);
    }

    std::shared_ptr<ComputeShaderReference> RHI_OpenGLImpl::LoadAndSetupComputeShader(const std::string& path)
    {
        std::vector<ShaderBlob> blobs;
        SlangShaderParser::GetInstance().TranslateSlangToShader(path, blobs, true);

        if (blobs.empty())
        {
            LOG_ERROR(std::format("[{}] Get nothing from slang code: {}", __FUNCTION__, path));
            return nullptr;
        }
        ZERO_CORE_ASSERT(blobs.size() == 1, "Should have only one compute shader");

        if (blobs.front().stage != ShaderStageFlag::Compute)
        {
            LOG_ERROR(
                std::format("[{}] Get wrong stage({}) from slang code: {}", __FUNCTION__, static_cast<uint16_t>(blobs.
                    front().stage), path));
            return nullptr;
        }

        uint32_t computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

        glShaderBinary(1, &computeShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V,
                       blobs.front().buffer,
                       blobs.front().bufferSize);
        int32_t binaryStatus;
        glGetShaderiv(computeShaderID, GL_SHADER_BINARY_FORMATS, &binaryStatus);
        if (binaryStatus == GL_FALSE)
        {
            LOG_ERROR(std::format("[{}] Failed to load SPIR-V binary, path: {}", __FUNCTION__, path));
            glDeleteShader(computeShaderID);
            return nullptr;
        }

        glSpecializeShader(computeShaderID, blobs.front().entryName, 0, nullptr, nullptr);
        int32_t specializeStatus;
        glGetShaderiv(computeShaderID, GL_COMPILE_STATUS, &specializeStatus);
        if (!specializeStatus)
        {
            char errorLog[1024];
            glGetShaderInfoLog(computeShaderID, 1024, nullptr, errorLog);
            LOG_ERROR(std::format("[{}] Can't compile compute shader, path: {}", __FUNCTION__, path));
            glDeleteShader(computeShaderID);
            return nullptr;
        }

        uint32_t programID = glCreateProgram();
        glAttachShader(programID, computeShaderID);

        glLinkProgram(programID);
        int32_t linkStatus;
        glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            char errorLog[1024];
            glGetShaderInfoLog(programID, 1024, nullptr, errorLog);
            LOG_ERROR(std::format("[{}] Can't link compute shader program, path:{}", __FUNCTION__, path));
            glDeleteProgram(programID);
            return nullptr;
        }
        glDeleteShader(computeShaderID);

        std::shared_ptr<ComputeShaderReference> reference = std::make_shared<ComputeShaderReference>();
        reference->id = programID;
        reference->path = path;
        //TODO: reference->shaderInfo =

        return reference;
    }

    void RHI_OpenGLImpl::DeleteComputeShader(uint32_t id)
    {
        glDeleteProgram(id);
    }

    void RHI_OpenGLImpl::DispatchComputeCommand(uint32_t commandID, uint32_t shaderID, uint32_t groupX, uint32_t groupY,
                                                uint32_t groupZ)
    {
        glUseProgram(shaderID);
        glDispatchCompute(groupX, groupY, groupZ);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
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
            FBOManager::GetInstance().RecreateAllFitWindowFBO();
        }
#else
        gData->ScreenWidth = newWindowWidth;
        gData->ScreenHeight = newWindowHeight;
        FBOManager::GetInstance().RecreateAllFitWindowFBO();
#endif
        mIsWindowSizeChanged = false;
    }

    void RHI_OpenGLImpl::UpdateRenderState()
    {
        if (mIsStateDirty)
        {
            mIsStateDirty = false;
        }
        else
        {
            return;
        }

        if (mCurRealState->enableDepthTest != mTargetState->enableDepthTest)
        {
            if (mTargetState->enableDepthTest)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }

        if (mCurRealState->enableDepthWrite != mTargetState->enableDepthWrite)
        {
            if (mTargetState->enableDepthWrite)
            {
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDepthMask(GL_FALSE);
            }
        }

        if (mCurRealState->srcFactor != mTargetState->srcFactor ||
            mCurRealState->dstFactor != mTargetState->dstFactor)
        {
            glBlendFunc(sGlBlendFactorMp[mTargetState->srcFactor], sGlBlendFactorMp[mTargetState->dstFactor]);
        }

        if (mCurRealState->enableFaceCulling != mTargetState->enableFaceCulling)
        {
            if (mTargetState->enableFaceCulling)
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }

        if (mCurRealState->faceCullingOption != mTargetState->faceCullingOption)
        {
            glCullFace(sGLFaceCullOptMp[mTargetState->faceCullingOption]);
        }

        if (mCurRealState->clearInfo.ClearColor != mTargetState->clearInfo.ClearColor)
        {
            glClearColor(mTargetState->clearInfo.ClearColor.r,
                         mTargetState->clearInfo.ClearColor.g,
                         mTargetState->clearInfo.ClearColor.b,
                         mTargetState->clearInfo.ClearColor.a);
        }

        if (mCurRealState->clearInfo.ClearDepth != mTargetState->clearInfo.ClearDepth)
        {
            glClearDepth(mTargetState->clearInfo.ClearDepth);
        }

        if (mCurRealState->clearInfo.ClearStencil != mTargetState->clearInfo.ClearStencil)
        {
            glClearStencil(mTargetState->clearInfo.ClearStencil);
        }

        mCurRealState.swap(mTargetState);
    }

    void RHI_OpenGLImpl::UpdateMaterialData()
    {
        auto shaderData = mShaderToMaterialData[mCurShaderID];
        auto materialData = GetMaterialDataByIdx(mCurMaterialDataID);

        for (auto& [propName, propTyAndData]: materialData->properties)
        {
            bool shouldUpdate = false;
            if (!shaderData->properties.contains(propName) ||
                shaderData->properties[propName].second != propTyAndData)
            {
                shouldUpdate = true;
            }

            if (shouldUpdate)
            {
                shaderData->properties[propName] = propTyAndData;

                switch (propTyAndData.first)
                {
                    case ShaderPropertyType::Int:
                        SetUniform(propName, propTyAndData.second.cast<int32_t>());
                        break;
                    case ShaderPropertyType::Bool:
                        SetUniform(propName, propTyAndData.second.cast<bool>());
                        break;
                    case ShaderPropertyType::Float:
                        SetUniform(propName, propTyAndData.second.cast<float>());
                        break;
                    case ShaderPropertyType::UInt:
                        SetUniform(propName, propTyAndData.second.cast<uint32_t>());
                        break;
                    case ShaderPropertyType::Vec2:
                        SetUniform(propName, propTyAndData.second.cast<glm::vec2>());
                        break;
                    case ShaderPropertyType::Vec3:
                        SetUniform(propName, propTyAndData.second.cast<glm::vec3>());
                        break;
                    case ShaderPropertyType::Vec4:
                        SetUniform(propName, propTyAndData.second.cast<glm::vec4>());
                        break;
                    case ShaderPropertyType::IVec2:
                        SetUniform(propName, propTyAndData.second.cast<glm::ivec2>());
                        break;
                    case ShaderPropertyType::IVec3:
                        SetUniform(propName, propTyAndData.second.cast<glm::ivec3>());
                        break;
                    case ShaderPropertyType::IVec4:
                        SetUniform(propName, propTyAndData.second.cast<glm::ivec4>());
                        break;
                    case ShaderPropertyType::UVec2:
                        SetUniform(propName, propTyAndData.second.cast<glm::uvec2>());
                        break;
                    case ShaderPropertyType::UVec3:
                        SetUniform(propName, propTyAndData.second.cast<glm::uvec3>());
                        break;
                    case ShaderPropertyType::UVec4:
                        SetUniform(propName, propTyAndData.second.cast<glm::uvec4>());
                        break;
                    case ShaderPropertyType::Mat2x2:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat2>());
                        break;
                    case ShaderPropertyType::Mat2x3:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat2x3>());
                        break;
                    case ShaderPropertyType::Mat2x4:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat2x4>());
                        break;
                    case ShaderPropertyType::Mat3x2:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat3x2>());
                        break;
                    case ShaderPropertyType::Mat3x3:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat3>());
                        break;
                    case ShaderPropertyType::Mat3x4:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat3x4>());
                        break;
                    case ShaderPropertyType::Mat4x2:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat4x2>());
                        break;
                    case ShaderPropertyType::Mat4x3:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat4x3>());
                        break;
                    case ShaderPropertyType::Mat4x4:
                        SetUniform(propName, propTyAndData.second.cast<glm::mat4>());
                        break;
                    default:
                        LOG_ERROR(
                            std::format("[{}] Unsupported type: {}", __FUNCTION__, static_cast<uint16_t>(propTyAndData.
                                first)));
                        break;
                }
            }
        }

        for (auto& [texName, ids]: materialData->textures)
        {
            DoSetShaderTexture(texName, ids[0], ids[1]);
        }

        for (auto& [cubeMapName, ids]: materialData->cubemaps)
        {
            DOSetShaderCubemap(cubeMapName, ids[0], ids[1]);
        }
    }

    void RHI_OpenGLImpl::ClearColorBuffer(const glm::vec4& color)
    {
        if (mCurRealState->clearInfo.ClearColor != color)
        {
            glClearColor(color.r, color.g, color.b, color.a);
            mCurRealState->clearInfo.ClearColor = color;
            mIsStateDirty = true;
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RHI_OpenGLImpl::ClearDepthBuffer(float depth)
    {
        // ClearDepthBuffer的前提是: 开启深度测试与深度写入
        if (!mCurRealState->enableDepthTest)
        {
            glEnable(GL_DEPTH_TEST);
            mCurRealState->enableDepthTest = true;
            mIsStateDirty = true;
        }
        if (!mCurRealState->enableDepthWrite)
        {
            glDepthMask(GL_TRUE);
            mCurRealState->enableDepthWrite = true;
            mIsStateDirty = true;
        }

        // 刷新DepthBuffer
        if (mCurRealState->clearInfo.ClearDepth != depth)
        {
            glClearDepth(depth);
            mCurRealState->clearInfo.ClearDepth = depth;
            mIsStateDirty = true;
        }
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void RHI_OpenGLImpl::ClearStencilBuffer(uint32_t stencil)
    {
        if (mCurRealState->clearInfo.ClearStencil != stencil)
        {
            glClearStencil(static_cast<GLint>(stencil));
            mCurRealState->clearInfo.ClearStencil = stencil;
            mIsStateDirty = true;
        }
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void RHI_OpenGLImpl::DoSetShaderTexture(const std::string& name, uint32_t ID, uint32_t idx)
    {
        glUniform1i(glGetUniformLocation(mCurShaderID, name.c_str()), idx);
        glActiveTexture(GL_TEXTURE0 + idx);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void RHI_OpenGLImpl::DOSetShaderCubemap(const std::string& name, uint32_t ID, uint32_t idx)
    {
        glUniform1i(glGetUniformLocation(mCurShaderID, name.c_str()), idx);
        glActiveTexture(GL_TEXTURE0 + idx);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    }

    uint32_t RHI_OpenGLImpl::GetNextVAOIdx()
    {
        for (size_t i = 0; i < mGLVAOArray.size(); ++i)
        {
            if (!mGLVAOArray[i]->mInUse)
                return i;
        }

        mGLVAOArray.push_back(std::make_shared<GLVertexArray>());
        return mGLVAOArray.size() - 1;
    }

    std::shared_ptr<GLVertexArray> RHI_OpenGLImpl::GetVAOByIdx(uint32_t idx)
    {
        return mGLVAOArray.at(idx);
    }

    uint32_t RHI_OpenGLImpl::GetNextMaterialDataIdx()
    {
        for (size_t i = 0; i < mGLMaterialDataArray.size(); ++i)
        {
            if (!mGLMaterialDataArray[i]->IsInUse)
                return i;
        }

        mGLMaterialDataArray.push_back(std::make_shared<GLMaterialData>());
        return mGLMaterialDataArray.size() - 1;
    }

    std::shared_ptr<GLMaterialData> RHI_OpenGLImpl::GetMaterialDataByIdx(uint32_t idx)
    {
        return mGLMaterialDataArray.at(idx);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<unsigned>(const std::string& name, const uint32_t& value) const
    {
        glUniform1i(glGetUniformLocation(mCurShaderID, name.c_str()), value);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<int>(const std::string& name, const int& value) const
    {
        glUniform1i(glGetUniformLocation(mCurShaderID, name.c_str()), value);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<bool>(const std::string& name, const bool& value) const
    {
        glUniform1i(glGetUniformLocation(mCurShaderID, name.c_str()), value ? GL_TRUE : GL_FALSE);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<float>(const std::string& name, const float& value) const
    {
        glUniform1f(glGetUniformLocation(mCurShaderID, name.c_str()), value);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<2, float>>(const std::string& name, const glm::vec2& value) const
    {
        glUniform2f(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<3, float>>(const std::string& name, const glm::vec3& value) const
    {
        glUniform3f(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y, value.z);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<4, float>>(const std::string& name, const glm::vec4& value) const
    {
        glUniform4f(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<2, int>>(const std::string& name, const glm::ivec2& value) const
    {
        glUniform2i(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<3, int>>(const std::string& name, const glm::ivec3& value) const
    {
        glUniform3i(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y, value.z);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<4, int>>(const std::string& name, const glm::ivec4& value) const
    {
        glUniform4i(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<2, unsigned>>(const std::string& name, const glm::uvec2& value) const
    {
        glUniform2ui(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<3, unsigned>>(const std::string& name, const glm::uvec3& value) const
    {
        glUniform3ui(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y, value.z);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::vec<4, unsigned>>(const std::string& name, const glm::uvec4& value) const
    {
        glUniform4ui(glGetUniformLocation(mCurShaderID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<2, 2, float>>(const std::string& name, const glm::mat2& value) const
    {
        glUniformMatrix2fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<2, 3, float>>(const std::string& name, const glm::mat2x3& value) const
    {
        glUniformMatrix2x3fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<2, 4, float>>(const std::string& name, const glm::mat2x4& value) const
    {
        glUniformMatrix2x4fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<3, 3, float>>(const std::string& name, const glm::mat3& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<3, 2, float>>(const std::string& name, const glm::mat3x2& value) const
    {
        glUniformMatrix3x2fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<3, 4, float>>(const std::string& name, const glm::mat3x4& value) const
    {
        glUniformMatrix3x4fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<4, 2, float>>(const std::string& name, const glm::mat4x2& value) const
    {
        glUniformMatrix4x2fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<4, 3, float>>(const std::string& name, const glm::mat4x3& value) const
    {
        glUniformMatrix4x3fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    template <>
    void RHI_OpenGLImpl::SetUniform<glm::mat<4, 4, float>>(const std::string& name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(mCurShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
} // ZeroEngine
