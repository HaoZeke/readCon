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
  std::vector<size_t> expected = {1, 8, 7, 26, 79};

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
  std::vector<size_t> expected = {8};

  EXPECT_EQ(yodecon::symbols_to_atomic_numbers(symbols), expected);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
