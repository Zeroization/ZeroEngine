#include <CXClass/Cursor/Cursor.h>
#include <CXClass/Type/Type.h>

namespace MetaParser::CX
{
    Cursor::Cursor(const CXCursor& handle) : m_handle(handle) {}

    void Cursor::VisitChildren(ChildrenVisitor visitor, void* data) { clang_visitChildren(m_handle, visitor, data); }

    CXCursorKind Cursor::GetKind() const { return m_handle.kind; }
    Type         Cursor::GetType() const { return Type(clang_getCursorType(m_handle)); }
    Type         Cursor::GetReturnType() const { return Type(clang_getCursorResultType(m_handle)); }
    std::string  Cursor::GetSpelling() const { return Utils::ToString(clang_getCursorSpelling(m_handle)); }
    std::string  Cursor::GetDisplayName() const { return Utils::ToString(clang_getCursorDisplayName(m_handle)); }

    std::string Cursor::GetSrcFilePath() const
    {
        CXSourceLocation location = clang_getCursorLocation(m_handle);

        CXFile   file;
        unsigned line, column, offset;
        clang_getSpellingLocation(location, &file, &line, &column, &offset);

        return Utils::ToString(clang_getFileName(file));
    }

    Cursor::ChildrenCursorList Cursor::GetChildren()
    {
        if (m_children.empty())
        {
            ChildrenVisitor visitor = [](CXCursor cursor, CXCursor parent, CXClientData data) {
                auto list = static_cast<ChildrenCursorList*>(data);
                list->emplace_back(cursor);

                if (cursor.kind == CXCursor_LastPreprocessing)
                {
                    return CXChildVisit_Break;
                }

                return CXChildVisit_Continue;
            };
            clang_visitChildren(m_handle, visitor, &m_children);
        }

        return m_children;
    }

    bool Cursor::IsDefinition() const { return clang_isCursorDefinition(m_handle); }

} // namespace MetaParser::CX