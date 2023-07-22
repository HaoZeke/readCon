#pragma once

#include "include/BaseTypes.hpp"
#include "include/helpers/StringHelpers.hpp"
namespace yodecon {

template <typename Range>
void process_header(const Range &a_header, yodecon::types::ConFrame &conframe) {
  // Headers are always the first 9 lines via take
  std::vector<std::string> header_vec(a_header.begin(), a_header.end());
  conframe.prebox_header[0] = header_vec[0];
  conframe.prebox_header[1] = header_vec[1];
  conframe.boxl =
      yodecon::helpers::string::get_array_from_string<double, 3>(header_vec[2]);
  conframe.angles =
      yodecon::helpers::string::get_array_from_string<double, 3>(header_vec[3]);
  conframe.postbox_header[0] = header_vec[4];
  conframe.postbox_header[1] = header_vec[5];
  conframe.natm_types =
      (yodecon::helpers::string::get_array_from_string<size_t, 1>(
          header_vec[6]))[0];
  conframe.natms_per_type =
      yodecon::helpers::string::get_val_from_string<size_t>(
          header_vec[7], conframe.natm_types);
  conframe.masses_per_type =
      yodecon::helpers::string::get_val_from_string<double>(
          header_vec[8], conframe.natm_types);
}

} // namespace yodecon
