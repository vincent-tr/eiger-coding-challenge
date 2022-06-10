# Eiger coding challenge

[Statement](statement.md)

## System requirements

gtest, cmake

### OSX - brew

```bash
brew install googletest cmake
```

## Building

```bash
cmake -S . -B build
cmake --build build
```

## Testing

```bash
ctest --test-dir build
```

## Implementation notes

### `hash`

Contains hash algorithm implementation.

I created an "interface" (What I call an interface in C++ is a class with no variable and all its methods declared pure virtual) and a factory because I wanted to easily use different hash algorithms. Note that we can also use a template contract for this, and carry the algorithm as a template parameter everywhere else. This also can be a solution if the computed hash type can has several types (for now only `uint32_t`).

### `fdiff`

Contains the file diffing implementation.
- `hash_computer` computes the signature of a file
- `hash_matcher` tries to find unchanged blocks in the new file, given the signature
- `patch_builder` creates patch data from the matches computed above

### `fs`

Contains files IO operations, based on `std::ifstream`/`std::ofstream` (Note: could also use `boost` for this)

### `engine`

High level API to glue components together