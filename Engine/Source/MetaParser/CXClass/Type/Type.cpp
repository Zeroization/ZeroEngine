#include <CXClass/Cursor/Cursor.h>
#include <CXClass/Type/Type.h>

namespace MetaParser::CX
{
    Type::Type(const CXType& handle) : m_handle(handle) {}

    std::string Type::GetSpelling() const { return Utils::ToString(clang_getTypeSpelling(m_handle)); }
    CXTypeKind  Type::GetKind() const { return m_handle.kind; }
    int         Type::GetArgumentCount() const { return clang_getNumArgTypes(m_handle); }
    Type        Type::GetArgument(unsigned idx) const { return Type(clang_getArgType(m_handle, idx)); }
    Type        Type::GetCanonicalType() const { return Type(clang_getCanonicalType(m_handle)); }
    Cursor      Type::GetDeclaration() const { return Cursor(clang_getTypeDeclaration(m_handle)); }

    bool Type::IsConst() const { return clang_isConstQualifiedType(m_handle); }
} // namespace MetaParser::CX