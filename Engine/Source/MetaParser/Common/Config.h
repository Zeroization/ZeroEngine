#pragma once

namespace MetaParser::AnnotateAttr
{
    // 用于识别的AnnotateAttr标记, 例如:
    // #define ZE_CLASS() [[clang::annotate("xxx")]] 的标记就是xxx
    const auto ClassAnnotateAttr    = "reflect-class";
    const auto StructAnnotateAttr   = "reflect-struct";
    const auto EnumAnnotateAttr     = "reflect-enum";
    const auto PropertyAnnotateAttr = "reflect-property";
    const auto FunctionAnnotateAttr = "reflect-function";
} // namespace MetaParser::AnnotateAttr

// AnnotateAttr用到的基础Tag, 例如:
// ZE_CLASS(a; b) 的Tag就是a和b
namespace MetaParser::BasicTag
{
    // Class

    // Struct

    // Enum

    // Property
    // 分类, 用于在ImGUI中显示自定义分页
    const auto Category = "Category";

    // Function

} // namespace MetaParser::BasicTag
