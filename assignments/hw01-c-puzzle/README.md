# Homework 1: C Puzzle Engine

Readme version 2.0

## Introduction

In this assignment, you will implement the core logic for a small C puzzle
engine. The puzzle is based on the "skyscrapers" constraint puzzle: an `N x N`
board must be filled with the numbers `1..N` so that every row and column
contains each number exactly once. Edge clues tell you how many buildings are
visible from a side. Taller buildings hide shorter buildings behind them.

The goal of this assignment is not to build a world-class puzzle solver. The
goal is to practice the C fundamentals that every later systems assignment will
depend on:

- arrays and row/column indexing;
- pointer parameters;
- structs as state containers;
- validation and error returns;
- exact unit-testable behavior;
- debugging with GDB.

This is intentionally the first assignment because it has no processes, files,
sockets, threads, or heap allocator complexity. If this feels shaky, slow down
here. It is much cheaper to fix C fundamentals before they are mixed with OS
behavior.

## Base Code Policy

The public interface is declared in `include/puzzle.h`. Treat this header as a
contract. You may read it as much as you want, but do not change function names,
argument types, return types, constants, or struct layouts unless you are also
updating all tests and course material.

You may add helper functions to `src/puzzle.c`. Helper functions should be
`static` unless another file truly needs them.

The starter implementation intentionally leaves most logic blank. This is by
design. You should learn by implementing the behavior described here, not by
reading a completed solution.

## Directory Layout

```text
hw01-c-puzzle/
├── CMakeLists.txt
├── README.md
├── include/
│   └── puzzle.h
├── src/
│   └── puzzle.c
└── tests/
    └── test_puzzle.cpp
```

The tests are written in C++ but call your C functions through `extern "C"` via
the header guard in `puzzle.h`. This is a common industry pattern: C libraries
can be tested from C++ test harnesses.

## Building and Running

From the repository root:

```bash
cmake --preset hw01
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

The provided tests are not exhaustive. They check basic behavior and give you a
starting point. You should add more tests as you implement edge cases.

## Puzzle Model

The board stores integers:

- `0` means empty;
- `1..N` are puzzle values.

For a size-4 board, valid row contents are some permutation of:

```text
1 2 3 4
```

Invalid row examples:

```text
1 1 2 3   duplicate 1
1 2 3 5   value outside range
1 2 3 0   incomplete row, not necessarily invalid yet
```

## Edge Clues

Imagine each number as a building height. Looking across a row, you see a
building if it is taller than every building before it.

Example:

```text
row: 2 1 4 3
```

Looking from the left:

- see `2`;
- `1` is hidden by `2`;
- see `4`;
- `3` is hidden by `4`.

Visible count is `2`.

Looking from the right:

- see `3`;
- see `4`;
- `1` and `2` are hidden by `4`.

Visible count is `2`.

Clue `0` means "no clue for this side."

## Input Encoding

`puzzle_load` receives two compact strings:

```c
int puzzle_load(puzzle_t *puzzle, const char *state, const char *clues);
```

`state` has `size * size` characters in row-major order. `'-'` and `'0'` mean
empty. Digits `'1'` through `'8'` mean filled cells.

For a 4x4 board:

```text
"1--4----3-------"
```

means:

```text
1 - - 4
- - - -
3 - - -
- - - -
```

`clues` has `4 * size` characters in this order:

```text
top clues, bottom clues, left clues, right clues
```

For size 4:

```text
top[0..3] bottom[0..3] left[0..3] right[0..3]
```

## Required Functions

### `int puzzle_init(puzzle_t *puzzle, int size)`

Initializes a puzzle object.

Return:

- `0` on success;
- `-1` if `puzzle == NULL` or size is outside `1..PUZZLE_MAX_SIZE`.

This function is already implemented in the starter. Read it to understand the
initial state.

### `int puzzle_load(puzzle_t *puzzle, const char *state, const char *clues)`

Loads board cells and edge clues from compact strings.

This function is partially implemented. You should understand it, but the
primary required work is in the validation and solving helpers below.

### `int puzzle_visible_count(const int *values, int count)`

Counts visible buildings in a sequence.

Required behavior:

- return `-1` for `NULL` input or negative count;
- return `0` for empty count;
- otherwise scan left to right and count values that exceed the maximum seen so
  far.

Example:

```text
values = 1 3 2 4
visible = 3
```

### `int puzzle_has_duplicate_in_row(const puzzle_t *puzzle, int row)`

Checks one row for duplicate non-empty values.

Return:

- `1` if a duplicate exists;
- `0` if no duplicate exists;
- `-1` for invalid arguments.

Empty cells do not count as duplicates.

### `int puzzle_has_duplicate_in_col(const puzzle_t *puzzle, int col)`

Same as row duplicate check, but for a column.

### `int puzzle_row_satisfies_clues(const puzzle_t *puzzle, int row)`

Checks whether a completed row satisfies its left and right clues.

Return:

- `1` if the row is complete and satisfies all nonzero clues;
- `0` if the row is complete but violates a clue;
- `-1` for invalid arguments or incomplete row.

This function should not silently accept incomplete rows. Treat incomplete rows
as not ready for final clue validation.

### `int puzzle_col_satisfies_clues(const puzzle_t *puzzle, int col)`

Same as row clue validation, but for top and bottom clues.

### `int puzzle_place(puzzle_t *puzzle, int row, int col, int value)`

Attempts to place a value.

Required behavior:

1. Validate arguments.
2. Reject placement into an occupied cell.
3. Temporarily place the value.
4. Reject if row or column duplicates appear.
5. If the row or column becomes complete, reject clue violations.
6. Keep the value only if all checks pass.

Return:

- `0` if placement is accepted;
- `-1` if placement is rejected.

Important: if rejected, the board must be restored to its previous state.

### `int puzzle_solve_basic(puzzle_t *puzzle)`

Applies simple deterministic solving rules.

Required minimum rules:

- clue `1` means the nearest cell must be `N`;
- clue `N` means the row/column must increase from that side;
- if a row has one empty cell and one missing value, fill it;
- if a column has one empty cell and one missing value, fill it;
- repeat until no more progress.

Return:

- `1` if the board is completely solved;
- `0` if no more basic progress is possible;
- `-1` for invalid input.

Do not implement backtracking for the required version. Backtracking is a
stretch feature.

## Suggested Implementation Order

1. Implement `puzzle_visible_count`.
2. Implement duplicate checks.
3. Implement row/column clue validation.
4. Implement `puzzle_place`.
5. Add your own tests for invalid placements.
6. Implement basic solver rules one at a time.
7. Only then consider stretch backtracking.

## Common Bugs

- Treating empty cells as duplicates.
- Reversing row and column indexes.
- Forgetting to restore a rejected move.
- Accepting values outside `1..N`.
- Validating clues before a row/column is complete.
- Using `sizeof` on a pointer and expecting array length.

## Testing Expectations

Your final solution should include tests for:

- visible count;
- duplicate row;
- duplicate column;
- invalid value;
- placement into occupied cell;
- rejected placement restores board;
- clue validation from both directions;
- basic solver progress.

## Grading Breakdown

| Area | Weight |
|---|---:|
| Loading and validation | 20% |
| Visible-count and clue logic | 25% |
| Placement correctness | 25% |
| Basic solver | 20% |
| Tests, style, and memory safety | 10% |

## Debugging Checklist

Before asking for help:

1. Run tests with `--output-on-failure`.
2. Print the board to `stderr`, not stdout.
3. Put a GDB breakpoint on `puzzle_place`.
4. Check row/column order.
5. Check that failed placements restore old state.

## Stretch

Implement a recursive backtracking solver:

1. Find the first empty cell.
2. Try each value `1..N`.
3. Use `puzzle_place`.
4. Recurse.
5. Restore state if recursion fails.

Backtracking should reuse your validation logic. If you duplicate validation in
the solver, your code will become harder to trust.
