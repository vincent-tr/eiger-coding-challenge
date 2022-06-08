#include "rolling_hash.hh"

// based on https://github.com/lemire/rollinghashcpp/blob/master/adler32.h

namespace eiger_coding_challenge::hash {
  namespace {

    struct adler32 : public rolling_hash {
      static constexpr uint32_t base = 65521;

      virtual ~adler32() = default;

      explicit adler32(std::size_t window)
       : m_len(window) {
      }

      void append(uint8_t inchar) override {
        m_sum1 = (m_sum1 + inchar) % base;
        m_sum2 = (m_sum2 + m_sum1) % base;

        m_hashvalue = (m_sum2 << 16) | m_sum1;
      }

      void update(uint8_t outchar, uint8_t inchar) override {
        int m_sum2 = (m_hashvalue >> 16) & 0xffff;
        int m_sum1 = m_hashvalue & 0xffff;

        m_sum1 += inchar - outchar;
        if (m_sum1 >= base) {
          m_sum1 -= base;
        } else if (m_sum1 < 0) {
          m_sum1 += base;
        }

        m_sum2 = ((int)(m_sum2 - m_len * outchar + m_sum1 - 1) % (int)base);

        if (m_sum2 < 0) {
          m_sum2 += base;
        }

        m_hashvalue = (m_sum2 << 16) | m_sum1;
      }

      uint32_t output() const override {
        return m_hashvalue;
      }

    private:
      uint32_t m_sum1 = 1;
      uint32_t m_sum2 = 0;
      uint32_t m_hashvalue = 0;
      std::size_t m_len;
    };
  }

  std::unique_ptr<rolling_hash> rolling_hash::create_adler32(std::size_t window) {
    return std::make_unique<adler32>(window);
  }
}
