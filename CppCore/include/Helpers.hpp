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
std::vector<T> get_val_from_string(const std::string &a_line,
                                   std::optional<size_t> a_nelements);
template <typename T, size_t N>
std::array<T, N> get_array_from_string(const std::string &a_line);
} // namespace string
namespace file {
std::vector<std::string> read_con_file(const std::string &a_fname);
}
} // namespace yodecon::helpers
