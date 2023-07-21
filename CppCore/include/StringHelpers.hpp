#pragma once
#include "Helpers.hpp"

namespace yodecon::helpers::string {
// Parses values from a string.
template <typename T>
std::vector<T> get_val_from_string(const std::string &line,
                                   std::optional<size_t> nelements) {
  if (line.empty()) {
    throw std::invalid_argument("Line must not be empty.");
  }

  std::vector<T> retval;
  const bool b_isunsigned{std::is_unsigned<T>::value};
  auto elements{get_split_strings(line)};

  if (nelements.has_value()) {
    if (nelements.value() <= 0) {
      throw std::invalid_argument("Number of elements must be positive.");
    }
    elements.resize(nelements.value());
  }

  for (const auto &elem : elements) {
    if (!isNumber(elem)) {
      continue;
    }

    std::istringstream ss{elem};

    if (b_isunsigned) {
      long double tmp;
      ss >> tmp;

      if (tmp < 0) {
        throw std::invalid_argument(
            "Can't represent negative numbers with an unsigned type.");
      }

      retval.push_back(static_cast<T>(tmp));
    } else {
      T tmp;
      ss >> tmp;
      retval.push_back(tmp);
    }
  }

  return retval;
}
} // namespace yodecon::helpers::string
