#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "readcon_conf.h"

#ifdef WITH_RANGE_V3
#include <range/v3/all.hpp>
#endif

#include "readCon/include/BaseTypes.hpp"
#include "readCon/include/FormatConstants.hpp"
#include "readCon/include/helpers/StringHelpers.hpp"

namespace yodecon {

/**
 * @brief Processes a range of strings representing header lines and assigns the
 * parsed values into a ConFrameLike structure.
 *
 * This template function takes a range of strings, expected to be header lines
 * from a .con file, and populates a given ConFrameLike structure with the
 * parsed data. The header typically includes pre-box headers, box dimensions,
 * angles, post-box headers, atom types, numbers of each atom type, and their
 * masses.
 *
 * @tparam Range The type of the container representing a range of strings. Must
 * support begin() and end() methods.
 * @tparam ConFrameLike The type of the structure to be populated. Must have
 * members corresponding to the expected headers such as prebox_header, boxl,
 * angles, etc.
 * @param a_header The range of strings each representing a line in the header
 * of a .con file.
 * @param conframe The ConFrameLike structure to populate with the parsed header
 * data.
 *
 * @exception std::invalid_argument Thrown if the number of header lines does
 * not match the expected count (9 lines), or if any of the line contents do not
 * conform to the expected formats for box dimensions, angles, atom types, or
 * masses.
 *
 * @details The function checks that the provided range contains exactly the
 * number of lines expected for a standard .con file header (9 lines). It then
 * parses these lines to initialize the properties of the `conframe` parameter.
 *          The function is versatile and can adapt to different container types
 * for the input range and different structures that conform to the expected
 * layout of a ConFrame-like data structure.
 *
 *          The parsing includes:
 *          - Pre-box header strings
 *          - Box dimensions (converted from string to three doubles)
 *          - Angles between the box sides (converted from string to three
 * doubles)
 *          - Post-box header strings
 *          - The number of atom types (converted from string to size_t)
 *          - The number of atoms per type (converted from string to a vector of
 * size_t)
 *          - Masses per atom type (converted from string to a vector of double)
 *
 * Example usage:
 * @code
 * std::vector<std::string> headerLines = {
 *     "Pre-header line 1", "Pre-header line 2",
 *     "10.0 20.0 30.0", "90.0 90.0 90.0",
 *     "Post-header line 1", "Post-header line 2",
 *     "3", "2 2 1", "16.0 12.0 1.0"
 * };
 * ConFrameLike conFrame;
 * process_header(headerLines, conFrame);
 * @endcode
 */
template <typename Range, typename ConFrameLike>
void process_header(const Range &a_header, ConFrameLike &conframe) {
  // TODO(rg): Move into a class later
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

// TODO(rg): Move into the ConFrame class later
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

// TODO(rg): Maybe move to ConFrame, or a helpers section
std::vector<int>
symbols_to_atomic_numbers(const std::vector<std::string> &a_symbols);
std::vector<std::string>
atomic_numbers_to_symbols(const std::vector<int> &a_atomic_numbers);
} // namespace yodecon
