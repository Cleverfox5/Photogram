#pragma once
#include "Request.hpp"
#include "HttpDef.hpp"
#include <string>
#include <map>
#include <stdint.h>
#include <optional>
#include <variant>

namespace HttpParser
{
e_status parse(std::string_view request, http_types_info& info_block);
};
