#include <cstddef>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include "hash/rolling_hash.hh"
#include "hash_matcher.hh"
#include "buffer.hh"
#include "chunk_hash.hh"
#include "chunk_match.hh"

namespace eiger_coding_challenge::fdiff {
  struct hash_matcher::window_back final {
    void push_buffer(std::shared_ptr<buffer> buf) {
      m_buffers.emplace(std::move(buf));
    }

    uint8_t pop() {
      const auto &buffer = m_buffers.front();
      uint8_t value = buffer->at(m_buffer_offset++);

      if (m_buffer_offset == buffer->size()) {
        m_buffer_offset = 0;
        m_buffers.pop();
      }

      return value;
    }

  private:
    std::queue<std::shared_ptr<buffer>> m_buffers;
    std::size_t m_buffer_offset = 0;
  };

  hash_matcher::hash_matcher(std::size_t chunk_size, const std::vector<chunk_hash> &chunks)
    : m_chunk_size(chunk_size)
    , m_hasher(hash::rolling_hash::create_adler32(m_chunk_size))
    , m_window_back(std::make_unique<window_back>()) {

    for (const auto &chunk : chunks) {
      m_chunks.emplace(chunk.hash(), chunk);
    }
  }

  hash_matcher::~hash_matcher() = default;

  void hash_matcher::append(const std::shared_ptr<buffer> &buf) {
    m_window_back->push_buffer(buf);

    for (std::size_t offset = 0; offset < buf->size(); ++offset) {
      auto inchar = buf->at(offset);
      if (m_current_offset - m_start_offset > m_chunk_size) {
        m_hasher->update(m_window_back->pop(), inchar);
      } else {
        m_hasher->append(inchar);
      }

      auto chunk_size = std::min(m_current_offset - m_start_offset, m_chunk_size);

      auto match_it = m_chunks.find(m_hasher->output());
      if (match_it != m_chunks.end()) {
        m_matchs.emplace_back(match_it->second, m_current_offset - chunk_size);

        m_hasher = hash::rolling_hash::create_adler32(m_chunk_size);
        m_window_back = std::make_unique<window_back>();
        m_start_offset = m_current_offset + 1;
      }

      ++m_current_offset;
    }
  }
}