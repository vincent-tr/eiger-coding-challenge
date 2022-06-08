#pragma once

#include <cstddef>
#include <memory>

namespace eiger_coding_challenge {

  struct rolling_hash {
    virtual ~rolling_hash() = default;

    virtual uint32_t output() const = 0;
    virtual void append(uint8_t inchar) = 0;
    virtual void update(uint8_t outchar, uint8_t inchar) = 0;

    static std::unique_ptr<rolling_hash> create_adler32(int window);
  };
}