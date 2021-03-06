#pragma once

#include <cstddef>
#include <string_view>
#include <stdexcept>
#include <memory>
#include <vector>

namespace eiger_coding_challenge {

  struct buffer {
  public:
    explicit buffer(const uint8_t *data, std::size_t size)
     : m_data(data)
     , m_size(size) {
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

    static std::shared_ptr<buffer> from_string_view(const std::string_view &sv) {
      return std::make_shared<buffer>(reinterpret_cast<const uint8_t *>(sv.data()), sv.size());
    }

    /**
     * Build a buffer from the vector's data.
     * Mutations in the vector's items will then mutate buffer's data
     * Note: the vector need to have proper size before calling this method, and the buffer's data will be invalid after vector's reallocation
     * Note: use only with T primitive or POD
     */
    template<typename T>
    static std::shared_ptr<buffer> from_vector(const std::vector<T> &vec) {
      return std::make_shared<buffer>(reinterpret_cast<const uint8_t *>(vec.data()), vec.size() * sizeof(T));
    }

    template<typename T>
    static std::shared_ptr<buffer> from_value(const T &value) {
      return std::make_shared<buffer>(reinterpret_cast<const uint8_t *>(&value), sizeof(T));
    }

  private:
    const uint8_t *m_data;
    std::size_t m_size;
  };

  struct mutable_buffer : public buffer {
  public:
    explicit mutable_buffer(uint8_t *data, std::size_t size)
     : buffer(data, size)
     , m_data(data) {
    }

    virtual ~mutable_buffer() = default;

    uint8_t *mutable_data() {
      return m_data;
    }

    uint8_t *begin() {
      return m_data;
    }

    uint8_t *end() {
      return m_data + size();
    }

    const uint8_t *cbegin() const {
      return m_data;
    }

    const uint8_t *cend() const {
      return m_data + size();
    }

    /**
     * Build a mutable buffer from the vector's data.
     * Mutations in the buffer will then mutate vector's items data, and vice-versa
     * Note: the vector need to have proper size before calling this method, and the buffer's data will be invalid after vector's reallocation
     * Note: use only with T primitive or POD
     */
    template<typename T>
    static std::shared_ptr<mutable_buffer> from_vector(std::vector<T> &vec) {
      return std::make_shared<mutable_buffer>(reinterpret_cast<uint8_t *>(vec.data()), vec.size() * sizeof(T));
    }

    template<typename T>
    static std::shared_ptr<mutable_buffer> from_value(T &value) {
      return std::make_shared<mutable_buffer>(reinterpret_cast<uint8_t *>(&value), sizeof(T));
    }

    static std::shared_ptr<mutable_buffer> allocate(std::size_t size) {

      struct allocated_buffer : public mutable_buffer {
        explicit allocated_buffer(std::unique_ptr<uint8_t[]> data, std::size_t size)
         : mutable_buffer(data.get(), size)
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
    uint8_t *m_data;
  };
}