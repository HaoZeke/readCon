#include "include/ReadCon.hpp"

namespace yodecon {
void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrame &conframe) {
  types::AtomDatum tmp_atm;
  size_t drop_amount = constants::HeaderLength;
  for (size_t idx{0}; idx < conframe.natm_types; idx++) {
    size_t natms = conframe.natms_per_type[idx];
    size_t take_amount = natms + constants::CoordHeader;
    auto coords_view = a_filecontents | ranges::views::drop(drop_amount) |
                       ranges::views::take(take_amount);
    tmp_atm.symbol = coords_view[0];
    for (auto &&line :
         coords_view | ranges::views::drop(constants::CoordHeader)) {
      auto dbl_line = helpers::string::get_array_from_string<double, 5>(line);
      tmp_atm.x = dbl_line[0];
      tmp_atm.y = dbl_line[1];
      tmp_atm.z = dbl_line[2];
      tmp_atm.is_fixed = static_cast<bool>(dbl_line[3]);
      tmp_atm.atom_id = static_cast<size_t>(dbl_line[4]);
      conframe.atom_data.push_back(tmp_atm);
    }
    drop_amount += take_amount;
  }
}

void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrameVec &conframevec) {
  types::AtomDatum tmp_atm;
  size_t drop_amount = constants::HeaderLength;
  for (size_t idx{0}; idx < conframevec.natm_types; idx++) {
    size_t natms = conframevec.natms_per_type[idx];
    size_t take_amount = natms + constants::CoordHeader;
    auto coords_view = a_filecontents | ranges::views::drop(drop_amount) |
                       ranges::views::take(take_amount);
    for (auto &&line :
         coords_view | ranges::views::drop(constants::CoordHeader)) {
      conframevec.symbol.push_back(coords_view[0]);
      auto dbl_line = helpers::string::get_array_from_string<double, 5>(line);
      conframevec.x.push_back(dbl_line[0]);
      conframevec.y.push_back(dbl_line[1]);
      conframevec.z.push_back(dbl_line[2]);
      conframevec.is_fixed.push_back(static_cast<bool>(dbl_line[3]));
      conframevec.atom_id.push_back(static_cast<size_t>(dbl_line[4]));
    }
    drop_amount += take_amount;
  }
}

std::vector<size_t>
symbols_to_atomic_numbers(const std::vector<std::string> &a_symbols) {
  return yodecon::helpers::con::convert_keys_to_values<std::string, size_t>(
      a_symbols, yodecon::types::known_info::AtomicNumbers,
      "Invalid element symbol");
}

std::vector<std::string>
atomic_numbers_to_symbols(const std::vector<size_t> &a_atomic_numbers) {
  return yodecon::helpers::con::convert_keys_to_values<size_t, std::string>(
      a_atomic_numbers, yodecon::types::known_info::AtomicSymbols,
      "Invalid atomic number");
}

} // namespace yodecon
