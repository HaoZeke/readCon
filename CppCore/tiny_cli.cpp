#include "Helpers.hpp"
#include "include/BaseTypes.hpp"
#include "include/ReadCon.hpp"
#include "include/StringHelpers.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  // std::array<size_t, 4> header_linum = {1, 2, 5, 6};
  // const size_t boxl_linum = 3;
  // const size_t angle_linum = 4;
  // const size_t natom_types_linum = 7;
  // const size_t natoms_per_type_linum = 8;
  // const size_t masses_per_type_linum = 9;

  auto fconts = yodecon::helpers::file::read_con_file(filename);

  for (auto &&line : fconts) {
    std::cout << line << "\n";
  }

  return EXIT_SUCCESS;
}
