#include <gtest/gtest.h>

#include "hash_computer.hh"
#include "buffer.hh"
#include "chunk_hash.hh"

namespace eiger_coding_challenge::fdiff {
  namespace {

    static constexpr int chunk_size = 4;

    TEST(FDiffComputer, Basic) {
      hash_computer computer(4);

      computer.append(buffer::from_string_view("123456789"));
      computer.end();

      auto output = computer.output();

      // Note: hash numbers issued from first tests 
      ASSERT_EQ(output, (std::vector<chunk_hash>{
        {33030347, 0, 4},
        {35651803, 4, 4},
        {3801146, 8, 1},
      }));
    }
  }
}