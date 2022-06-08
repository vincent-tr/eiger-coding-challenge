#pragma once

#include <cstddef>
#include <string_view>

namespace eiger_coding_challenge {

  template<typename T>
  struct buffer_iterable;

  struct buffer {
    explicit buffer(const void *data, std::size_t size)
     : m_data(data)
     , m_size(size) {
    }

    virtual ~buffer() = default;

    const void *data() const {
      return m_data;
    }

    std::size_t size() const {
      return m_size;
    }

    template<typename T>
    buffer_iterable<T> as_iterable() const;

    static buffer from_string_view(const std::string_view &sv) {
      return buffer(sv.data(), sv.size());
    }

  private:
    const void *m_data;
    std::size_t m_size;
  };

  template<typename T>
  struct buffer_iterable final {
    explicit buffer_iterable(const buffer &target)
     : m_target(target) {
    }

    const T *begin() const {
      return reinterpret_cast<const T *>(m_target.data());
    }

    const T *end() const {
      // round down to the nearest item to be sure to not read outside of the buffer.
      // Note: need to handle that manually by the reader
      return begin() + m_target.size() / sizeof (T);
    }

  private:
    const buffer &m_target;
  };

  template<typename T>
  inline buffer_iterable<T> buffer::as_iterable() const {
    return buffer_iterable<T>(*this);
  }
}