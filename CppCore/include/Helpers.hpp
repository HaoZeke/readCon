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
bool isNumber(const std::string &a_token);
std::vector<std::string> get_split_strings(const std::string &a_line);

template <typename T>
std::vector<T> get_val_from_string(const std::string &line,
                                   std::optional<size_t> a_nelements);
} // namespace string
namespace file {
std::vector<std::string> read_con_file(const std::string_view a_fname);
}
} // namespace yodecon::helpers
