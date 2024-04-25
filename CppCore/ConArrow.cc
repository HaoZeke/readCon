// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#ifdef WITH_ARROW
#include "readCon/include/ConArrow.hpp"

namespace yodecon::conarrow {
std::shared_ptr<arrow::Table>
ConvertToArrowTable(const yodecon::types::ConFrame &conFrame) {
  // Create builders for each field
  arrow::StringBuilder symbolBuilder;
  arrow::DoubleBuilder xBuilder, yBuilder, zBuilder;
  arrow::BooleanBuilder isFixedBuilder;
  arrow::UInt64Builder atomIdBuilder;

  // Iterate through the data and append it to the builders
  for (const auto &atomDatum : conFrame.atom_data) {
    CHECK_ARROW_STATUS(symbolBuilder.Append(atomDatum.symbol));
    CHECK_ARROW_STATUS(xBuilder.Append(atomDatum.x));
    CHECK_ARROW_STATUS(yBuilder.Append(atomDatum.y));
    CHECK_ARROW_STATUS(zBuilder.Append(atomDatum.z));
    CHECK_ARROW_STATUS(isFixedBuilder.Append(atomDatum.is_fixed));
    CHECK_ARROW_STATUS(atomIdBuilder.Append(atomDatum.atom_id));
  }

  // Finalize the arrays
  std::shared_ptr<arrow::Array> symbolArray;
  CHECK_ARROW_STATUS(symbolBuilder.Finish(&symbolArray));
  std::shared_ptr<arrow::Array> xArray;
  CHECK_ARROW_STATUS(xBuilder.Finish(&xArray));
  std::shared_ptr<arrow::Array> yArray;
  CHECK_ARROW_STATUS(yBuilder.Finish(&yArray));
  std::shared_ptr<arrow::Array> zArray;
  CHECK_ARROW_STATUS(zBuilder.Finish(&zArray));
  std::shared_ptr<arrow::Array> isFixedArray;
  CHECK_ARROW_STATUS(isFixedBuilder.Finish(&isFixedArray));
  std::shared_ptr<arrow::Array> atomIdArray;
  CHECK_ARROW_STATUS(atomIdBuilder.Finish(&atomIdArray));

  // Create a vector of fields to describe the schema
  std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
      arrow::field("symbol", arrow::utf8()),
      arrow::field("x", arrow::float64()),
      arrow::field("y", arrow::float64()),
      arrow::field("z", arrow::float64()),
      arrow::field("is_fixed", arrow::boolean()),
      arrow::field("atom_id", arrow::uint64())};

  // Create a schema
  auto schema = std::make_shared<arrow::Schema>(schema_vector);

  std::vector<std::string> keys = {
      "prebox_header", "boxl",           "angles",         "postbox_header",
      "natm_types",    "natms_per_type", "masses_per_type"};

  std::vector<std::string> values = {
      yodecon::helpers::string::to_csv_string(conFrame.prebox_header),
      yodecon::helpers::string::to_csv_string(conFrame.boxl),
      yodecon::helpers::string::to_csv_string(conFrame.angles),
      yodecon::helpers::string::to_csv_string(conFrame.postbox_header),
      std::to_string(conFrame.natm_types),
      yodecon::helpers::string::to_csv_string(conFrame.natms_per_type),
      yodecon::helpers::string::to_csv_string(conFrame.masses_per_type)};

  // Create metadata
  auto metadata = std::make_shared<arrow::KeyValueMetadata>(keys, values);

  // Add metadata to schema
  schema = schema->WithMetadata(metadata);

  // Create a vector of arrays corresponding to the schema
  std::vector<std::shared_ptr<arrow::Array>> array_vector = {
      symbolArray, xArray, yArray, zArray, isFixedArray, atomIdArray};

  // Create a table
  auto table = arrow::Table::Make(schema, array_vector);

  return table;
}

std::shared_ptr<arrow::RecordBatch>
get_chunk_as_record_batch(std::shared_ptr<arrow::Table> table,
                          int chunk_index) {
  std::vector<std::shared_ptr<arrow::Array>> chunks;
  for (int i = 0; i < table->num_columns(); i++) {
    chunks.push_back(table->column(i)->chunk(chunk_index));
  }

  // Reconstruct a RecordBatch using the extracted chunks and the original
  // table's schema
  return arrow::RecordBatch::Make(table->schema(), chunks[0]->length(), chunks);
}

} // namespace yodecon::conarrow

#endif // WITH_ARROW
