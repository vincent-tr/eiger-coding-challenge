#pragma once

#include <cstddef>
#include <vector>
#include <memory>

namespace eiger_coding_challenge::hash {
  struct rolling_hash;
}

namespace eiger_coding_challenge::fdiff {
  struct buffer;
  struct chunk_hash;

  struct hash_computer final {
    explicit hash_computer(std::size_t chunk_size);
    ~hash_computer();

    void append(const std::shared_ptr<buffer> &buf);
    void end();

    const std::vector<chunk_hash> &output() const {
      return m_output;
    }
  
  private:
    std::size_t m_chunk_size;
    std::unique_ptr<hash::rolling_hash> m_hasher;
    std::vector<chunk_hash> m_output;
    std::size_t m_block_start_offset = 0;
    std::size_t m_current_chunk_size = 0;
  };

}