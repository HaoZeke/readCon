#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
// clang-format off
#include <algorithm>
#include <string>
#include <vector>
// clang-format on
#include "../Helpers.hpp"

namespace yodecon::helpers::string {
// Parses values from a string.
template <typename T>
std::vector<T> get_val_from_string(const std::string &a_line,
                                   std::optional<size_t> nelements) {
  if (a_line.empty()) {
    throw std::invalid_argument("Line must not be empty.");
  }

  std::vector<T> retval;
  const bool b_isunsigned{std::is_unsigned<T>::value};
  auto elements{get_split_strings(a_line)};

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

template <typename T, size_t N>
std::array<T, N> get_array_from_string(const std::string &a_line) {
  if (a_line.empty()) {
    throw std::invalid_argument("Line must not be empty.");
  }

  std::array<T, N> retval;
  const bool b_isunsigned{std::is_unsigned<T>::value};
  auto elements{get_split_strings(a_line)};

  size_t idx{0};
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

      retval[idx] = static_cast<T>(tmp);
      idx++;
    } else {
      T tmp;
      ss >> tmp;
      retval[idx] = tmp;
      idx++;
    }
  }

  return retval;
}
template <typename Container>
std::string to_csv_string(const Container &container) {
  std::ostringstream ss;
  std::copy(container.begin(), container.end(),
            std::ostream_iterator<typename Container::value_type>(ss, ","));
  std::string result = ss.str();
  return result.empty()
             ? result
             : result.substr(0, result.length() -
                                    1); // get rid of the trailing comma
}

} // namespace yodecon::helpers::string
