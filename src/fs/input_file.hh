#pragma once

#include <cstddef>
#include <fstream>
#include <memory>

namespace eiger_coding_challenge {
  struct buffer;
  struct mutable_buffer;
}

namespace eiger_coding_challenge::fs {

  struct input_file final {
  public:
    explicit input_file(const std::string &filename);

    std::size_t size();
    void read(std::size_t offset, mutable_buffer *buf);
    std::shared_ptr<buffer> read(std::size_t offset, std::size_t size);

  private:
    std::ifstream stream;
  };
}