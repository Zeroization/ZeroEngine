#include "GLVertexArray.h"
#include "GLVertexBufferLayout.hpp"
#include "glad/glad.h"

namespace ZeroEngine::RHI_GL
{
    GLVertexArray::GLVertexArray()
    {
    }

    GLVertexArray::~GLVertexArray()
    {
    }

    void GLVertexArray::AddVBO(uint32_t VBO, const GLVertexBufferLayout& layout)
    {
        Bind();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        const auto& elements = layout.GetElements();
        uint32_t offset = 0;
        for (uint32_t i = 0; i < elements.size(); ++i)
        {
            const auto& element = elements[i];

            glEnableVertexAttribArray(i);
            switch (element.glType)
            {
                case GL_INT:
                    glVertexAttribIPointer(i, element.count, element.glType, layout.GetStride(),
                                           reinterpret_cast<const void *>(offset));
                    break;
                default:
                    glVertexAttribPointer(i, element.count, element.glType, element.normalized, layout.GetStride(),
                                          reinterpret_cast<const void *>(offset));
                    break;
            }
            glVertexAttribDivisor(i, 1);

            offset += element.count * GLVertexBufferElement::GetSizeOfType(element.glType);
        }

        if (VBO != mVBO)
        {
            mVBO = VBO;
        }
    }

    void GLVertexArray::Bind() const
    {
        glBindVertexArray(mID);
    }

    void GLVertexArray::UnBind() const
    {
        glBindVertexArray(0);
    }
} // ZeroEngine::RHI_GL
