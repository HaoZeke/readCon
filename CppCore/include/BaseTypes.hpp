#pragma once

#include <array>
#include <string>
#include <vector>

namespace yodecon::types {

struct AtomDatum {
  char m_symbol;
  double m_mass, m_x, m_y, m_z;
  bool m_is_fixed;
  size_t m_atom_id;
};

struct ConFrame {
  std::array<std::string, 2> m_prebox_header;
  std::array<double, 3> m_boxl;
  std::array<double, 3> m_angles;
  std::array<std::string, 2> m_postbox_header;
  size_t m_natm_types;
  std::vector<size_t> m_natms_per_type;
  std::vector<size_t> m_masses_per_type;
  std::vector<AtomDatum> m_atom_data;
};

} // namespace yodecon::types
