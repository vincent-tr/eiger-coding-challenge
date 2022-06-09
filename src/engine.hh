#pragma once

#include <string>
#include <cstddef>

namespace eiger_coding_challenge::fs {
  struct input_file;
  struct output_file;
}

namespace eiger_coding_challenge {

  struct engine final {
    explicit engine(std::size_t chunk_size = 512, std::size_t io_buffer_size = 4096);

    void signature(const std::string old_filename, const std::string signature_filename) const;
    void delta(const std::string signature_filename, const std::string new_filename, const std::string delta_filename) const;
    void patch(const std::string basis_filename, const std::string delta_filename, const std::string new_filename) const;

  private:
    void copy(fs::input_file &source, fs::output_file &target, std::size_t source_offset, std::size_t target_offset, std::size_t size) const;

    std::size_t m_chunk_size;
    std::size_t m_io_buffer_size;
  };
}