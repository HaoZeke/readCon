#ifdef WITH_ARROW
#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
// clang-format off
#include <arrow/api.h>
#include <memory>
// clang-format on
#include "include/BaseTypes.hpp"
#include "include/helpers/StringHelpers.hpp"

#define CHECK_ARROW_STATUS(status)                                             \
  do {                                                                         \
    auto _s = (status);                                                        \
    if (!_s.ok()) {                                                            \
      throw std::runtime_error(_s.ToString());                                 \
    }                                                                          \
  } while (0)

namespace yodecon::conarrow {
/**
 * @brief Converts a ConFrame data structure to an Apache Arrow Table.
 *
 * This function iterates through all the atomic data in a ConFrame structure,
 * populating corresponding Arrow builders to create an Arrow Table with
 * metadata. Each atom's data, including symbol, coordinates, fixation status,
 * and ID, is processed to form the respective columns in the resulting table.
 *
 * @param conFrame The ConFrame structure containing the data to be converted.
 * @return A shared pointer to an Apache Arrow Table containing the converted
 * data.
 *
 * @details The function sets up multiple Arrow builders for different types
 * of atomic data. It handles the conversion process through the following
 * steps:
 * - Appending data to the builders.
 * - Finalizing the builders to create Arrow arrays.
 * - Constructing the schema and metadata for the table.
 * - Creating and returning the final Arrow Table with all arrays and schema.
 *
 * @note Ensure that all Arrow status checks pass, otherwise, exceptions might
 * be thrown due to failed Arrow operations.
 */
std::shared_ptr<arrow::Table>
ConvertToArrowTable(const yodecon::types::ConFrame &conFrame);

/**
 * @brief Retrieves a specified chunk from an Apache Arrow Table as a
 * RecordBatch.
 *
 * This function extracts a chunk from each column of an Arrow Table based on
 * the provided index and reconstructs it into a RecordBatch. The RecordBatch
 * maintains the same schema as the original table, facilitating operations that
 * require handling segments of data independently while preserving the overall
 * data structure integrity.
 *
 * @param table The shared pointer to an Apache Arrow Table from which to
 * extract the chunk.
 * @param chunk_index The index of the chunk to be extracted, typically based on
 * chunk division of the table data.
 * @return A shared pointer to an Apache Arrow RecordBatch comprising the
 * extracted chunks.
 *
 * @details The function performs the extraction by iterating through all
 * columns of the table, extracting the specified chunk by index from each
 * column. These chunks are then used to construct a new RecordBatch that
 * reflects a segment of the original table.
 *
 * @note If the specified chunk_index is out of range for any column, the
 * function may throw an exception or result in undefined behavior depending on
 * Arrow's handling of out-of-range indices. Ensure the chunk_index is valid for
 * all columns to avoid runtime errors.
 *
 * @see arrow::RecordBatch::Make for details on how the RecordBatch is
 * constructed.
 */
std::shared_ptr<arrow::RecordBatch>
get_chunk_as_record_batch(std::shared_ptr<arrow::Table> table, int chunk_index);

} // namespace yodecon::conarrow

#endif // WITH_ARROW
