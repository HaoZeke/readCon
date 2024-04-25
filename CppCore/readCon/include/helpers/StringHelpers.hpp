#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
// clang-format off
#include <algorithm>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>
// clang-format on
#include "readCon/include/Helpers.hpp"

namespace yodecon::helpers::string {
/**
 * @brief Parses a string and converts it into a vector of values of type T.
 *
 * This template function reads a string containing whitespace-separated tokens
 * and attempts to convert each token into type T, accumulating the results into
 * a vector. This is useful for extracting typed data from strings, such as
 * reading numerical data from a line of text.
 *
 * @tparam T The type into which the string tokens will be converted. T must be
 * compatible with standard input stream operations.
 * @param a_line The string to parse.
 * @param nelements An optional parameter specifying the number of elements to
 * extract. If provided, the function will resize the resulting vector to this
 * size, which affects how many elements are converted.
 * @return std::vector<T> A vector of elements of type T extracted and converted
 * from the string.
 *
 * @exception std::invalid_argument Thrown if the input string is empty, if
 * `nelements` is zero or negative, if the string contains elements that cannot
 * be converted to type T, or if trying to convert a negative number into an
 * unsigned type T.
 *
 * @details The function splits the string into tokens using whitespace as
 * delimiters. Each token is checked to determine if it represents a valid
 * number. For unsigned types, it additionally validates that the number is not
 * negative. The function is robust against input errors, enforcing strong type
 * checks and constraints as defined by the template parameter.
 *
 * Example usage:
 * @code
 * std::string data = "10 20 30 40 50";
 * auto numbers = get_val_from_string<int>(data, std::nullopt);
 * for (int num : numbers) {
 *     std::cout << num << std::endl;  // Output: 10, 20, 30, 40, 50
 * }
 * @endcode
 */
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
      long double tmp{0};
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
/**
 * @brief Parses a string and converts it into a fixed-size array of values of
 * type T.
 *
 * This template function reads a string containing whitespace-separated tokens
 * and attempts to convert each token into type T, storing the results into a
 * std::array of a specified size. The function is useful for extracting typed
 * data from a single line of text where the expected number of elements is
 * known and fixed.
 *
 * @tparam T The type into which the string tokens will be converted. T must
 * support standard input stream operations.
 * @tparam N The size of the array to return. This template parameter specifies
 * the expected number of elements that the input string should contain.
 * @param a_line The string to parse.
 * @return std::array<T, N> An array of elements of type T extracted and
 * converted from the string.
 *
 * @exception std::invalid_argument Thrown if the input string is empty, if a
 * token in the string cannot be converted to type T, or if trying to convert a
 * negative number into an unsigned type T when T is an unsigned type.
 *
 * @details The function splits the string into tokens using whitespace as
 * delimiters. Each token is checked to determine if it represents a valid
 * number. The function parses up to N tokens into the resulting array. For
 * unsigned types, it additionally checks that the numbers are not negative. If
 * there are fewer tokens than N, the remaining elements of the array retain
 *          their default-initialized values. If more, they are ignored,
 * ensuring the array size matches N.
 *
 * Example usage:
 * @code
 * std::string data = "10 20 30";
 * auto numbers = get_array_from_string<int, 3>(data);
 * std::cout << numbers[0] << ", " << numbers[1] << ", " << numbers[2]; //
 * Output: 10, 20, 30
 * @endcode
 */
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
      long double tmp{0};
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
/**
 * @brief Converts the contents of a container to a comma-separated values (CSV)
 * string.
 *
 * This template function takes a container and converts each element into a
 * string, concatenating them into a single CSV string. The elements are
 * separated by commas. It is useful for quickly serializing container data to a
 * format that is easily readable and usable in CSV files or for display
 * purposes.
 *
 * @tparam Container The type of the container which must support iteration and
 * whose value_type must be compatible with output stream operations.
 * @param container The container whose elements are to be converted into a CSV
 * string.
 * @return std::string A string containing the elements of the container,
 * separated by commas. If the container is empty, returns an empty string.
 *
 * @details The function creates an output string stream and uses std::copy with
 * an std::ostream_iterator to insert each element into the stream followed by a
 * comma. After copying all elements, it checks if the resulting string is not
 * empty and removes the trailing comma. This is crucial to ensure the output
 * does not end with an extra comma, making it suitable for use in CSV files or
 * other contexts where such formatting is important.
 *
 * Example usage:
 * @code
 * std::vector<int> numbers = {1, 2, 3, 4};
 * std::string csv = to_csv_string(numbers);
 * std::cout << csv; // Output: "1,2,3,4"
 * @endcode
 */
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
