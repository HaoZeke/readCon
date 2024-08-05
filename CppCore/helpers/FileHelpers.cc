// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>

#include <iostream>
#include <stdexcept>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "readCon/include/Helpers.hpp"

namespace fs = std::filesystem;

namespace yodecon::helpers {
namespace file {
std::vector<std::string> read_con_file(const std::string &a_fname) {
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
