#include "include/StringHelpers.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace fs = std::filesystem;

namespace yodecon::helpers {
namespace string {
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
} // namespace string
namespace file {
std::vector<std::string> read_con_file(const std::string_view a_fname) {
  if (fs::exists(a_fname)) {
    std::ifstream file{a_fname};
    if (file.is_open()) {
      auto size = fs::file_size(a_fname);
      std::string content(size, '\0');
      file.seekg(0);
      file.read(&content[0], size);
      file.close();
      std::istringstream ss{content};
      std::string line;
      std::vector<std::string> lines;
      while (std::getline(ss, line)) {
        lines.push_back(line);
      }
      return lines;
    } else {
      throw std::runtime_error("Failed to open the file");
    }
  } else {
    throw std::runtime_error("File not found");
  }
}
} // namespace file
} // namespace yodecon::helpers
