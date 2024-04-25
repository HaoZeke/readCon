// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#include "readCon/include/helpers/StringHelpers.hpp"

namespace yodecon::helpers {
namespace string {
bool isNumber(const std::string &a_token) {
  return std::regex_match(
      a_token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

// Checks if a string is a number.
// Splits a string into constituent strings by whitespace.
std::vector<std::string> get_split_strings(const std::string &a_line) {
  std::istringstream ss{a_line};
  std::vector<std::string> split_strings{std::istream_iterator<std::string>{ss},
                                         std::istream_iterator<std::string>()};
  return split_strings;
}
} // namespace string
} // namespace yodecon::helpers
