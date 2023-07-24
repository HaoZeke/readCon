#pragma once
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
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

namespace con {

template <typename T> std::string to_string_helper(const T &value) {
  return std::to_string(value);
}

template <>
inline std::string to_string_helper<std::string>(const std::string &value) {
  return value;
}

template <typename Key, typename Value>
std::vector<Value>
convert_keys_to_values(const std::vector<Key> &keys,
                       const std::unordered_map<Key, Value> &map,
                       const std::string &error_message) {
  std::vector<Value> values;
  for (const auto &key : keys) {
    try {
      values.push_back(map.at(key));
    } catch (const std::out_of_range &e) {
      throw std::invalid_argument(error_message + ": " +
                                  yodecon::helpers::con::to_string_helper(key));
    }
  }
  return values;
}
} // namespace con

} // namespace yodecon::helpers
