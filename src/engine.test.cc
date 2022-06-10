#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>
#include <cstring>
#include "engine.hh"
#include "buffer.hh"
#include "fs/input_file.hh"

namespace eiger_coding_challenge::fdiff {
  namespace {

    static void run_test(const std::string &old_file, const std::string &new_file);

    TEST(Engine, Append) {
      run_test("../test-files/base.log", "../test-files/append.log");
    }

    TEST(Engine, Change) {
      run_test("../test-files/base.log", "../test-files/change.log");
    }

    TEST(Engine, Delete) {
      run_test("../test-files/base.log", "../test-files/delete.log");
    }

    TEST(Engine, InsertBegining) {
      run_test("../test-files/base.log", "../test-files/insert-begining.log");
    }

    struct temp_filename final {
      explicit temp_filename(const std::string &suffix)
       : m_path(std::filesystem::temp_directory_path() / ("eiger-coding-challenge-engine-test-" + suffix)) {
      }

      ~temp_filename() {
        if (std::filesystem::exists(m_path)) {
          std::filesystem::remove(m_path);
        }
      }

      const std::string &path() const {
        return m_path;
      }

    private:
      std::string m_path;
    };

    static bool files_equal(const std::string &filename1, const std::string &filename2) {
      constexpr std::size_t block_size = 4096;

      fs::input_file file1(filename1);
      fs::input_file file2(filename2);

      auto size = file1.size();
      if (size != file2.size()) {
        return false;
      }

      std::size_t offset = 0;

      while (offset < size) {
        auto read_size = std::min(size - offset, block_size);
        auto buf1 = file1.read(offset, read_size);
        auto buf2 = file2.read(offset, read_size);
        offset += read_size;

        if (std::memcmp(buf1->data(), buf2->data(), read_size)) {
          return false;
        }
      }

      return true;
    }

    void run_test(const std::string &old_file, const std::string &new_file) {
      engine e;

      temp_filename signature("signature");
      temp_filename delta("delta");
      temp_filename patched_file("patched-file");

      e.signature(old_file, signature.path());
      e.delta(signature.path(), new_file, delta.path());
      e.patch(old_file, delta.path(), patched_file.path());

      ASSERT_TRUE(files_equal(new_file, patched_file.path()));

      // print file sizes in the test debug output as a hint
      // run the tests with : "ctest --test-dir build --verbose -R Engine" to get it
      auto old_file_size = fs::input_file(old_file).size();
      auto new_file_size = fs::input_file(new_file).size();
      auto signature_file_size = fs::input_file(signature.path()).size();
      auto delta_file_size = fs::input_file(delta.path()).size();
      auto ratio = static_cast<float>(delta_file_size) / static_cast<float>(new_file_size);
      
      std::cout << "Test file sizes:" << std::endl;
      std::cout << "Old file is " << old_file_size << " bytes" << std::endl;
      std::cout << "New file is " << new_file_size << " bytes" << std::endl;
      std::cout << "Signature file is " << signature_file_size << " bytes" << std::endl;
      std::cout << "Delta file is " << delta_file_size << " bytes" << std::endl;
      std::cout << "Delta file represents " << (ratio * 100) << "% of new file" << std::endl;
    }
  }
}
