#include "include/ReadCon.hpp"

void process_header(const std::vector<std::string> &a_header,
                    yodecon::types::ConFrame conframe) {
  // Headers are always the first 9 lines via take
  conframe.prebox_header[0] = a_header[0];
  conframe.prebox_header[1] = a_header[1];
  conframe.boxl =
      yodecon::helpers::string::get_array_from_string<double, 3>(a_header[2]);
  conframe.angles =
      yodecon::helpers::string::get_array_from_string<double, 3>(a_header[3]);
  conframe.postbox_header[0] = a_header[4];
  conframe.postbox_header[1] = a_header[5];
  conframe.natm_types =
      (yodecon::helpers::string::get_array_from_string<size_t, 1>(
          a_header[6]))[0];
  conframe.natms_per_type =
      yodecon::helpers::string::get_val_from_string<size_t>(
          a_header[7], conframe.natm_types);
  conframe.masses_per_type =
      yodecon::helpers::string::get_val_from_string<double>(
          a_header[8], conframe.natm_types);
}
