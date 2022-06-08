brew install gtest

cmake -S . -B build

cmake --build build

ctest --test-dir build
