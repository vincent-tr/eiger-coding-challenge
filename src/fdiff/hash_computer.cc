#include <cstddef>
#include <vector>
#include <memory>
#include "hash/rolling_hash.hh"
#include "hash_computer.hh"
#include "buffer.hh"
#include "chunk_hash.hh"

namespace eiger_coding_challenge::fdiff {

  hash_computer::hash_computer(std::size_t chunk_size)
    : m_chunk_size(chunk_size)
    , m_hasher(hash::rolling_hash::create_adler32(m_chunk_size)) {
  }

  hash_computer::~hash_computer() = default;

  void hash_computer::append(const std::shared_ptr<buffer> &buf) {
    for (std::size_t offset = 0; offset < buf->size(); ++offset) {
      m_hasher->append(buf->at(offset));
      ++m_current_chunk_size;

      if (m_current_chunk_size == m_chunk_size) {
        m_output.emplace_back(m_hasher->output(), m_block_start_offset, m_current_chunk_size);
        
        m_hasher = hash::rolling_hash::create_adler32(m_chunk_size);
        m_block_start_offset += m_current_chunk_size;
        m_current_chunk_size = 0;
      }
    }
  }

  void hash_computer::end() {
    if (m_current_chunk_size == 0) {
      return;
    }

    // pad with 0 to get a full window (else it cannot be matched)
    for (std::size_t i=0; i < m_chunk_size - m_current_chunk_size; ++i) {
      m_hasher->append(0);
    }

    m_output.emplace_back(m_hasher->output(), m_block_start_offset, m_current_chunk_size);
  }

}