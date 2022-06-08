#pragma once

#include <cstddef>
#include <vector>
#include <memory>
#include <unordered_map>
#include "hash/rolling_hash.hh"
#include "buffer.hh"
#include "chunk_hash.hh"

namespace eiger_coding_challenge::hash {
  struct rolling_hash;
}

namespace eiger_coding_challenge::fdiff {
  struct buffer;
  struct chunk_hash;
  struct chunk_match;

  struct hash_matcher final {
    explicit hash_matcher(std::size_t chunk_size, const std::vector<chunk_hash> &chunks);
    ~hash_matcher();

    void append(const std::shared_ptr<buffer> &buf);

    const std::vector<chunk_match> &output() const {
      return m_matchs;
    }

  private:
    struct window_back;

    std::size_t m_chunk_size;
    std::size_t m_start_offset = 0;
    std::size_t m_current_offset = 0;
    std::unique_ptr<hash::rolling_hash> m_hasher;
    std::unordered_map<uint32_t, chunk_hash> m_chunks;
    std::vector<chunk_match> m_matchs;
    std::unique_ptr<window_back> m_window_back;
  };
}