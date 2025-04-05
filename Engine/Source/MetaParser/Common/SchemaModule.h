#pragma once

#include <Common/pch.h>

namespace MetaParser
{
    class Class;

    struct SchemaModule
    {
        std::string moduleName;

        std::vector<std::shared_ptr<Class>> classes;
    };
} // namespace MetaParser
