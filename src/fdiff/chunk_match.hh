#pragma once

#include <cstddef>
#include "chunk_hash.hh"

namespace eiger_coding_challenge::fdiff {

  struct chunk_match final {
    chunk_match(const chunk_hash &source, std::size_t offset)
     : m_source(source)
     , m_offset(offset) {
    }

    const chunk_hash &source() const {
      return m_source;
    }

    std::size_t offset() const {
      return m_offset;
    }

    std::size_t source_offset() const {
      return m_source.offset();
    }

    std::size_t size() const {
      return m_source.size();
    }

    friend auto operator<=>(const chunk_match&, const chunk_match&) = default;

  private:
    chunk_hash m_source;
    std::size_t m_offset;
  };
}