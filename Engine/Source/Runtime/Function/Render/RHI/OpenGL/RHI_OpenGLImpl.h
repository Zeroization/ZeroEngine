#pragma once

#include "Function/Render/RHI/RHI.h"
#include "glad/glad.h"

namespace ZeroEngine
{
    class RHI_OpenGLImpl final : public RHI
    {
    public:
        RHI_OpenGLImpl();

        void BeginFrame() override;
        void EndFrame() override;

        /// 光栅化渲染管线的RHI

        /// 光线追踪渲染管线的RHI

        /// OpenGL拓展
    };

    ZERO_FORCE_INLINE void GLAPIENTRY GLDebugCallback(unsigned source, unsigned type, unsigned id, unsigned severity,
                                                      int length, const char* message, const void* userParam)
    {
        std::stringstream ss;

        ss << "[";
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: ss << "API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: ss << "Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY: ss << "Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION: ss << "Application";
                break;
            case GL_DEBUG_SOURCE_OTHER: ss << "Other";
                break;
            default: ss << "Unknown";
                break;
        }
        ss << "]";

        ss << "[";
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: ss << "Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Deprecated";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: ss << "UB";
                break;
            case GL_DEBUG_TYPE_PORTABILITY: ss << "Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE: ss << "Performance";
                break;
            case GL_DEBUG_TYPE_MARKER: ss << "Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP: ss << "Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP: ss << "Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER: ss << "Other";
                break;
            default: ss << "Unknown";
                break;
        }
        ss << "]" << id << ":";

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                ZERO_CORE_ASSERT(false, "OpenGL error occurred!");
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                LOG_WARN(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LOG_INFO(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_DEBUG(std::format("[{}]{} {}", __FUNCTION__, ss.str(), message));
                break;
        }
    }
} // ZeroEngine
