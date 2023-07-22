#include "include/ReadCon.hpp"
#include "include/BaseTypes.hpp"
#include "include/FormatConstants.hpp"

namespace yodecon {
void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrame &conframe) {
  types::AtomDatum tmp_atm;
  size_t drop_amount = constants::HeaderLength;
  for (size_t idx{0}; idx < conframe.natm_types; idx++) {
    size_t natms = conframe.natms_per_type[idx];
    size_t take_amount = natms + constants::CoordHeader;
    auto coords_view = a_filecontents | std::views::drop(drop_amount) |
                       std::views::take(take_amount);
    tmp_atm.symbol = coords_view[0];
    for (auto &&line : coords_view | std::views::drop(constants::CoordHeader)) {
      std::tie(tmp_atm.x, tmp_atm.y, tmp_atm.z, tmp_atm.is_fixed,
               tmp_atm.atom_id) =
          helpers::string::get_array_from_string<double, 5>(line);
      conframe.atom_data.push_back(tmp_atm);
    }
    drop_amount += take_amount;
  }
}

#ifdef WITH_ARROW
std::shared_ptr<arrow::Table>
ConvertToArrowTable(const yodecon::types::ConFrame &conFrame) {
  // Create builders for each field
  arrow::StringBuilder symbolBuilder;
  arrow::DoubleBuilder xBuilder, yBuilder, zBuilder;
  arrow::BooleanBuilder isFixedBuilder;
  arrow::UInt64Builder atomIdBuilder;

  // Iterate through the data and append it to the builders
  for (const auto &atomDatum : conFrame.atom_data) {
    symbolBuilder.Append(atomDatum.symbol);
    xBuilder.Append(atomDatum.x);
    yBuilder.Append(atomDatum.y);
    zBuilder.Append(atomDatum.z);
    isFixedBuilder.Append(atomDatum.is_fixed);
    atomIdBuilder.Append(atomDatum.atom_id);
  }

  // Finalize the arrays
  std::shared_ptr<arrow::Array> symbolArray;
  symbolBuilder.Finish(&symbolArray);
  std::shared_ptr<arrow::Array> xArray;
  xBuilder.Finish(&xArray);
  std::shared_ptr<arrow::Array> yArray;
  yBuilder.Finish(&yArray);
  std::shared_ptr<arrow::Array> zArray;
  zBuilder.Finish(&zArray);
  std::shared_ptr<arrow::Array> isFixedArray;
  isFixedBuilder.Finish(&isFixedArray);
  std::shared_ptr<arrow::Array> atomIdArray;
  atomIdBuilder.Finish(&atomIdArray);

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
#endif

} // namespace yodecon
