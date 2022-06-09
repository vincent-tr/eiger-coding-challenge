#pragma once

#include <ostream>
#include <cstddef>
#include "chunk_hash.hh"

namespace eiger_coding_challenge::fdiff {

  enum class patch_type { reuse, synchronize };

  struct chunk_patch final {

    chunk_patch(patch_type type, std::size_t source_offset, std::size_t size)
     : m_type(type)
     , m_source_offset(source_offset)
     , m_size(size) {
    }

    patch_type type() const { 
      return m_type;
    }

    /**
     * if type = reuse, offset in the original file
     * if type = synchronize, while building offset in the new file, then offset in the delta file
     */
    std::size_t source_offset() const { 
      return m_source_offset;
    }

    std::size_t size() const { 
      return m_size;
    }
    
    friend auto operator<=>(const chunk_patch&, const chunk_patch&) = default;

    friend std::ostream& operator<< (std::ostream& os, const chunk_patch& val) {
      auto stype = val.type() == patch_type::reuse ? "reuse" : "synchronize";
      return os << "{ type=" << stype << "; source_offset=" << val.source_offset() << "; size=" << val.size() << " }";
    }

  private:
    patch_type m_type;
    std::size_t m_source_offset;
    std::size_t m_size;
  };
}