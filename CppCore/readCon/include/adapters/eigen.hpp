// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#pragma once
#ifdef WITH_EIGEN
#ifdef EIGEN_DEFAULT_TO_ROW_MAJOR
#error "readCon doesn't support row major Eigen yet"
#endif
// Include required headers
#include <Eigen/Dense>
#include <array>
#include <vector>

namespace yodecon::types::adapt {
namespace eigen {
inline Eigen::MatrixXd
extract_positions(const yodecon::types::ConFrameVec &frame) {
  size_t n_atoms = frame.x.size();
  Eigen::MatrixXd positions(n_atoms, 3);
  for (size_t i = 0; i < n_atoms; ++i) {
    positions(i, 0) = frame.x[i];
    positions(i, 1) = frame.y[i];
    positions(i, 2) = frame.z[i];
  }
  return positions;
}
} // namespace eigen
} // namespace yodecon::types::adapt

#endif
