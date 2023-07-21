#pragma once

#include <array>
#include <string>
#include <vector>

namespace yodecon::types {

struct AtomDatum {
  char symbol;
  double mass, x, y, z;
  bool is_fixed;
  size_t atom_id;
};

struct ConFrame {
  std::array<std::string, 2> prebox_header;
  std::array<double, 3> boxl;
  std::array<double, 3> angles;
  std::array<std::string, 2> postbox_header;
  size_t natm_types;
  std::vector<size_t> natms_per_type;
  std::vector<size_t> masses_per_type;
  std::vector<AtomDatum> atom_data;
};

} // namespace yodecon::types
