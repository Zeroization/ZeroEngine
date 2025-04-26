#pragma once
#include "pch.h"
#include "glad/glad.h"

namespace ZeroEngine::RHI_GL
{
    // 用于glVertexAttribPointer()的参数
    struct GLVertexBufferElement
    {
        uint32_t glType;
        uint32_t count;
        uint8_t normalized;

        // Ref: https://www.khronos.org/opengl/wiki/OpenGL_Type
        static uint32_t GetSizeOfType(uint32_t glType)
        {
            switch (glType)
            {
                // 8 bit -> 1 byte
                case GL_BYTE:
                case GL_UNSIGNED_BYTE:
                    return 1;

                // 16 bit -> 2 byte
                case GL_SHORT:
                case GL_UNSIGNED_SHORT:
                case GL_HALF_FLOAT:
                    return 2;

                // 32 bit -> 4 byte
                case GL_INT:
                case GL_UNSIGNED_INT:
                case GL_FIXED:
                case GL_FLOAT:
                    return 4;

                // 64 bit -> 8 byte
                case GL_DOUBLE:
                    return 8;

                default: ZERO_CORE_ASSERT(false, "Unknown opengl type!");
                    return 0;
            }
        }
    };

    // 存储VBO中顶点属性的布局
    class GLVertexBufferLayout
    {
    public:
        GLVertexBufferLayout(const std::vector<std::pair<uint32_t, uint32_t>>& glTypeAndElemCntList)
            : mStride(0)
        {
            if (glTypeAndElemCntList.size() > 0)
            {
                for (const auto& [glType, count]: glTypeAndElemCntList)
                {
                    uint8_t normalized = (glType == GL_UNSIGNED_BYTE) ? GL_TRUE : GL_FALSE;
                    mElements.emplace_back(glType, count, normalized);
                    mStride += count * GLVertexBufferElement::GetSizeOfType(glType);
                }
            }
            else
            {
                ZERO_CORE_ASSERT(false, "Try init a zero-param VBO");
            }
        }

        uint32_t GetStride() const { return mStride; }
        const std::vector<GLVertexBufferElement>& GetElements() const { return mElements; }

    private:
        uint32_t mStride;
        std::vector<GLVertexBufferElement> mElements;
    };
}
