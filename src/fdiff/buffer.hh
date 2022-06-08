#pragma once

#include <cstddef>
#include <string_view>
#include <stdexcept>

namespace eiger_coding_challenge::fdiff {

  struct buffer : std::enable_shared_from_this<buffer> {
  protected:
    // only used from create to enforce shared_ptr good design
    explicit buffer(const uint8_t *data, std::size_t size, std::shared_ptr<const buffer> parent = nullptr)
     : m_data(data)
     , m_size(size) {
    }

  public:
    static std::shared_ptr<buffer> create(const uint8_t *data, std::size_t size, std::shared_ptr<const buffer> parent = nullptr) {
      return std::shared_ptr<buffer>(new buffer(data, size, std::move(parent)));
    }

    virtual ~buffer() = default;

    const uint8_t *data() const {
      return m_data;
    }

    std::size_t size() const {
      return m_size;
    }

    uint8_t at(std::size_t index) const {
      return m_data[index];
    }

    const uint8_t *begin() const {
      return m_data;
    }

    const uint8_t *end() const {
      return m_data + m_size;
    }

    std::shared_ptr<buffer> slice(std::size_t begin, std::size_t slice_size) const {
      if (begin + slice_size > size()) {
        throw std::range_error("slice index out of bound: begin + size > buffer size");
      }

      return create(m_data + begin, slice_size, shared_from_this());
    }

    static std::shared_ptr<buffer> from_string_view(const std::string_view &sv) {
      return create(reinterpret_cast<const uint8_t *>(sv.data()), sv.size());
    }

    static std::shared_ptr<buffer> with_copy(const uint8_t *data, std::size_t size) {

      struct allocated_buffer : public buffer {
        explicit allocated_buffer(std::unique_ptr<uint8_t[]> data, std::size_t size)
         : buffer(data.get(), size, nullptr)
         , m_data(std::move(data)) {
        }

        virtual ~allocated_buffer() = default;

      private:
        // used only for its lifetime
        std::unique_ptr<uint8_t[]> m_data;
      };

      auto allocated_data = std::make_unique<uint8_t[]>(size);
      return std::make_shared<allocated_buffer>(std::move(allocated_data), size);
    }

  private:
    const uint8_t *m_data;
    std::size_t m_size;
    std::shared_ptr<const buffer> m_parent; // used for cheap slicing
  };
}