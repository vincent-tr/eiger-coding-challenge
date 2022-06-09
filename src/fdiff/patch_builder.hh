#pragma once

#include <cstddef>
#include <vector>

namespace eiger_coding_challenge::fdiff {
  struct chunk_match;
  struct chunk_patch;

  std::vector<chunk_patch> build_patch(std::size_t file_size, const std::vector<chunk_match> &matches);
}