// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
/*
 * TestConFrameHelpers.cc
 *
 *  Created on: 24 July 2024
 *      Author: Rohit Goswami
 *     Company: University of Iceland
 */

#include "readCon/include/ReadCon.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("SymbolToAtomicNumberTest - ValidSymbols", "[SymbolToAtomicNumber]") {
  std::vector<std::string> symbols = {"H", "O", "N", "Fe", "Au"};
  std::vector<int> expected = {1, 8, 7, 26, 79};

  REQUIRE(yodecon::symbols_to_atomic_numbers(symbols) == expected);
}

TEST_CASE("SymbolToAtomicNumberTest - InvalidSymbol",
          "[SymbolToAtomicNumber]") {
  std::vector<std::string> symbols = {"H", "O", "N", "Fe", "InvalidSymbol"};

  REQUIRE_THROWS_AS(yodecon::symbols_to_atomic_numbers(symbols),
                    std::invalid_argument);
}

TEST_CASE("SymbolToAtomicNumberTest - EmptyInput", "[SymbolToAtomicNumber]") {
  std::vector<std::string> symbols;

  REQUIRE(yodecon::symbols_to_atomic_numbers(symbols).empty());
}

TEST_CASE("SymbolToAtomicNumberTest - SingleSymbol", "[SymbolToAtomicNumber]") {
  std::vector<std::string> symbols = {"O"};
  std::vector<int> expected = {8};

  REQUIRE(yodecon::symbols_to_atomic_numbers(symbols) == expected);
}

TEST_CASE("AtomicNumberToSymbolTest - ValidAtomicNumbers",
          "[AtomicNumberToSymbol]") {
  std::vector<int> atomic_numbers = {1, 6, 8, 15};
  std::vector<std::string> expected_symbols = {"H", "C", "O", "P"};

  REQUIRE(yodecon::atomic_numbers_to_symbols(atomic_numbers) ==
          expected_symbols);
}

TEST_CASE("AtomicNumberToSymbolTest - InvalidAtomicNumber",
          "[AtomicNumberToSymbol]") {
  std::vector<int> atomic_numbers = {1,
                                     300}; // 300 is not a valid atomic number

  REQUIRE_THROWS_AS(yodecon::atomic_numbers_to_symbols(atomic_numbers),
                    std::invalid_argument);
}

TEST_CASE("AtomicNumberToSymbolTest - EmptyInput", "[AtomicNumberToSymbol]") {
  std::vector<int> atomic_numbers = {};
  std::vector<std::string> expected_symbols = {};

  REQUIRE(yodecon::atomic_numbers_to_symbols(atomic_numbers) ==
          expected_symbols);
}
