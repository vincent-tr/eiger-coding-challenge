#include <gtest/gtest.h>

#include "rolling_hash.hh"

namespace eiger_coding_challenge::hash {
  namespace {

    static constexpr int window = 4;

    void append_string(rolling_hash *hash, const std::string_view &sv) {
      for(std::size_t i=0; i<sv.size(); ++i) {
        auto inchar = sv[i];
        if (i >= window) {
          auto outchar = sv[i - window];
          hash->update(outchar, inchar);
        } else {
          hash->append(inchar);
        }
      }
    }

    TEST(RollingHash, Basic) {
      auto hash1 = rolling_hash::create_adler32(window);
      auto hash2 = rolling_hash::create_adler32(window);

      append_string(hash1.get(), "12345678");
      append_string(hash2.get(), "12345678");

      ASSERT_EQ(hash1->output(), hash2->output());
    }

    TEST(RollingHash, Rolling) {
      auto hash1 = rolling_hash::create_adler32(window);
      auto hash2 = rolling_hash::create_adler32(window);

      append_string(hash1.get(), "12345678");
      append_string(hash2.get(), "5678");

      ASSERT_EQ(hash1->output(), hash2->output());
    }
  }
}