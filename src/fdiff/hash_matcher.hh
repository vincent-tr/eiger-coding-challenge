#pragma once

#include <cstddef>
#include <vector>
#include <memory>
#include <unordered_map>

namespace eiger_coding_challenge {
  struct buffer;
}

namespace eiger_coding_challenge::hash {
  struct rolling_hash;
}

namespace eiger_coding_challenge::fdiff {
  struct chunk_hash;
  struct chunk_match;

  struct hash_matcher final {
    explicit hash_matcher(std::size_t chunk_size, const std::vector<chunk_hash> &chunks);
    ~hash_matcher();

    void append(const std::shared_ptr<buffer> &buf);
    void end();

    const std::vector<chunk_match> &output() const {
      return m_matchs;
    }

  private:
    struct window_back;

    void append_value(uint8_t value);

    std::size_t m_chunk_size;
    std::size_t m_window_size = 0;
    std::size_t m_current_offset = 0;
    std::unique_ptr<hash::rolling_hash> m_hasher;
    std::unordered_map<uint32_t, chunk_hash> m_chunks;
    std::vector<chunk_match> m_matchs;
    std::unique_ptr<window_back> m_window_back;
  };
}