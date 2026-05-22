# HW04: C++ RAII Systems Library

## Theme

This assignment translates C systems knowledge into modern C++. You will build
small RAII wrappers around Unix resources and use move semantics correctly.

## Learning Goals

- Express ownership with types.
- Delete copy operations for unique resources.
- Implement move constructors and move assignment.
- Wrap POSIX file descriptors safely.
- Use `std::vector`, `std::string`, and `std::optional`.

## Provided

- `include/raii.hpp`
- `src/raii.cpp`
- `tests/test_raii.cpp`

## Your Tasks

1. Finish `UniqueFd`.
2. Finish `OwnedBuffer`.
3. Implement `read_file`.
4. Implement `write_file`.
5. Implement `run_command_capture_status`.

## Commands

```bash
cmake --preset hw04
cmake --build --preset hw04
ctest --preset hw04 --output-on-failure
```

## Design Requirements

- `UniqueFd` must be move-only.
- `UniqueFd` must close the descriptor in its destructor.
- `release()` transfers the raw descriptor to the caller.
- `reset()` closes any previous descriptor before taking a new one.
- No function should call `exit`.

## Stretch

Add a `Pipe` type with `read_end` and `write_end` as `UniqueFd` members.
