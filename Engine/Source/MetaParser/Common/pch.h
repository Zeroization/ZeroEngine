#pragma once

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <clang-c/Index.h>

namespace fs = std::filesystem;

#include <Common/Config.h>
#include <Util/MetaUtils.h>

#include <inja/inja.hpp>
namespace Inja = inja;