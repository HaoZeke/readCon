#pragma once

#include "include/BaseTypes.hpp"
#include "include/FormatConstants.hpp"
#include "include/helpers/StringHelpers.hpp"
#include <ranges>
#include <stdexcept>
namespace yodecon {

// TODO: Move into the ConFrame class later
template <typename Range>
void process_header(const Range &a_header, yodecon::types::ConFrame &conframe) {
  std::vector<std::string> header_vec(a_header.begin(), a_header.end());
  if (header_vec.size() != yodecon::constants::HeaderLength) {
    throw std::invalid_argument("Headers are always 9 lines for a con file");
  }
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

// TODO: Move into the ConFrame class later
void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrame &conframe);
} // namespace yodecon
