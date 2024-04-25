#ifdef WITH_ARROW
#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>
#include "include/BaseTypes.hpp"
#include "include/helpers/StringHelpers.hpp"
#include <arrow/api.h>
#include <memory>

#define CHECK_ARROW_STATUS(status)                                             \
  do {                                                                         \
    auto _s = (status);                                                        \
    if (!_s.ok()) {                                                            \
      throw std::runtime_error(_s.ToString());                                 \
    }                                                                          \
  } while (0)

namespace yodecon::conarrow {

std::shared_ptr<arrow::Table>
ConvertToArrowTable(const yodecon::types::ConFrame &conFrame);

std::shared_ptr<arrow::RecordBatch>
get_chunk_as_record_batch(std::shared_ptr<arrow::Table> table, int chunk_index);

} // namespace yodecon::conarrow

#endif // WITH_ARROW
