#include "include/ReadCon.hpp"
#include "include/BaseTypes.hpp"
#include "include/FormatConstants.hpp"

namespace yodecon {
void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrame &conframe) {
  types::AtomDatum tmp_atm;
  size_t drop_amount = constants::HeaderLength;
  for (size_t idx{0}; idx < conframe.natm_types; idx++) {
    size_t natms = conframe.natms_per_type[idx];
    size_t take_amount = natms + constants::CoordHeader;
    auto coords_view = a_filecontents | std::views::drop(drop_amount) |
                       std::views::take(take_amount);
    for (auto &&line : coords_view | std::views::drop(constants::CoordHeader)) {
      tmp_atm.symbol = coords_view[0];
      tmp_atm.mass = conframe.masses_per_type[idx];
      std::tie(tmp_atm.x, tmp_atm.y, tmp_atm.z, tmp_atm.is_fixed,
               tmp_atm.atom_id) =
          helpers::string::get_array_from_string<double, 5>(line);
      conframe.atom_data.push_back(tmp_atm);
    }
    drop_amount += take_amount;
  }
}

} // namespace yodecon
