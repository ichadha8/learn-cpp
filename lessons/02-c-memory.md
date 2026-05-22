# Lesson 02: C Memory

## The Four Regions You Must Know

Most C programs use memory from four conceptual regions:

- text: executable code;
- data/BSS: globals and static variables;
- stack: function calls and local variables;
- heap: dynamically allocated memory.

The stack is automatic. The heap is manual. Globals live for the lifetime of the
process. Bugs often come from confusing these lifetimes.

## Pointers

A pointer is a variable that stores an address. LearnCpp describes modern C++
raw pointers as distinct from smart pointers, but the C model is the raw model:
you can store an address, dereference it, pass it around, and accidentally do
something very wrong.

Rules:

- initialize pointers;
- know whether a pointer owns the memory;
- know the length of the object or array being pointed at;
- use `const` for read-only access;
- set pointers to `NULL` after freeing only when it helps avoid reuse.

Bad:

```c
int *p;
printf("%d\n", *p);
```

Better:

```c
int value = 42;
int *p = &value;
printf("%d\n", *p);
```

## Arrays and Decay

An array is contiguous storage. In most expressions, an array decays to a
pointer to its first element.

This matters because the callee usually cannot know the array length:

```c
int sum(const int *values, size_t count);
```

Pass the pointer and the length together. If you split them apart, you create a
future bug.

## Structs and Ownership

Use structs to group data that changes together:

```c
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} buffer_t;
```

Then write explicit lifetime functions:

```c
int buffer_init(buffer_t *buffer);
void buffer_free(buffer_t *buffer);
```

This is C's manual version of RAII. It is not automatic, but it gives you a
clear contract.

## Allocation

Common allocation pattern:

```c
int *values = calloc(count, sizeof *values);
if (values == NULL) {
    return -1;
}

/* use values */

free(values);
```

Prefer `sizeof *ptr` over repeating the type. It survives type changes.

## Undefined Behavior

Undefined behavior means the language standard stops promising anything.

Examples:

- signed integer overflow;
- out-of-bounds array access;
- use after free;
- reading uninitialized automatic variables;
- violating strict aliasing;
- returning the address of a local variable.

Undefined behavior can pass tests today and fail tomorrow.

## Defensive C API Design

For assignment APIs:

- return `0` on success and `-1` on error;
- validate `NULL` pointers;
- validate lengths and enum ranges;
- document ownership;
- keep allocation and cleanup symmetric.

Example:

```c
int puzzle_init(puzzle_t *puzzle, int size);
void puzzle_destroy(puzzle_t *puzzle);
```

## Practice

Implement a dynamic array in C:

- `init`
- `push`
- `get`
- `free`

Then intentionally break it and use AddressSanitizer to catch the bug.

## Interview Angle

You should be able to answer:

- What is the difference between stack and heap lifetime?
- Why does `sizeof(pointer)` not give array length?
- What does ownership mean in C?
- What is undefined behavior?
