// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#include "readCon/include/BaseTypes.hpp"
#include "readCon/include/FormatConstants.hpp"
#include "readCon/include/Helpers.hpp"
#include "readCon/include/ReadCon.hpp"
#include "readCon/include/helpers/StringHelpers.hpp"

#ifdef WITH_FMT
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#endif

#ifdef WITH_ARROW
#include <arrow/csv/api.h>
#include <arrow/filesystem/api.h>
#include <arrow/io/api.h>
#include <arrow/io/interfaces.h>
#include <arrow/ipc/api.h>
#include <arrow/table.h>
#endif

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  // std::array<size_t, 4> header_linum = {1, 2, 5, 6};
  // const size_t boxl_linum = 3;
  // const size_t angle_linum = 4;
  // const size_t natom_types_linum = 7;
  // const size_t natoms_per_type_linum = 8;
  // const size_t masses_per_type_linum = 9;

  std::vector<std::string> fconts =
      yodecon::helpers::file::read_con_file(filename);

  auto tmp = yodecon::create_single_con<yodecon::types::ConFrameVec>(fconts);

  //   // yodecon::types::ConFrame tmp;

  //   // yodecon::process_header(
  //   //     (fconts | ranges::views::take(yodecon::constants::HeaderLength)),
  //   tmp);

#ifdef WITH_FMT
  fmt::print("prebox_headers: {}\n", tmp.prebox_header);
  fmt::print("box lengths: {}\n", tmp.boxl);
  fmt::print("angles: {}\n", tmp.angles);
  fmt::print("postbox_headers: {}\n", tmp.postbox_header);
  fmt::print("natm_types: {}\n", tmp.natm_types);
  fmt::print("natms_per_type: {}\n", tmp.natms_per_type);
  fmt::print("masses_per_type: {}\n", tmp.masses_per_type);
#endif

  //   //   yodecon::process_coordinates(fconts, tmp);

#ifdef WITH_FMT
  // // ConFrame
  // fmt::print("{}\n", tmp.atom_data.size());
  // for (auto &&tmp_id : tmp.atom_data) {
  //   fmt::print("{} {} {} {} {} {}\n", tmp_id.symbol, tmp_id.x, tmp_id.y,
  //              tmp_id.z, tmp_id.is_fixed, tmp_id.atom_id);
  // }

  // ConFrameVec
  // TODO(rg): Doesn't work out of the box on newer fmt (10)
  // Provide a formatter
  // size_t framesize =
  //     std::accumulate(tmp.natms_per_type.begin(), tmp.natms_per_type.end(),
  //     0);
  // for (size_t idx{0}; idx < framesize; idx++) {
  //   fmt::print("{} {} {} {} {} {}\n", tmp.symbol[idx], tmp.x[idx],
  //   tmp.y[idx],
  //              tmp.z[idx], tmp.is_fixed[idx], tmp.atom_id[idx]);
  // }

  // auto atmnums = yodecon::symbols_to_atomic_numbers(tmp.symbol);
  // fmt::print("\n");
  // for (auto &&atmnm : atmnums) {
  //   fmt::print("{} ", atmnm);
  // }
  // fmt::print("\n");
  // for (auto &&sym : tmp.symbol) {
  //   fmt::print("{} ", sym);
  // }
  // fmt::print("\n");
  // std::cout << yodecon::helpers::string::to_csv_string(tmp.prebox_header)
  //           << "\n";
  // std::cout << yodecon::helpers::string::to_csv_string(tmp.boxl) << "\n";
  // std::cout << yodecon::helpers::string::to_csv_string(tmp.angles) << "\n";
  // std::cout << yodecon::helpers::string::to_csv_string(tmp.postbox_header)
  //           << "\n";
  // std::cout << std::to_string(tmp.natm_types) << "\n";
  // std::cout << yodecon::helpers::string::to_csv_string(tmp.natms_per_type)
  //           << "\n";
  // std::cout << yodecon::helpers::string::to_csv_string(tmp.masses_per_type)
  //           << "\n";
#endif

  // #ifdef WITH_ARROW
  //   // Convert to Arrow table
  //   std::shared_ptr<arrow::Table> table = yodecon::ConvertToArrowTable(tmp);

  //   // Write the table to a file
  //   auto outfile = arrow::io::FileOutputStream::Open("test_out.csv");
  //   auto write_options = arrow::csv::WriteOptions::Defaults();
  //   if (WriteCSV(*table, write_options, outfile->get()).ok()) {
  //     // Handle writer error...
  //   }
  //   // Read the table back from the file
  //   std::shared_ptr<arrow::io::ReadableFile> infile;

  //   // Print the schema's metadata
  //   std::shared_ptr<const arrow::KeyValueMetadata> metadata =
  //       table->schema()->metadata();
  //   for (int i = 0; i < metadata->size(); ++i) {
  //     std::cout << metadata->key(i) << ": " << metadata->value(i) <<
  //     std::endl;
  //   }

  //   // Get recordbatch
  //   std::shared_ptr<arrow::RecordBatch> rbatch =
  //       yodecon::get_chunk_as_record_batch(table, 0);

  //   // Print the table
  //   // fmt::print("{} {} {} {} {} {}",
  //   table->column(0)->chunk(0)->ToString(),
  //   //            table->column(1)->chunk(0)->ToString(),
  //   //            table->column(2)->chunk(0)->ToString(),
  //   //            table->column(3)->chunk(0)->ToString(),
  //   //            table->column(4)->chunk(0)->ToString(),
  //   //            table->column(5)->chunk(0)->ToString(),
  // #endif

  return EXIT_SUCCESS;
}
