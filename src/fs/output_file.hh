#pragma once

#include <cstddef>
#include <fstream>

namespace eiger_coding_challenge {
  struct buffer;
}

namespace eiger_coding_challenge::fs {

  struct output_file final {
  public:
    explicit output_file(const std::string &filename);

    void append(const buffer *buf);
    void write(std::size_t offset, const buffer *buf);

  private:
    std::ofstream stream;
  };
}