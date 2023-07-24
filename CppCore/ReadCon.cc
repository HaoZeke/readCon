#include "include/ReadCon.hpp"

namespace yodecon {
void process_coordinates(const std::vector<std::string> &a_filecontents,
                         yodecon::types::ConFrame &conframe) {
  types::AtomDatum tmp_atm;
  size_t drop_amount = constants::HeaderLength;
  for (size_t idx{0}; idx < conframe.natm_types; idx++) {
    size_t natms = conframe.natms_per_type[idx];
    size_t take_amount = natms + constants::CoordHeader;
    auto coords_view = a_filecontents | ranges::views::drop(drop_amount) |
                       ranges::views::take(take_amount);
    tmp_atm.symbol = coords_view[0];
    for (auto &&line :
         coords_view | ranges::views::drop(constants::CoordHeader)) {
      auto dbl_line = helpers::string::get_array_from_string<double, 5>(line);
      tmp_atm.x = dbl_line[0];
      tmp_atm.y = dbl_line[1];
      tmp_atm.z = dbl_line[2];
      tmp_atm.is_fixed = static_cast<bool>(dbl_line[3]);
      tmp_atm.atom_id = static_cast<size_t>(dbl_line[4]);
      conframe.atom_data.push_back(tmp_atm);
    }
    drop_amount += take_amount;
  }
}

std::vector<size_t>
symbols_to_atomic_numbers(const std::vector<std::string> &a_symbols) {
  std::vector<size_t> numbers;
  for (const auto &symbol : a_symbols) {
    try {
      numbers.push_back(yodecon::types::known_info::AtomicNumbers.at(symbol));
    } catch (const std::out_of_range &e) {
      throw std::invalid_argument("Invalid element symbol: " + symbol);
    }
  }
  return numbers;
}

std::vector<std::string>
atomic_numbers_to_symbols(const std::vector<size_t> &a_atomic_numbers) {
  // This is linear in the number of elements, would be faster to have
  // AtomicSymbols, as a map has O(1). However, there are only 118 elements and
  // so this is a moot point, and adding another const map is probably a worse
  // trade-off
  std::vector<std::string> symbols;
  for (const auto &number : a_atomic_numbers) {
    auto it = std::find_if(
        yodecon::types::known_info::AtomicNumbers.begin(),
        yodecon::types::known_info::AtomicNumbers.end(),
        [&number](const auto &pair) { return pair.second == number; });

    if (it == yodecon::types::known_info::AtomicNumbers.end()) {
      throw std::invalid_argument("Invalid atomic number: " +
                                  std::to_string(number));
    }
    symbols.push_back(it->first);
  }
  return symbols;
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

#endif

} // namespace yodecon
