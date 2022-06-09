#include <gtest/gtest.h>

#include "hash_matcher.hh"
#include "buffer.hh"
#include "chunk_hash.hh"
#include "chunk_match.hh"

namespace eiger_coding_challenge::fdiff {
  namespace {

    static constexpr int chunk_size = 4;

    // output of hash computer for "123456789"
    static const std::vector<chunk_hash> chunks{
      {33030347, 0, 4},
      {35651803, 4, 4},
      {15204410, 8, 1},
    };

    TEST(FDiffMatcher, Basic) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("123456789"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 0},
        {4, 4, 4},
        {8, 1, 8},
      }));
    }

    TEST(FDiffMatcher, UpdateMiddle) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("12345X789"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 0},
        {8, 1, 8},
      }));
    }

    TEST(FDiffMatcher, InsertFirst) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("X123456789"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 1},
        {4, 4, 5},
        {8, 1, 9},
      }));
    }

    TEST(FDiffMatcher, InsertMiddle) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("123456X789"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 0},
        {8, 1, 9},
      }));
    }

    TEST(FDiffMatcher, InsertEnd) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("123456789X"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 0},
        {4, 4, 4},
      }));
    }

    TEST(FDiffMatcher, RemoveFirst) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("23456789"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {4, 4, 3},
        {8, 1, 7},
      }));
    }

    TEST(FDiffMatcher, RemoveMiddle) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("12345789"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 0},
        {8, 1, 7},
      }));
    }

    TEST(FDiffMatcher, RemoveEnd) {
      hash_matcher matcher(4, chunks);

      matcher.append(buffer::from_string_view("12345678"));
      matcher.end();

      auto output = matcher.output();

      ASSERT_EQ(output, (std::vector<chunk_match>{
        {0, 4, 0},
        {4, 4, 4},
      }));
    }
  }
}