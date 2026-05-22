# HW01: C Puzzle Engine

## Theme

This assignment is inspired by a CSE220-style board puzzle solver. It is the
gentle on-ramp: C arrays, structs, validation, tests, and debugging.

You will implement a small "skyscraper" puzzle engine. A board is an `N x N`
grid where each row and column must contain values `1..N` exactly once. Edge
clues describe how many buildings are visible from a direction. Taller
buildings hide shorter buildings behind them.

## Learning Goals

- Model state with C structs.
- Parse compact strings into board data.
- Validate rows and columns.
- Implement visibility checks.
- Use unit tests and GDB.
- Practice clean error handling.

## Provided

- `include/puzzle.h`: public API.
- `src/puzzle.c`: starter implementation with TODOs.
- `tests/test_puzzle.cpp`: unit tests.
- `CMakeLists.txt`: build/test configuration.

## Your Tasks

1. Implement `puzzle_visible_count`.
2. Implement row and column duplicate checks.
3. Implement `puzzle_row_satisfies_clues`.
4. Implement `puzzle_col_satisfies_clues`.
5. Implement `puzzle_place`.
6. Implement `puzzle_solve_basic`.

The basic solver does not need full backtracking. It should repeatedly apply
these rules:

- clue `1` means the nearest cell must be `N`;
- clue `N` means the row or column must be `1,2,...,N` from that side;
- if a row or column has exactly one missing value and one empty cell, fill it.

## Commands

```bash
cmake --preset hw01
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

## Debugging Requirements

Before marking this homework complete:

- debug one failing test in GDB;
- run with AddressSanitizer or Valgrind;
- add two extra tests of your own.

## Stretch

Add a backtracking solver that can solve harder boards after the basic rules
stop making progress.
