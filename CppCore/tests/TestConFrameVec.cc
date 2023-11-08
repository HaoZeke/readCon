/*
 * TestConFrameVec.cc
 *
 *  Created on: 25 July 2024
 *      Author: Rohit Goswami
 *     Company: University of Iceland
 */

#include "../include/ReadCon.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace yodecon::types;
constexpr double fp_tol{1e-12};

TEST_CASE("ConFrameVecTest - Constructor", "[ConFrameVec]") {
  ConFrameVec conFrameVec;
  conFrameVec.prebox_header = {"Generated by eOn", ""};
  conFrameVec.boxl = {25.0, 25.0, 25.0};
  conFrameVec.angles = {90.0, 90.0, 90.0};
  conFrameVec.postbox_header = {"", ""};
  conFrameVec.natm_types = 4;
  conFrameVec.natms_per_type = {2, 4, 6, 1};
  conFrameVec.masses_per_type = {15.99, 12.011, 1.008, 32.065};
  conFrameVec.symbol.push_back("O");
  conFrameVec.x.push_back(10.477713);
  conFrameVec.y.push_back(12.379463);
  conFrameVec.z.push_back(12.871778);
  conFrameVec.is_fixed.push_back(false);
  conFrameVec.atom_id.push_back(1);

  REQUIRE("Generated by eOn" == conFrameVec.prebox_header[0]);
  REQUIRE_THAT(conFrameVec.boxl[0], Catch::Matchers::WithinAbs(25.0, fp_tol));
  REQUIRE_THAT(conFrameVec.angles[0], Catch::Matchers::WithinAbs(90.0, fp_tol));
  REQUIRE(conFrameVec.natm_types == 4);
  REQUIRE(conFrameVec.natms_per_type[0] == 2);
  REQUIRE_THAT(conFrameVec.masses_per_type[0],
               Catch::Matchers::WithinAbs(15.99, fp_tol));
  REQUIRE(conFrameVec.symbol[0] == "O");
  REQUIRE_THAT(conFrameVec.x[0], Catch::Matchers::WithinAbs(10.477713, fp_tol));
  REQUIRE_THAT(conFrameVec.y[0], Catch::Matchers::WithinAbs(12.379463, fp_tol));
  REQUIRE_THAT(conFrameVec.z[0], Catch::Matchers::WithinAbs(12.871778, fp_tol));
  REQUIRE(false == conFrameVec.is_fixed[0]);
  REQUIRE(1 == conFrameVec.atom_id[0]);
}

TEST_CASE("ConFrameVecTest - CreateSingleCon_SingleFile", "[ConFrameVec]") {
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
  ConFrameVec result = yodecon::create_single_con<ConFrameVec>(a_fconts);
  // Check the resulting ConFrame object against expected values.
  REQUIRE("Random Number Seed" == result.prebox_header[0]);
  REQUIRE("Time" == result.prebox_header[1]);
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
  REQUIRE(result.symbol[0] == "Cu");
  REQUIRE_THAT(result.x[0],
               Catch::Matchers::WithinAbs(0.63940000000000108, fp_tol));
  REQUIRE_THAT(result.y[0],
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.z[0],
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(true == result.is_fixed[0]);
  REQUIRE(0 == result.atom_id[0]);

  REQUIRE_THAT(result.symbol[1], Catch::Matchers::Equals("Cu"));
  REQUIRE_THAT(result.x[1],
               Catch::Matchers::WithinAbs(3.19699999999999873, fp_tol));
  REQUIRE_THAT(result.y[1],
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.z[1],
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.is_fixed[1] == true);
  REQUIRE(result.atom_id[1] == 1);

  REQUIRE_THAT(result.symbol[2], Catch::Matchers::Equals("H"));
  REQUIRE_THAT(result.x[2],
               Catch::Matchers::WithinAbs(8.68229999999999968, fp_tol));
  REQUIRE_THAT(result.y[2],
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.z[2],
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE(result.is_fixed[2] == false);
  REQUIRE(result.atom_id[2] == 2);

  REQUIRE_THAT(result.symbol[3], Catch::Matchers::Equals("H"));
  REQUIRE_THAT(result.x[3],
               Catch::Matchers::WithinAbs(7.94209999999999550, fp_tol));
  REQUIRE_THAT(result.y[3],
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.z[3],
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE(result.is_fixed[3] == false);
  REQUIRE(result.atom_id[3] == 3);
}

TEST_CASE("ConFrameVecTest - CreateSingleCon_MultiFile", "[ConFrameVec]") {
  // The actual lines of the multi con file.
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
  ConFrameVec result = yodecon::create_single_con<ConFrameVec>(a_fconts);
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

  REQUIRE(result.symbol[0] == "Cu");
  REQUIRE_THAT(result.x[0],
               Catch::Matchers::WithinAbs(0.63940000000000108, fp_tol));
  REQUIRE_THAT(result.y[0],
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.z[0],
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.is_fixed[0] == true);
  REQUIRE(result.atom_id[0] == 0);

  REQUIRE(result.symbol[1] == "Cu");
  REQUIRE_THAT(result.x[1],
               Catch::Matchers::WithinAbs(3.19699999999999873, fp_tol));
  REQUIRE_THAT(result.y[1],
               Catch::Matchers::WithinAbs(0.90450000000000019, fp_tol));
  REQUIRE_THAT(result.z[1],
               Catch::Matchers::WithinAbs(6.97529999999999539, fp_tol));
  REQUIRE(result.is_fixed[1] == true);
  REQUIRE(result.atom_id[1] == 1);

  REQUIRE(result.symbol[2] == "H");
  REQUIRE_THAT(result.x[2],
               Catch::Matchers::WithinAbs(8.68229999999999968, fp_tol));
  REQUIRE_THAT(result.y[2],
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.z[2],
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE(result.is_fixed[2] == false);
  REQUIRE(result.atom_id[2] == 2);

  REQUIRE(result.symbol[3] == "H");
  REQUIRE_THAT(result.x[3],
               Catch::Matchers::WithinAbs(7.94209999999999550, fp_tol));
  REQUIRE_THAT(result.y[3],
               Catch::Matchers::WithinAbs(9.94699999999999740, fp_tol));
  REQUIRE_THAT(result.z[3],
               Catch::Matchers::WithinAbs(11.73299999999999343, fp_tol));
  REQUIRE(result.is_fixed[3] == false);
  REQUIRE(result.atom_id[3] == 3);
}
