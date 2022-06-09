#include "buffer.hh"
#include "output_file.hh"

namespace eiger_coding_challenge::fs {

  output_file::output_file(const std::string &filename) {
    stream.exceptions(~std::ofstream::goodbit);
    stream.open(filename, std::ios_base::out | std::ios_base::binary);
  }

  void output_file::append(const buffer *buf) {
    stream.seekp(0, std::ios_base::end);
    stream.write(reinterpret_cast<const char *>(buf->data()), buf->size());
  }
}