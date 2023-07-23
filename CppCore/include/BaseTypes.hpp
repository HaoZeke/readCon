#pragma once

#include <array>
#include <string>
#include <vector>

namespace yodecon::types {

struct AtomDatum {
  std::string symbol;
  // mass is redundant and in the conframe metadata
  double x, y, z;
  bool is_fixed;
  size_t atom_id;
  AtomDatum() : symbol{"NaN"}, x{0}, y{0}, z{0}, is_fixed{false}, atom_id{0} {}
  AtomDatum(std::string a_symbol, double a_x, double a_y, double a_z,
            bool a_is_fixed, size_t a_atom_id)
      : symbol{a_symbol}, x{a_x}, y{a_y}, z{a_z}, is_fixed{a_is_fixed},
        atom_id{a_atom_id} {}
};

// TODO: Give this an constructor taking a file
// Delete the default constructor
struct ConFrame {
  std::array<std::string, 2> prebox_header;
  std::array<double, 3> boxl;
  std::array<double, 3> angles;
  std::array<std::string, 2> postbox_header;
  size_t natm_types;
  std::vector<size_t> natms_per_type;
  std::vector<double> masses_per_type;
  std::vector<AtomDatum> atom_data;
};

} // namespace yodecon::types
