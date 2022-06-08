# Eiger coding challenge

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