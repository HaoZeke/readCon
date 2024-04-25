/*
 * TestConFrame.cc
 *
 *  Created on: 23 July 2024
 *      Author: Rohit Goswami
 *     Company: University of Iceland
 */

#include "../include/ReadCon.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace yodecon::types;
constexpr double fp_tol{1e-12};

TEST_CASE("AtomDatum Constructor") {
  AtomDatum atom("O", 1.0, 2.0, 3.0, false, 1);
  REQUIRE(atom.symbol == "O");
  REQUIRE_THAT(atom.x, Catch::Matchers::WithinAbs(1.0, fp_tol));
  REQUIRE_THAT(atom.y, Catch::Matchers::WithinAbs(2.0, fp_tol));
  REQUIRE_THAT(atom.z, Catch::Matchers::WithinAbs(3.0, fp_tol));
  REQUIRE_FALSE(atom.is_fixed);
  REQUIRE(atom.atom_id == 1);
}

TEST_CASE("ConFrame Constructor") {
  ConFrame conFrame;
  conFrame.prebox_header = {"Generated by eOn", ""};
  conFrame.boxl = {25.0, 25.0, 25.0};
  conFrame.angles = {90.0, 90.0, 90.0};
  conFrame.postbox_header = {"", ""};
  conFrame.natm_types = 4;
  conFrame.natms_per_type = {2, 4, 6, 1};
  conFrame.masses_per_type = {15.99, 12.011, 1.008, 32.065};
  AtomDatum atom("O", 10.477713, 12.379463, 12.871778, false, 1);
  conFrame.atom_data.push_back(atom);

  REQUIRE(conFrame.prebox_header[0] == "Generated by eOn");
  REQUIRE_THAT(conFrame.boxl[0], Catch::Matchers::WithinAbs(25.0, fp_tol));
  REQUIRE_THAT(conFrame.angles[0], Catch::Matchers::WithinAbs(90.0, fp_tol));
  REQUIRE(conFrame.natm_types == 4);
  REQUIRE(conFrame.natms_per_type[0] == 2);
  REQUIRE_THAT(conFrame.masses_per_type[0],
               Catch::Matchers::WithinAbs(15.99, fp_tol));
  REQUIRE(conFrame.atom_data[0].symbol == "O");
}

TEST_CASE("ConFrame AtomicSymbolsRoundTrip") {
  // Create a ConFrame with multiple AtomDatum objects.
  ConFrame conFrame;
  conFrame.atom_data.push_back(AtomDatum("H", 1.0, 2.0, 3.0, false, 1));
  conFrame.atom_data.push_back(AtomDatum("C", 4.0, 5.0, 6.0, false, 2));
  conFrame.atom_data.push_back(AtomDatum("O", 7.0, 8.0, 9.0, false, 3));

  // Extract atomic symbols from ConFrame.
  std::vector<std::string> atomicSymbols;
  for (const auto &atom : conFrame.atom_data) {
    atomicSymbols.push_back(atom.symbol);
  }

  // Convert atomic symbols to numbers.
  std::vector<int> atomicNumbers =
      yodecon::symbols_to_atomic_numbers(atomicSymbols);

  // Check that the conversion was correct.
  REQUIRE(atomicNumbers.size() == 3);
  REQUIRE(atomicNumbers[0] == 1); // H
  REQUIRE(atomicNumbers[1] == 6); // C
  REQUIRE(atomicNumbers[2] == 8); // O

  // Convert atomic numbers back to symbols.
  std::vector<std::string> roundTripSymbols =
      yodecon::atomic_numbers_to_symbols(atomicNumbers);

  // Check that the conversion back to symbols was correct.
  REQUIRE(roundTripSymbols.size() == 3);
  REQUIRE(roundTripSymbols[0] == "H");
  REQUIRE(roundTripSymbols[1] == "C");
  REQUIRE(roundTripSymbols[2] == "O");
}

TEST_CASE("ConFrameTest CreateSingleCon_SingleFile") {
  // The actual lines of the single con file.
  std::vector<std::string> a_fconts = {
      "Random Number Seed",
      "Time",
      "15.345600\t21.702000\t100.000000",
      "90.000000\t90.000000\t90.000000",
      "0 0",
      "218 0 1",
      "2",
      "2 2",
      "63.546000 1.007930",
      "Cu",
      "Coordinates of Component 1",
      "0.63940000000000108    0.90450000000000019    6.97529999999999539 1    "
      "0",
      "3.19699999999999873    0.90450000000000019    6.97529999999999539 1    "
      "1",
      "H",
      "Coordinates of Component 2",
      "8.68229999999999968    9.94699999999999740   11.73299999999999343 0  2",
      "7.94209999999999550    9.94699999999999740   11.73299999999999343 0  3"};

  // Call the function under test.
  auto result = yodecon::create_single_con<ConFrame>(a_fconts);
  // Check the resulting ConFrame object against expected values.

  REQUIRE(result.prebox_header[0] == "Random Number Seed");
  REQUIRE(result.prebox_header[1] == "Time");

  REQUIRE_THAT(result.boxl[0], Catch::Matchers::WithinAbs(15.345600, fp_tol));
  REQUIRE_THAT(result.boxl[1], Catch::Matchers::WithinAbs(21.702000, fp_tol));
  REQUIRE_THAT(result.boxl[2], Catch::Matchers::WithinAbs(100.000000, fp_tol));

  REQUIRE_THAT(result.angles[0], Catch::Matchers::WithinAbs(90.000000, fp_tol));
  REQUIRE_THAT(result.angles[1], Catch::Matchers::WithinAbs(90.000000, fp_tol));
  REQUIRE_THAT(result.angles[2], Catch::Matchers::WithinAbs(90.000000, fp_tol));

  REQUIRE(result.natm_types == 2);
  REQUIRE(result.natms_per_type[0] == 2);
  REQUIRE(result.natms_per_type[1] == 2);

  REQUIRE_THAT(result.masses_per_type[0],
               Catch::Matchers::WithinAbs(63.546000, fp_tol));
  REQUIRE_THAT(result.masses_per_type[1],
               Catch::Matchers::WithinAbs(1.007930, fp_tol));

  REQUIRE(result.atom_data.size() == 4);

  REQUIRE(result.atom_data[0].symbol == "Cu");
  REQUIRE_THAT(result.atom_data[0].x,
               Catch::Matchers::WithinAbs(0.63940000000000108, fp_tol));
  REQUIRE_THAT(result.atom_data[0].y,
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.atom_data[0].z,
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.atom_data[0].is_fixed);
  REQUIRE(result.atom_data[0].atom_id == 0);

  REQUIRE(result.atom_data[1].symbol == "Cu");
  REQUIRE_THAT(result.atom_data[1].x,
               Catch::Matchers::WithinAbs(3.19699999999999873, fp_tol));
  REQUIRE_THAT(result.atom_data[1].y,
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.atom_data[1].z,
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.atom_data[1].is_fixed);
  REQUIRE(result.atom_data[1].atom_id == 1);

  REQUIRE(result.atom_data[2].symbol == "H");
  REQUIRE_THAT(result.atom_data[2].x,
               Catch::Matchers::WithinAbs(8.68229999999999968, fp_tol));
  REQUIRE_THAT(result.atom_data[2].y,
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.atom_data[2].z,
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE_FALSE(result.atom_data[2].is_fixed);
  REQUIRE(result.atom_data[2].atom_id == 2);

  REQUIRE(result.atom_data[3].symbol == "H");
  REQUIRE_THAT(result.atom_data[3].x,
               Catch::Matchers::WithinAbs(7.94209999999999550, fp_tol));
  REQUIRE_THAT(result.atom_data[3].y,
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.atom_data[3].z,
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE_FALSE(result.atom_data[3].is_fixed);
  REQUIRE(result.atom_data[3].atom_id == 3);
}

TEST_CASE("ConFrameTest CreateSingleCon_MultiFile") {
  // The actual lines of the multi con file.
  std::vector<std::string> a_fconts = {
      "Random Number Seed", "Time", "15.345600\t21.702000\t100.000000",
      "90.000000\t90.000000\t90.000000", "0 0", "218 0 1", "2", "2 2",
      "63.546000 1.007930", "Cu", "Coordinates of Component 1",
      "0.63940000000000108    0.90450000000000019    6.97529999999999539 1    "
      "0",
      "3.19699999999999873    0.90450000000000019    6.97529999999999539 1    "
      "1",
      "H", "Coordinates of Component 2",
      "8.68229999999999968    9.94699999999999740   11.73299999999999343 0  2",
      "7.94209999999999550    9.94699999999999740   11.73299999999999343 0  3",
      // Starting of the second con
      "Random Number Seed", "Time", "15.345600\t21.702000\t100.000000",
      "90.000000\t90.000000\t90.000000", "0 0", "218 0 1", "2", "2 2",
      "63.546000 1.007930", "Cu", "Coordinates of Component 1",
      "0.63940000000000108    0.90450000000000019    6.97529999999999539 1    "
      "0",
      "3.19699999999999873    0.90450000000000019    6.97529999999999539 1    "
      "1",
      "H", "Coordinates of Component 2",
      "8.85495714285713653    9.94699999999999740   11.16538571428571380 0  2",
      "7.76944285714285154    9.94699999999999740   11.16538571428571380 0  3"};

  // Call the function under test.
  auto result = yodecon::create_single_con<ConFrame>(a_fconts);
  // Check the resulting ConFrame object against expected values.
  REQUIRE(result.prebox_header[0] == "Random Number Seed");
  REQUIRE(result.prebox_header[1] == "Time");

  REQUIRE_THAT(result.boxl[0], Catch::Matchers::WithinAbs(15.345600, fp_tol));
  REQUIRE_THAT(result.boxl[1], Catch::Matchers::WithinAbs(21.702000, fp_tol));
  REQUIRE_THAT(result.boxl[2], Catch::Matchers::WithinAbs(100.000000, fp_tol));

  REQUIRE_THAT(result.angles[0], Catch::Matchers::WithinAbs(90.000000, fp_tol));
  REQUIRE_THAT(result.angles[1], Catch::Matchers::WithinAbs(90.000000, fp_tol));
  REQUIRE_THAT(result.angles[2], Catch::Matchers::WithinAbs(90.000000, fp_tol));

  REQUIRE(result.natm_types == 2);
  REQUIRE(result.natms_per_type[0] == 2);
  REQUIRE(result.natms_per_type[1] == 2);

  REQUIRE_THAT(result.masses_per_type[0],
               Catch::Matchers::WithinAbs(63.546000, fp_tol));
  REQUIRE_THAT(result.masses_per_type[1],
               Catch::Matchers::WithinAbs(1.007930, fp_tol));

  REQUIRE(result.atom_data.size() == 4);

  REQUIRE(result.atom_data[0].symbol == "Cu");
  REQUIRE_THAT(result.atom_data[0].x,
               Catch::Matchers::WithinAbs(0.63940000000000108, fp_tol));
  REQUIRE_THAT(result.atom_data[0].y,
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.atom_data[0].z,
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.atom_data[0].is_fixed == true);
  REQUIRE(result.atom_data[0].atom_id == 0);

  REQUIRE(result.atom_data[1].symbol == "Cu");
  REQUIRE_THAT(result.atom_data[1].x,
               Catch::Matchers::WithinAbs(3.19699999999999873, fp_tol));
  REQUIRE_THAT(result.atom_data[1].y,
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.atom_data[1].z,
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.atom_data[1].is_fixed == true);
  REQUIRE(result.atom_data[1].atom_id == 1);

  REQUIRE(result.atom_data[2].symbol == "H");
  REQUIRE_THAT(result.atom_data[2].x,
               Catch::Matchers::WithinAbs(8.68229999999999968, fp_tol));
  REQUIRE_THAT(result.atom_data[2].y,
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.atom_data[2].z,
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE(result.atom_data[2].is_fixed == false);
  REQUIRE(result.atom_data[2].atom_id == 2);

  REQUIRE(result.atom_data[3].symbol == "H");
  REQUIRE_THAT(result.atom_data[3].x,
               Catch::Matchers::WithinAbs(7.94209999999999550, fp_tol));
  REQUIRE_THAT(result.atom_data[3].y,
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.atom_data[3].z,
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE(result.atom_data[3].is_fixed == false);
  REQUIRE(result.atom_data[3].atom_id == 3);
}

TEST_CASE("ConFrameTest CreateMultiCon") {
  // The actual lines of the multi con file.
  std::vector<std::string> a_fconts = {
      "Random Number Seed", "Time", "15.345600\t21.702000\t100.000000",
      "90.000000\t90.000000\t90.000000", "0 0", "218 0 1", "2", "2 2",
      "63.546000 1.007930", "Cu", "Coordinates of Component 1",
      "0.63940000000000108    0.90450000000000019    6.97529999999999539 1    "
      "0",
      "3.19699999999999873    0.90450000000000019    6.97529999999999539 1    "
      "1",
      "H", "Coordinates of Component 2",
      "8.68229999999999968    9.94699999999999740   11.73299999999999343 0  2",
      "7.94209999999999550    9.94699999999999740   11.73299999999999343 0  3",
      // Starting of the second con
      "Random Number Seed", "Time", "15.345600\t21.702000\t100.000000",
      "90.000000\t90.000000\t90.000000", "0 0", "218 0 1", "2", "2 2",
      "63.546000 1.007930", "Cu", "Coordinates of Component 1",
      "0.63940000000000108    0.90450000000000019    6.97529999999999539 1    "
      "0",
      "3.19699999999999873    0.90450000000000019    6.97529999999999539 1    "
      "1",
      "H", "Coordinates of Component 2",
      "8.85495714285713653    9.94699999999999740   11.16538571428571380 0  2",
      "7.76944285714285154    9.94699999999999740   11.16538571428571380 0  3"};

  // Call the function under test.
  std::vector<ConFrame> results = yodecon::create_multi_con<ConFrame>(a_fconts);
  // We're expecting 2 frames from the input data
  REQUIRE(results.size() == 2);
  // Check the first ConFrame
  {
    const auto &result = results[0];
    REQUIRE(result.prebox_header[0] == "Random Number Seed");
    REQUIRE(result.prebox_header[1] == "Time");

    REQUIRE_THAT(result.boxl[0], Catch::Matchers::WithinAbs(15.345600, fp_tol));
    REQUIRE_THAT(result.boxl[1], Catch::Matchers::WithinAbs(21.702000, fp_tol));
    REQUIRE_THAT(result.boxl[2],
                 Catch::Matchers::WithinAbs(100.000000, fp_tol));

    REQUIRE_THAT(result.angles[0],
                 Catch::Matchers::WithinAbs(90.000000, fp_tol));
    REQUIRE_THAT(result.angles[1],
                 Catch::Matchers::WithinAbs(90.000000, fp_tol));
    REQUIRE_THAT(result.angles[2],
                 Catch::Matchers::WithinAbs(90.000000, fp_tol));

    REQUIRE(result.natm_types == 2);
    REQUIRE(result.natms_per_type[0] == 2);
    REQUIRE(result.natms_per_type[1] == 2);

    REQUIRE_THAT(result.masses_per_type[0],
                 Catch::Matchers::WithinAbs(63.546000, fp_tol));
    REQUIRE_THAT(result.masses_per_type[1],
                 Catch::Matchers::WithinAbs(1.007930, fp_tol));

    REQUIRE(result.atom_data.size() == 4);

    REQUIRE_THAT(result.atom_data[0].x,
                 Catch::Matchers::WithinAbs(0.63940000000000108, fp_tol));
    REQUIRE_THAT(result.atom_data[0].y,
                 Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
    REQUIRE_THAT(result.atom_data[0].z,
                 Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
    REQUIRE(result.atom_data[0].is_fixed == true);
    REQUIRE(result.atom_data[0].atom_id == 0);

    REQUIRE_THAT(result.atom_data[1].x,
                 Catch::Matchers::WithinAbs(3.19699999999999873, fp_tol));
    REQUIRE_THAT(result.atom_data[1].y,
                 Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
    REQUIRE_THAT(result.atom_data[1].z,
                 Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
    REQUIRE(result.atom_data[1].is_fixed == true);
    REQUIRE(result.atom_data[1].atom_id == 1);

    REQUIRE_THAT(result.atom_data[2].x,
                 Catch::Matchers::WithinAbs(8.68229999999999968, fp_tol));
    REQUIRE_THAT(result.atom_data[2].y,
                 Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
    REQUIRE_THAT(result.atom_data[2].z,
                 Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
    REQUIRE(result.atom_data[2].is_fixed == false);
    REQUIRE(result.atom_data[2].atom_id == 2);

    REQUIRE_THAT(result.atom_data[3].x,
                 Catch::Matchers::WithinAbs(7.94209999999999550, fp_tol));
    REQUIRE_THAT(result.atom_data[3].y,
                 Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
    REQUIRE_THAT(result.atom_data[3].z,
                 Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
    REQUIRE(result.atom_data[3].is_fixed == false);
    REQUIRE(result.atom_data[3].atom_id == 3);
  }
  // Check the second ConFrame
  {
    const auto &result = results[1];
    REQUIRE(result.prebox_header[0] == "Random Number Seed");
    REQUIRE(result.prebox_header[1] == "Time");

    REQUIRE_THAT(result.boxl[0], Catch::Matchers::WithinAbs(15.345600, fp_tol));
    REQUIRE_THAT(result.boxl[1], Catch::Matchers::WithinAbs(21.702000, fp_tol));
    REQUIRE_THAT(result.boxl[2],
                 Catch::Matchers::WithinAbs(100.000000, fp_tol));

    REQUIRE_THAT(result.angles[0],
                 Catch::Matchers::WithinAbs(90.000000, fp_tol));
    REQUIRE_THAT(result.angles[1],
                 Catch::Matchers::WithinAbs(90.000000, fp_tol));
    REQUIRE_THAT(result.angles[2],
                 Catch::Matchers::WithinAbs(90.000000, fp_tol));

    REQUIRE(result.natm_types == 2);
    REQUIRE(result.natms_per_type[0] == 2);
    REQUIRE(result.natms_per_type[1] == 2);

    REQUIRE_THAT(result.masses_per_type[0],
                 Catch::Matchers::WithinAbs(63.546000, fp_tol));
    REQUIRE_THAT(result.masses_per_type[1],
                 Catch::Matchers::WithinAbs(1.007930, fp_tol));

    REQUIRE_THAT(result.atom_data[0].x,
                 Catch::Matchers::WithinAbs(0.63940000000000108, fp_tol));
    REQUIRE_THAT(result.atom_data[0].y,
                 Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
    REQUIRE_THAT(result.atom_data[0].z,
                 Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
    REQUIRE(result.atom_data[0].is_fixed == true);
    REQUIRE(result.atom_data[0].atom_id == 0);

    REQUIRE_THAT(result.atom_data[1].x,
                 Catch::Matchers::WithinAbs(3.19699999999999873, fp_tol));
    REQUIRE_THAT(result.atom_data[1].y,
                 Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
    REQUIRE_THAT(result.atom_data[1].z,
                 Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
    REQUIRE(result.atom_data[1].is_fixed == true);
    REQUIRE(result.atom_data[1].atom_id == 1);

    REQUIRE_THAT(result.atom_data[2].x,
                 Catch::Matchers::WithinAbs(8.85495714285713653, fp_tol));
    REQUIRE_THAT(result.atom_data[2].y,
                 Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
    REQUIRE_THAT(result.atom_data[2].z,
                 Catch::Matchers::WithinAbs(11.16538571428571380, fp_tol));
    REQUIRE(result.atom_data[2].is_fixed == false);
    REQUIRE(result.atom_data[2].atom_id == 2);

    REQUIRE_THAT(result.atom_data[3].x,
                 Catch::Matchers::WithinAbs(7.76944285714285154, fp_tol));
    REQUIRE_THAT(result.atom_data[3].y,
                 Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
    REQUIRE_THAT(result.atom_data[3].z,
                 Catch::Matchers::WithinAbs(11.16538571428571380, fp_tol));
    REQUIRE(result.atom_data[3].is_fixed == false);
    REQUIRE(result.atom_data[3].atom_id == 3);
  }
}