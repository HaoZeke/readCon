// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#include "readCon/include/ReadCon.hpp"
#include "readCon/include/adapters/xtensor.hpp"
#include <catch2/catch_all.hpp>

#ifdef WITH_XTENSOR

TEST_CASE("extract_positions correctly extracts position vectors",
          "[extract_positions]") {
  yodecon::types::ConFrameVec conFrameVec;
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
  auto positions = yodecon::types::adapt::xts::extract_positions(conFrameVec);

  REQUIRE(positions.shape()[0] == 1); // 1 atoms
  REQUIRE(positions.shape()[1] == 3); // 3 dimensions

  REQUIRE(positions(0, 0) == 10.477713);
  REQUIRE(positions(0, 1) == 12.379463);
  REQUIRE(positions(0, 2) == 12.871778);
}

#endif
