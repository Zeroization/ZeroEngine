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
    }

    void RHI_OpenGLImpl::EndFrame()
    {
        glfwSwapBuffers(static_cast<GLFWwindow *>(WindowManager::GetInstance()->GetWindowPtr()));
    }
} // ZeroEngine
