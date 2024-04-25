#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
// clang-format off
#include <algorithm>
// clang-format on
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace yodecon {
#ifndef WITH_RANGE_V3
namespace norange {
template <typename T> std::vector<T> take(const std::vector<T> &v, size_t n) {
  std::vector<T> result;
  // Copy up to n elements into result
  std::copy_n(v.begin(), std::min(n, v.size()), std::back_inserter(result));
  return result;
}

template <typename T> std::vector<T> drop(const std::vector<T> &v, size_t n) {
  std::vector<T> result;
  if (n < v.size()) {
    // Copy elements from v starting from the nth element
    std::copy(v.begin() + n, v.end(), std::back_inserter(result));
  }
  return result;
}
} // namespace norange
#endif
namespace helpers {

namespace string {
/**
 * @brief Checks if a given string represents a valid number.
 *
 * This function determines whether a string is a valid numeric format,
 * supporting integer and decimal representations, optionally with leading plus
 * or minus signs.
 *
 * @param a_token The string to check.
 * @return true If the string is a valid number, false otherwise.
 *
 * @details The function uses a regular expression to match the string against a
 * pattern that accepts signed integers and floating-point numbers. It checks
 * for optional leading signs (+ or -), followed by one or more digits, and
 * optionally a decimal point followed by zero or more digits.
 *
 * Example:
 * @code
 * bool result = isNumber("-123.45"); // returns true
 * result = isNumber("abc"); // returns false
 * @endcode
 */
bool isNumber(const std::string &a_token);

/**
 * @brief Splits a string into its constituent parts based on whitespace.
 *
 * This function takes a single string and divides it into a vector of strings,
 * split according to spaces. It is useful for parsing lines of text into words
 * or data entries.
 *
 * @param a_line The string to be split.
 * @return std::vector<std::string> A vector containing each segment of the
 * string that was separated by whitespace.
 *
 * @details Utilizes a string stream to iteratively extract strings separated by
 *          whitespace from the input string, collecting the results in a
 * vector.
 *
 * Example:
 * @code
 * std::vector<std::string> words = get_split_strings("hello world from
 * ChatGPT"); for (const auto& word : words) { std::cout << word << std::endl;
 * // Outputs each word on a new line
 * }
 * @endcode
 */
std::vector<std::string> get_split_strings(const std::string &a_line);

template <typename T>
std::vector<T> get_val_from_string(const std::string &a_line,
                                   std::optional<size_t> a_nelements);
template <typename T, size_t N>
std::array<T, N> get_array_from_string(const std::string &a_line);
} // namespace string

namespace file {
/**
 * @brief Reads the entire contents of a .con file and returns it as a vector of
 * strings.
 *
 * Each string in the returned vector represents one line of the file. This
 * function is primarily used to load and process .con files, which are commonly
 * used in simulation environments to store configuration data.
 *
 * @param a_fname The filename and path of the .con file to be read.
 * @return std::vector<std::string> A vector containing each line of the file as
 * a string.
 *
 * @exception std::runtime_error Thrown if the file cannot be opened or the file
 * does not exist.
 *
 * @details The function first checks if the file exists and then attempts to
 * open it. If successful, it reads the contents into a string and then parses
 * this string into lines which are stored and returned in a vector. This
 * approach ensures that the file is read efficiently, handling large files by
 * reading them completely into memory before processing.
 *
 * Usage Example:
 * @code
 * try {
 *     std::vector<std::string> fileLines = read_con_file("path/to/file.con");
 *     for (const auto& line : fileLines) {
 *         std::cout << line << std::endl;
 *     }
 * } catch (const std::runtime_error& e) {
 *     std::cerr << "Error: " << e.what() << std::endl;
 * }
 * @endcode
 */
std::vector<std::string> read_con_file(const std::string &a_fname);
} // namespace file

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

} // namespace helpers
} // namespace yodecon
