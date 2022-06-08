#include <gtest/gtest.h>

#include "hash.hh"

namespace eiger_coding_challenge {
  namespace {

    TEST(Hash, Basic) {
      ASSERT_EQ(hash(1), 1);
    }

  }
}