#pragma once
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace yodecon::helpers {
namespace string {
bool isNumber(const std::string &token);
std::vector<std::string> get_split_strings(const std::string &line);
} // namespace string
} // namespace yodecon::helpers
