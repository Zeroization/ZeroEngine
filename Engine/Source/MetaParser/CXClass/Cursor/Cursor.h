#pragma once

#include <Common/pch.h>

namespace MetaParser::CX
{
    class Type;

    class Cursor
    {
    public:
        // 当前Cursor节点的所有儿子节点列表
        using ChildrenCursorList = std::vector<Cursor>;
        // 遍历Cursor儿子节点的函数逻辑
        using ChildrenVisitor = CXCursorVisitor;

        explicit Cursor(const CXCursor& handle);

        void VisitChildren(ChildrenVisitor visitor, void* data = nullptr);

    public: // Getters
        // 获取当前节点的类型
        CXCursorKind GetKind() const;
        // 获取当前节点元素的类型
        Type GetType() const;
        // 如果当前节点是函数/方法, 获取返回值类型
        Type GetReturnType() const;
        // 获取当前节点的全名, 例如变量名, 完整函数签名
        std::string GetSpelling() const;
        // 获取当前节点的简短名, 例如函数名
        std::string GetDisplayName() const;
        // 获取当前节点所在的源文件路径
        std::string GetSrcFilePath() const;
        // 获取当前节点的所有儿子节点
        ChildrenCursorList GetChildren();

    public: // IsXXXs
        bool IsDefinition() const;

    private:
        CXCursor m_handle;
        ChildrenCursorList m_children;
    };

} // namespace Metadata::CX