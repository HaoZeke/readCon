#include "include/BaseTypes.hpp"
#include "include/Helpers.hpp"
#include "include/ReadCon.hpp"

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    // Check if the file exists
    if (fs::exists(filename)) {
        // Open the file using std::filesystem::ifstream
        std::ifstream file(filename);

        if (file.is_open()) {
            // File successfully opened
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
            file.close();
        } else {
            std::cout << "Failed to open the file." << std::endl;
        }
    } else {
        std::cout << "File not found." << std::endl;
    }
  return EXIT_SUCCESS;
}
