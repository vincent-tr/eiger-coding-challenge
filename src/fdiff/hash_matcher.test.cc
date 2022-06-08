#include <gtest/gtest.h>

#include "hash_matcher.hh"
#include "buffer.hh"
#include "chunk_hash.hh"
#include "chunk_match.hh"

namespace eiger_coding_challenge::fdiff {
  namespace {

    static constexpr int chunk_size = 4;

    TEST(FDiffMatcher, Basic) {
      // output of hash computer for "123456789"
      std::vector<chunk_hash> chunks{
        {33030347, 0, 4},
        {35651803, 4, 4},
        {3801146, 8, 1},
      };

      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("123456789"));

      auto output = matcher.output();
      // ASSERT
    }
  }
}