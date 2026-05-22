# Homework 3: Fuzzer-Lite

Readme version 2.0

## Introduction

In this assignment, you will build a small mutation-based fuzzer. A fuzzer is a
program that repeatedly runs another program with generated inputs, hoping to
find crashes, hangs, or other interesting behavior.

This homework is inspired by a much larger coverage-guided fuzzer assignment,
but it is deliberately scoped down. You will focus on the core shape:

```text
seed inputs -> queue -> mutation -> target runner -> result classification
```

The goal is to practice:

- safe input buffers in C;
- linked-list queue ownership;
- deterministic mutation functions;
- repeated child-process execution;
- crash artifact saving;
- Python tooling around C programs.

## Base Code Policy

The public API lives in `include/fuzzer_lite.h`. Do not change function
signatures or struct layouts. Later capstone code uses the same design ideas.

You may add helper functions in `src/fuzzer_lite.c`.

## Directory Layout

```text
hw03-fuzzer-lite/
├── CMakeLists.txt
├── README.md
├── include/
│   └── fuzzer_lite.h
├── src/
│   └── fuzzer_lite.c
└── tests/
    └── test_fuzzer_lite.cpp
```

## Build and Test

```bash
cmake --preset hw03
cmake --build --preset hw03
ctest --preset hw03 --output-on-failure
```

## Input Representation

The starter uses:

```c
typedef struct {
    unsigned char bytes[FL_MAX_INPUT];
    size_t length;
} fl_input_t;
```

This is a fixed-capacity byte buffer. It can hold text or binary data. The
`length` field is authoritative. Do not use `strlen` on `bytes` unless the
input is known to be a null-terminated string.

Why bytes instead of `char *`?

- fuzz inputs may contain `'\0'`;
- fixed capacity avoids early heap complexity;
- copying is simple for a learning project.

## Queue Ownership

The queue stores copies of `fl_input_t`.

```c
typedef struct fl_node fl_node_t;

typedef struct {
    fl_node_t *head;
    fl_node_t *tail;
    size_t size;
} fl_queue_t;
```

Invariants:

- empty queue: `head == NULL`, `tail == NULL`, `size == 0`;
- nonempty queue: `head != NULL`, `tail != NULL`, `size > 0`;
- `tail->next == NULL`;
- every node allocated by push is freed by pop or destroy.

## Required Queue Functions

### `void fl_queue_init(fl_queue_t *queue)`

Initializes an empty queue. Safe to call on a valid queue object before use.

### `void fl_queue_destroy(fl_queue_t *queue)`

Frees all nodes and returns the queue to empty state. It should safely handle
`NULL`.

### `int fl_queue_push(fl_queue_t *queue, const fl_input_t *input)`

Allocates a node and appends a copy of `input`.

Return:

- `0` on success;
- `-1` for invalid arguments or allocation failure.

### `int fl_queue_pop(fl_queue_t *queue, fl_input_t *out)`

Removes the oldest input.

Return:

- `0` on success;
- `-1` if queue is empty or arguments are invalid.

## Required Input Function

### `int fl_input_from_cstr(fl_input_t *out, const char *text)`

Creates an input from a null-terminated C string.

This is a convenience for tests and seed loading. It should reject strings
longer than `FL_MAX_INPUT`.

## Required Mutations

Mutation functions must not modify their input argument. They write a mutated
copy to `out`.

### `int fl_mutate_flip_byte(const fl_input_t *input, fl_input_t *out, unsigned int seed)`

Copies input to output, chooses an index using `seed % input->length`, and flips
one bit or byte at that index.

For empty input, return a valid empty output.

### `int fl_mutate_duplicate(const fl_input_t *input, fl_input_t *out)`

Duplicates the input bytes once:

```text
"ab" -> "abab"
```

If doubling would exceed `FL_MAX_INPUT`, truncate to `FL_MAX_INPUT`.

### `int fl_mutate_append_int(const fl_input_t *input, fl_input_t *out, int value)`

Appends the decimal string representation of `value` if there is room. Truncate
if necessary.

Examples:

```text
"x", 123 -> "x123"
"x", -1  -> "x-1"
```

## Fuzzer Loop Concept

The starter API focuses on queue and mutation, but your written implementation
plan should describe the full loop:

1. Load seed inputs into queue.
2. Pop an input.
3. Mutate it.
4. Write mutated bytes to a temporary file.
5. Run target with HW02-style runner.
6. If target crashes, save input to `crashes/`.
7. If input is interesting, push it back into queue.
8. Stop after requested iteration count.

## Interesting Inputs

This homework does not require real coverage. For a learning version, an input
can be "interesting" if it produces:

- a new exit code;
- a new signal;
- a new output-size bucket;
- a timeout;
- a crash.

FuzzLab later turns this into a more portfolio-worthy design.

## Common Bugs

- Treating fuzz input as a C string.
- Forgetting to free queue nodes.
- Losing the tail pointer after pop.
- Mutating the input instead of writing to `out`.
- Failing to handle empty input.
- Writing past `FL_MAX_INPUT`.
- Assuming random behavior is testable without a deterministic seed.

## Tests You Should Add

- push two inputs and pop in FIFO order;
- destroy nonempty queue;
- duplicate empty input;
- duplicate input near max length;
- append negative integer;
- flip byte with several seeds.

## Grading Breakdown

| Area | Weight |
|---|---:|
| Queue correctness and memory cleanup | 30% |
| Input handling | 15% |
| Mutation correctness | 30% |
| Fuzzer-loop design notes | 15% |
| Tests and style | 10% |

## Debugging Checklist

- Run Valgrind on queue tests.
- Assert queue invariants after push/pop while debugging.
- Print bytes with indexes, not just as strings.
- Test empty input explicitly.
- Test max-length input explicitly.

## Stretch

Use the HW02 runner to fuzz a deliberately fragile target program and save
crashing inputs to disk.
