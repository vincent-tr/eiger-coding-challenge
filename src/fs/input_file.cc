#include "buffer.hh"
#include "input_file.hh"

namespace eiger_coding_challenge::fs {

  input_file::input_file(const std::string &filename) {
    stream.exceptions(~std::ifstream::goodbit);
    stream.open(filename, std::ios_base::in | std::ios_base::binary);
  }

  std::size_t input_file::size() {
    stream.seekg(0, std::ios_base::end);
    return stream.tellg();
  }

  std::shared_ptr<buffer> input_file::read(std::size_t offset, std::size_t size) {
    auto buf = mutable_buffer::allocate(size);

    stream.seekg(offset);
    stream.read(reinterpret_cast<char *>(buf->mutable_data()), size);

    return buf;
  }
}