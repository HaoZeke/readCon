/*
 * TestConFrameHelpers.cc
 *
 *  Created on: 24 July 2024
 *      Author: Rohit Goswami
 *     Company: University of Iceland
 */

#include "../include/ReadCon.hpp"
#include <gtest/gtest.h>

TEST(SymbolToAtomicNumberTest, ValidSymbols) {
  std::vector<std::string> symbols = {"H", "O", "N", "Fe", "Au"};
  std::vector<int> expected = {1, 8, 7, 26, 79};

  EXPECT_EQ(yodecon::symbols_to_atomic_numbers(symbols), expected);
}

TEST(SymbolToAtomicNumberTest, InvalidSymbol) {
  std::vector<std::string> symbols = {"H", "O", "N", "Fe", "InvalidSymbol"};

  EXPECT_THROW(yodecon::symbols_to_atomic_numbers(symbols),
               std::invalid_argument);
}

TEST(SymbolToAtomicNumberTest, EmptyInput) {
  std::vector<std::string> symbols;

  EXPECT_TRUE(yodecon::symbols_to_atomic_numbers(symbols).empty());
}

TEST(SymbolToAtomicNumberTest, SingleSymbol) {
  std::vector<std::string> symbols = {"O"};
  std::vector<int> expected = {8};

  EXPECT_EQ(yodecon::symbols_to_atomic_numbers(symbols), expected);
}

TEST(AtomicNumberToSymbolTest, TestValidAtomicNumbers) {
  std::vector<int> atomic_numbers = {1, 6, 8, 15};
  std::vector<std::string> expected_symbols = {"H", "C", "O", "P"};
  EXPECT_EQ(yodecon::atomic_numbers_to_symbols(atomic_numbers),
            expected_symbols);
}

TEST(AtomicNumberToSymbolTest, TestInvalidAtomicNumber) {
  std::vector<int> atomic_numbers = {1,
                                     300}; // 300 is not a valid atomic number
  EXPECT_THROW(yodecon::atomic_numbers_to_symbols(atomic_numbers),
               std::invalid_argument);
}

TEST(AtomicNumberToSymbolTest, TestEmptyInput) {
  std::vector<int> atomic_numbers = {};
  std::vector<std::string> expected_symbols = {};
  EXPECT_EQ(yodecon::atomic_numbers_to_symbols(atomic_numbers),
            expected_symbols);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
