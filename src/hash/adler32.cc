#include "rolling_hash.hh"

// based on https://github.com/lemire/rollinghashcpp/blob/master/adler32.h

namespace eiger_coding_challenge {
  namespace {

    struct adler32 : public rolling_hash {
      static constexpr uint32_t base = 65521;

      virtual ~adler32() = default;

      explicit adler32(int window)
       : len(window) {
      }

      void append(uint8_t inchar) override {
        sum1 = (sum1 + inchar) % base;
        sum2 = (sum2 + sum1) % base;

        hashvalue = (sum2 << 16) | sum1;
      }

      void update(uint8_t outchar, uint8_t inchar) override {
        int sum2 = (hashvalue >> 16) & 0xffff;
        int sum1 = hashvalue & 0xffff;

        sum1 += inchar - outchar;
        if (sum1 >= base) {
          sum1 -= base;
        } else if (sum1 < 0) {
          sum1 += base;
        }

        sum2 = ((int)(sum2 - len * outchar + sum1 - 1) % (int)base);

        if (sum2 < 0) {
          sum2 += base;
        }

        hashvalue = (sum2 << 16) | sum1;
      }

      uint32_t output() const override {
        return hashvalue;
      }

    private:
      uint32_t sum1 = 1;
      uint32_t sum2 = 0;
      uint32_t hashvalue = 0;
      int len;
    };
  }

  std::unique_ptr<rolling_hash> rolling_hash::create_adler32(int window) {
    return std::make_unique<adler32>(window);
  }
}
