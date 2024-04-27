// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#pragma once
#ifdef WITH_XTENSOR
// Include required headers
#include <array>
#include <vector>
#include <xtensor/xtensor.hpp>

namespace yodecon::types::adapt {
namespace xts {
inline xt::xtensor<double, 2>
extract_positions(const yodecon::types::ConFrameVec &frame) {
  size_t n_atoms = frame.x.size();
  std::array<size_t, 2> shape = {static_cast<size_t>(n_atoms), 3};

  xt::xtensor<double, 2> positions = xt::empty<double>(shape);
  for (size_t i = 0; i < n_atoms; ++i) {
    positions(i, 0) = frame.x[i];
    positions(i, 1) = frame.y[i];
    positions(i, 2) = frame.z[i];
  }
  return positions;
}
} // namespace xts
} // namespace yodecon::types::adapt

#endif
