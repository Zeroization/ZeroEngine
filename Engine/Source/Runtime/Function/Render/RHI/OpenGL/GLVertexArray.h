#pragma once

namespace ZeroEngine::RHI_GL
{
    class GLVertexBufferLayout;

    class GLVertexArray
    {
    public:
        GLVertexArray();
        virtual ~GLVertexArray();

        void AddVBO(uint32_t VBO, const GLVertexBufferLayout& layout);
        void Bind() const;
        void UnBind() const;

    public:
        uint32_t mID;
        uint32_t mVBO;
        uint32_t mEBO;
        uint32_t mCount; ///< 如果有EBO, 就是EBO元素数量; 否则为顶点数组大小
        bool mIndexed = true;
        bool mInUse = false;
    };
} // ZeroEngine::RHI_GL
