#pragma once

#include <cstddef>
#include <fstream>

namespace eiger_coding_challenge {
  struct buffer;
}

namespace eiger_coding_challenge::fs {

  struct input_file final {
  public:
    explicit input_file(const std::string &filename);

    std::size_t size();
    std::shared_ptr<buffer> read(std::size_t offset, std::size_t size);

  private:
    std::ifstream stream;
  };
}