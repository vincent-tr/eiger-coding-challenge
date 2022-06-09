#pragma once

#include <ostream>
#include <cstddef>

namespace eiger_coding_challenge::fdiff {

  struct chunk_hash final {
    chunk_hash(uint32_t hash, std::size_t offset, std::size_t size)
     : m_hash(hash)
     , m_offset(offset)
     , m_size(size) {
    }

    uint32_t hash() const {
      return m_hash;
    }

    std::size_t offset() const {
      return m_offset;
    }

    std::size_t size() const {
      return m_size;
    }

    friend auto operator<=>(const chunk_hash&, const chunk_hash&) = default;

    friend std::ostream& operator<< (std::ostream& os, const chunk_hash& val) {
      return os << "{ hash=" << val.hash() << "; offset=" << val.offset() << "; size=" << val.size() << " }";
    }

  private:
    uint32_t m_hash;
    std::size_t m_offset;
    std::size_t m_size;
  };
}