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

types::ConFrame create_single_con(const std::vector<std::string> &a_fconts) {
  yodecon::types::ConFrame result;
  yodecon::process_header(
      (a_fconts | ranges::views::take(yodecon::constants::HeaderLength)),
      result);
  size_t natmlines = std::accumulate(result.natms_per_type.begin(),
                                     result.natms_per_type.end(), 0);
  std::cout << "We have " << natmlines << " atom lines.\n";
  std::cout << "We have " << result.natm_types << " types. So we have "
            << result.natm_types * 2
            << " non-coordinate lines.\nThe last line of this frame is:";
  size_t nframelines =
      natmlines + yodecon::constants::HeaderLength + (result.natm_types * 2);
  std::cout << a_fconts[nframelines - 1] << "\n"; // -1 for the indexing from 0
  // NOTE: This is inefficient, we can probably do better
  std::vector<std::string> a_frame(a_fconts.begin(),
                                   a_fconts.begin() + nframelines);
  process_coordinates(a_frame, result);
  return result;
}

std::vector<yodecon::types::ConFrame>
create_multi_con(std::vector<std::string> a_fconts) {
  std::vector<yodecon::types::ConFrame> result;
  while (!a_fconts.empty()) {
    // Create a single con frame
    yodecon::types::ConFrame single_frame = create_single_con(a_fconts);
    // Add this frame to the results
    result.push_back(single_frame);
    // Calculate the number of lines this frame contained
    size_t natmlines = std::accumulate(single_frame.natms_per_type.begin(),
                                       single_frame.natms_per_type.end(), 0);
    size_t nframelines = natmlines + yodecon::constants::HeaderLength +
                         (single_frame.natm_types * 2);
    // Ensure we don't try to erase more lines than exist in a_fconts
    nframelines = std::min(nframelines, a_fconts.size());
    // Erase the lines of the frame we just processed from a_fconts
    a_fconts.erase(a_fconts.begin(), a_fconts.begin() + nframelines);
  }
  return result;
}

} // namespace yodecon
