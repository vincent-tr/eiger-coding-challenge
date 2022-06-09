#pragma once

#include <ostream>
#include <cstddef>
#include "chunk_hash.hh"

namespace eiger_coding_challenge::fdiff {

  struct chunk_match final {
    chunk_match(std::size_t old_offset, std::size_t size, std::size_t new_offset)
     : m_old_offset(old_offset)
     , m_size(size)
     , m_new_offset(new_offset) {
    }

    std::size_t old_offset() const {
      return m_old_offset;
    }

    std::size_t size() const {
      return m_size;
    }

    std::size_t new_offset() const {
      return m_new_offset;
    }

    friend auto operator<=>(const chunk_match&, const chunk_match&) = default;

    friend std::ostream& operator<< (std::ostream& os, const chunk_match& val) {
      return os << "{ old_offset=" << val.old_offset() << "; size=" << val.size() << "; new_offset=" << val.new_offset() << " }";
    }

  private:
    std::size_t m_old_offset;
    std::size_t m_size;
    std::size_t m_new_offset;
  };
}