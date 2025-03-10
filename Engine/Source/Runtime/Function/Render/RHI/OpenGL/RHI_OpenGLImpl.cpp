#include "RHI_OpenGLImpl.h"

#include "Function/Render/Window/WindowManager.h"
#include "glad/glad.h"
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

        const char *glVersionStr = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        LOG_INFO(std::format("[{}] OpenGL Version: {}", __FUNCTION__, glVersionStr));
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
