#include "Helpers.hpp"
#include "include/BaseTypes.hpp"
#include "include/FormatConstants.hpp"
#include "include/ReadCon.hpp"
#include "include/helpers/StringHelpers.hpp"

#ifdef WITH_FMT
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#endif

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

  std::vector<std::string> fconts =
      yodecon::helpers::file::read_con_file(filename);

  yodecon::types::ConFrame tmp;

  yodecon::process_header(
      (fconts | std::views::take(yodecon::constants::HeaderLength)), tmp);

#ifdef WITH_FMT
  fmt::print("prebox_headers: {}\n", tmp.prebox_header);
  fmt::print("box lengths: {}\n", tmp.boxl);
  fmt::print("angles: {}\n", tmp.angles);
  fmt::print("postbox_headers: {}\n", tmp.postbox_header);
  fmt::print("natm_types: {}\n", tmp.natm_types);
  fmt::print("natms_per_type: {}\n", tmp.natms_per_type);
  fmt::print("masses_per_type: {}\n", tmp.masses_per_type);
#endif

  yodecon::process_coordinates(fconts, tmp);

#ifdef WITH_FMT
  fmt::print("{}\n", tmp.atom_data.size());
  for (auto &&tmp_id : tmp.atom_data) {
    fmt::print("{} {} {} {} {} {} {}\n", tmp_id.symbol, tmp_id.mass, tmp_id.x,
               tmp_id.y, tmp_id.z, tmp_id.is_fixed, tmp_id.atom_id);
  }
#endif

  return EXIT_SUCCESS;
}
