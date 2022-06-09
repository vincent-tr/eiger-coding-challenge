#include <cstddef>
#include <queue>
#include "patch_builder.hh"
#include "chunk_match.hh"
#include "chunk_patch.hh"

namespace eiger_coding_challenge::fdiff {
  struct chunk_match;
  struct chunk_patch;

  // Note: matches are supposed to be ordered.
  std::vector<chunk_patch> build_patch(std::size_t file_size, const std::vector<chunk_match> &matches) {
    std::size_t current_offset = 0;
    std::vector<chunk_patch> patch;

    for (const auto &match : matches) {
      if (match.new_offset() > current_offset) {
        // we have a hole, take it from new file
        auto size = match.new_offset() - current_offset;
        patch.emplace_back(patch_type::synchronize, current_offset, size);
        current_offset += size;
      }

      patch.emplace_back(patch_type::reuse, match.old_offset(), match.size());
      current_offset += match.size();
    }

    if (current_offset < file_size) {
      auto size = file_size - current_offset;
      patch.emplace_back(patch_type::synchronize, current_offset, size);
    }

    return patch;
  }
}