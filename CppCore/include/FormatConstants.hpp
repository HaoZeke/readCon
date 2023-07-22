#pragma once
#include <cstddef>
namespace yodecon::constants {
// First 9 lines form the header
constexpr size_t HeaderLength{9};
// Each coord block has 2 lines, which need to be handled within the coordinate
// parser Cu Coordinates of Component 1
constexpr size_t CoordHeader{2};
} // namespace yodecon::constants
