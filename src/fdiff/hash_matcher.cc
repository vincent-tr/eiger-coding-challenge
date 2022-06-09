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

    void advance(std::size_t size) {
      // Note: last advance call will move after end, because of padding
      while (size > 0 && !m_buffers.empty()) {
        const auto &buffer = m_buffers.front();
        auto skip_size = std::min(size, buffer->size() - m_buffer_offset);

        size -= skip_size;
        m_buffer_offset += skip_size;

        if (m_buffer_offset == buffer->size()) {
          m_buffer_offset = 0;
          m_buffers.pop();
        }
      }
    }

    bool empty() {
      return m_buffers.empty();
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
      append_value(inchar);
    }
  }

  void hash_matcher::end() {
    // pad with 0 until the window has no data
    while (!m_window_back->empty()) {
      append_value(0);
    }
  }

  void hash_matcher::append_value(uint8_t inchar) {
    if (m_window_size < m_chunk_size) {
      m_hasher->append(inchar);
      ++m_window_size;
    } else {
      m_hasher->update(m_window_back->pop(), inchar);
    }

    ++m_current_offset;

    if (m_window_size < m_chunk_size) {
      // cannot match on incomplete window
      return;
    }

    auto match_it = m_chunks.find(m_hasher->output());
    if (match_it == m_chunks.end()) {
      return;
    }

    const auto &chunk_hash = match_it->second;
    m_matchs.emplace_back(chunk_hash.offset(), chunk_hash.size(), m_current_offset - m_chunk_size);

    m_hasher = hash::rolling_hash::create_adler32(m_chunk_size);
    m_window_back->advance(m_chunk_size);
    m_window_size = 0;
  }
}