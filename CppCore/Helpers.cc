#include "include/StringHelpers.hpp"

namespace yodecon::helpers::string {
bool isNumber(const std::string &token) {
  return std::regex_match(
      token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

// Checks if a string is a number.
// Splits a string into constituent strings by whitespace.
std::vector<std::string> get_split_strings(const std::string &line) {
  std::istringstream ss{line};
  std::vector<std::string> split_strings{std::istream_iterator<std::string>{ss},
                                         std::istream_iterator<std::string>()};
  return split_strings;
}

} // namespace yodecon::helpers::string
