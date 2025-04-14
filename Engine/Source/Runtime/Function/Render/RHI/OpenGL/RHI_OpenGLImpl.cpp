#include "RHI_OpenGLImpl.h"

#include "Function/Render/Window/WindowManager.h"
#include "GLFW/glfw3.h"

namespace ZeroEngine
{
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
    }

    void RHI_OpenGLImpl::BeginFrame()
    {
        // TODO: 升级到渲染管线后删了
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RHI_OpenGLImpl::EndFrame()
    {
        glfwSwapBuffers(static_cast<GLFWwindow *>(WindowManager::GetInstance()->GetWindowPtr()));
    }
} // ZeroEngine
