#include <gtest/gtest.h>

#include "patch_builder.hh"
#include "chunk_match.hh"
#include "chunk_patch.hh"

namespace eiger_coding_challenge::fdiff {
  namespace {

    TEST(FDiffPatch, Reuse) {
      std::vector<chunk_match> matchs{
        {42, 4, 0},
        {52, 4, 4},
        {62, 1, 8},
      };

      auto output = build_patch(9, matchs);

      ASSERT_EQ(output, (std::vector<chunk_patch>{
        {patch_type::reuse, 42, 4},
        {patch_type::reuse, 52, 4},
        {patch_type::reuse, 62, 1},
      }));
    }

    TEST(FDiffPatch, SyncBegin) {
      std::vector<chunk_match> matchs{
        {52, 4, 4},
        {62, 1, 8},
      };

      auto output = build_patch(9, matchs);

      ASSERT_EQ(output, (std::vector<chunk_patch>{
        {patch_type::synchronize, 0, 4},
        {patch_type::reuse, 52, 4},
        {patch_type::reuse, 62, 1},
      }));
    }

    TEST(FDiffPatch, SyncEnd) {
      std::vector<chunk_match> matchs{
        {42, 4, 0},
        {52, 4, 4},
      };

      auto output = build_patch(9, matchs);

      ASSERT_EQ(output, (std::vector<chunk_patch>{
        {patch_type::reuse, 42, 4},
        {patch_type::reuse, 52, 4},
        {patch_type::synchronize, 8, 1},
      }));
    }

    TEST(FDiffPatch, SyncHole) {
      std::vector<chunk_match> matchs{
        {42, 4, 0},
        {62, 1, 8},
      };

      auto output = build_patch(9, matchs);

      ASSERT_EQ(output, (std::vector<chunk_patch>{
        {patch_type::reuse, 42, 4},
        {patch_type::synchronize, 4, 4},
        {patch_type::reuse, 62, 1},
      }));
    }
  }
}