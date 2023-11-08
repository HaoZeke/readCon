#pragma once

#include <stdexcept>

#ifdef WITH_RANGE_V3
#include <range/v3/all.hpp>
#endif

#include "BaseTypes.hpp"
#include "FormatConstants.hpp"
#include "helpers/StringHelpers.hpp"

namespace yodecon {

// TODO: Move into a class later
template <typename Range, typename ConFrameLike>
void process_header(const Range &a_header, ConFrameLike &conframe) {
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

void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrameVec &conframe);

#ifdef WITH_RANGE_V3
//! This function extracts con file information from a vector of strings
template <typename ConFrameLike>
ConFrameLike create_single_con(const std::vector<std::string> &a_fconts) {
  ConFrameLike result;
  yodecon::process_header(
      (a_fconts | ranges::views::take(yodecon::constants::HeaderLength)),
      result);
  size_t natmlines = std::accumulate(result.natms_per_type.begin(),
                                     result.natms_per_type.end(), 0);
  // std::cout << "We have " << natmlines << " atom lines.\n";
  // std::cout << "We have " << result.natm_types << " types. So we have "
  //           << result.natm_types * 2
  //           << " non-coordinate lines.\nThe last line of this frame is:";
  size_t nframelines =
      natmlines + yodecon::constants::HeaderLength + (result.natm_types * 2);
  // std::cout << a_fconts[nframelines - 1] << "\n"; // -1 for the indexing from
  // 0 NOTE: This is inefficient, we can probably do better
  std::vector<std::string> a_frame(a_fconts.begin(),
                                   a_fconts.begin() + nframelines);
  process_coordinates(a_frame, result);
  return result;
}
#else
//! This function extracts con file information from a vector of strings
template <typename ConFrameLike>
ConFrameLike create_single_con(const std::vector<std::string> &a_fconts) {
  ConFrameLike result;
  auto header_view = norange::take(a_fconts, yodecon::constants::HeaderLength);
  yodecon::process_header(header_view, result);
  size_t natmlines = std::accumulate(result.natms_per_type.begin(),
                                     result.natms_per_type.end(), 0);
  size_t nframelines = natmlines + yodecon::constants::HeaderLength +
                       (result.natm_types * yodecon::constants::CoordHeader);
  // Use take to get the subset of the frame content we're interested in
  // NOTE: This is inefficient, we can probably do better
  auto a_frame = norange::take(a_fconts, nframelines);
  process_coordinates(a_frame, result);
  return result;
}
#endif

//! This function extracts a list of con data from a vector of strings
template <typename ConFrameLike>
std::vector<ConFrameLike> create_multi_con(std::vector<std::string> a_fconts) {
  std::vector<ConFrameLike> result;
  while (!a_fconts.empty()) {
    // Create a single con frame
    ConFrameLike single_frame = create_single_con<ConFrameLike>(a_fconts);
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

// TODO: Maybe move to ConFrame, or a helpers section
std::vector<int>
symbols_to_atomic_numbers(const std::vector<std::string> &a_symbols);
std::vector<std::string>
atomic_numbers_to_symbols(const std::vector<int> &a_atomic_numbers);
} // namespace yodecon
