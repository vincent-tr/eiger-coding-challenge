#include "engine.hh"
#include "fs/input_file.hh"
#include "fs/output_file.hh"
#include "fdiff/hash_computer.hh"
#include "fdiff/hash_matcher.hh"
#include "fdiff/patch_builder.hh"
#include "fdiff/chunk_hash.hh"
#include "fdiff/chunk_patch.hh"
#include "buffer.hh"

namespace eiger_coding_challenge {

  static void write_signature(const std::string &filename, const std::vector<fdiff::chunk_hash> &data);
  static std::vector<fdiff::chunk_hash> read_signature(const std::string &filename);

  engine::engine(std::size_t chunk_size, std::size_t io_buffer_size)
   : m_chunk_size(chunk_size)
   , m_io_buffer_size(io_buffer_size) {
  }

  void engine::signature(const std::string old_filename, const std::string signature_filename) const {
    fdiff::hash_computer computer(m_chunk_size);

    fs::input_file old_file(old_filename);
    auto size = old_file.size();

    std::size_t offset = 0;
    while (offset < size) {
      auto read_size = std::min(size - offset, m_io_buffer_size);
      auto buffer = old_file.read(offset, read_size);
      offset += read_size;

      computer.append(buffer);
    }

    computer.end();

    write_signature(signature_filename, computer.output());
  }

  void engine::delta(const std::string signature_filename, const std::string new_filename, const std::string delta_filename) const {
    auto signature_data = read_signature(signature_filename);
    fdiff::hash_matcher matcher(m_chunk_size, signature_data);

    fs::input_file new_file(new_filename);
    auto size = new_file.size();

    std::size_t offset = 0;
    while (offset < size) {
      auto read_size = std::min(size - offset, m_io_buffer_size);
      auto buffer = new_file.read(offset, read_size);
      offset += read_size;

      matcher.append(buffer);
    }

    matcher.end();

    auto patch_data = build_patch(size, matcher.output());
    const std::size_t count = patch_data.size();

    // patch file layout:
    // - std::size_t chunk_patch item count
    // - linear list of chunk_patch items
    // - chunks that are needed to update the old file to new new one

    // compute offsets in patch file
    auto count_buffer = buffer::from_value(count);
    auto patch_data_buffer = buffer::from_vector(patch_data);

    std::size_t delta_offset = count_buffer->size() + patch_data_buffer->size();
    fs::output_file delta_file(delta_filename);

    for (auto &item : patch_data) {
      if (item.type() == fdiff::patch_type::reuse) {
        continue;
      }

      copy(new_file, delta_file, item.source_offset(), delta_offset, item.size());
      item.update_source_offset(delta_offset);
      delta_offset += item.size();
    }

    // write headers
    delta_file.write(0, count_buffer.get());
    delta_file.write(count_buffer->size(), patch_data_buffer.get());
  }

  void engine::patch(const std::string basis_filename, const std::string delta_filename, const std::string new_filename) const {
    // TODO
  }

  void engine::copy(fs::input_file &source, fs::output_file &target, std::size_t source_offset, std::size_t target_offset, std::size_t size) const {
    const std::size_t source_end = source_offset + size;

    while (source_offset < source_end) {
      auto read_size = std::min(source_end - source_offset, m_io_buffer_size);
      auto buf = source.read(source_offset, read_size);
      source_offset += read_size;

      target.write(target_offset, buf.get());
      target_offset += read_size;
    }
  }

  void write_signature(const std::string &filename, const std::vector<fdiff::chunk_hash> &data) {
    // write directly vector data to file
    fs::output_file file(filename);
    auto buf = buffer::from_vector(data);
    file.append(buf.get());
  }

  std::vector<fdiff::chunk_hash> read_signature(const std::string &filename) {
    // read data directly into vector
    fs::input_file file(filename);
    auto item_count = file.size() / sizeof(fdiff::chunk_hash);

    std::vector<fdiff::chunk_hash> data(item_count, {0, 0, 0});
    auto buf = mutable_buffer::from_vector(data);
    file.read(0, buf.get());

    return data;
  }

}