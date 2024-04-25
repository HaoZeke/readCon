#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#include <cstddef>
namespace yodecon::constants {
constexpr size_t HeaderLength{9}; ///< First 9 lines form the header
// parser Cu Coordinates of Component 1
constexpr size_t CoordHeader{2}; ///< Each coord block has 2 lines, which need
                                 ///< to be handled within the coordinate
} // namespace yodecon::constants
