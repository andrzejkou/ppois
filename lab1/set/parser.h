#pragma once

#include "Set.h"
#include <string>

namespace set_parser {
Set parse(const std::string &input);
Element parseElement(const std::string &input);
} // namespace set_parser
