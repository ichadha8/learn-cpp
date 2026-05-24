# Lesson 02: C Memory, Pointers, Arrays, and Undefined Behavior

## What This Lesson Teaches

C is a language where values and addresses are visible. You need to understand
where objects live, how long they live, who owns them, and what counts as
undefined behavior.

By the end, you should be able to:

- distinguish stack, heap, global/static, and text memory;
- trace pointer snippets with confidence;
- explain array decay and pointer arithmetic;
- design a C struct with explicit init/free functions;
- identify aliasing and lifetime bugs;
- recognize when a C program has undefined behavior.

## The Four Regions

A useful programmer model of a process has these regions:

```text
high addresses
  stack        local variables, function frames
  ...
  heap         malloc/calloc/realloc storage
  data/BSS     globals and static variables
  text         executable code
low addresses
```

This is a model, not a promise about exact addresses. The important part is
lifetime.

### Stack

Stack memory is automatic. Function calls create stack frames. Returning from a
function destroys that frame.

Bad:

```c
int *make_number(void) {
    int value = 42;
    return &value;
}
```

The returned pointer points to a local object that no longer exists. This is a
dangling pointer.

### Heap

Heap memory is explicit:

```c
int *values = calloc(count, sizeof *values);
if (values == NULL) {
    return -1;
}

/* use values */

free(values);
```

The caller must know who owns the allocation and who frees it.

### Globals and Statics

Globals and `static` variables live for the lifetime of the process. They are
easy to access and easy to misuse. In threaded programs they become shared
state, which means races are possible.

### Text

The text segment contains executable instructions. You normally do not write to
it.

## Pointer Vocabulary

Given:

```c
int x = 5;
int *p = &x;
```

Meanings:

- `x`: the integer object.
- `&x`: address of `x`.
- `p`: pointer variable containing the address of `x`.
- `*p`: the integer object at the address stored in `p`.

After:

```c
*p = 9;
```

`x` is `9`.

After:

```c
p = NULL;
```

`p` no longer points to `x`. The value of `x` did not change.

## Pointer Arithmetic

Pointer arithmetic scales by the pointed-to type.

If `p` is an `int *`, then:

```c
p + 1
```

means "the next `int`," not "the next byte."

For raw byte arithmetic, use `unsigned char *` or `char *`.

## Arrays and Decay

An array is storage. In most expressions, the array name decays into a pointer
to the first element.

```c
void print_values(const int *values, size_t count);
```

The function receives a pointer, not the whole array. It cannot discover the
length by calling `sizeof(values)`. You must pass the length.

Common bug:

```c
void f(int values[]) {
    size_t n = sizeof(values) / sizeof(values[0]);
}
```

Inside a function parameter list, `int values[]` is adjusted to `int *values`.
The `sizeof` expression gives pointer size, not array length.

## Row-Major Arrays

C stores multi-dimensional arrays in row-major order:

```c
int grid[ROWS][COLS];
```

The memory order is:

```text
grid[0][0], grid[0][1], ..., grid[0][COLS-1],
grid[1][0], grid[1][1], ...
```

This matters for both correctness and performance. A flattened board usually
uses:

```c
cells[row * width + col]
```

or, for `(x, y)` coordinate style:

```c
cells[y * width + x]
```

## Structs and Manual RAII

C does not have constructors and destructors, so you create explicit lifetime
functions.

```c
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} buffer_t;

int buffer_init(buffer_t *buffer);
void buffer_destroy(buffer_t *buffer);
```

Rules:

- `init` establishes invariants.
- `destroy` releases resources.
- functions validate `NULL` where appropriate.
- ownership is documented.

This is the C version of the RAII idea you will later use in C++.

## Aliasing

Aliasing means two expressions may refer to the same memory.

```c
void write_two(int *a, int *b) {
    *a = 1;
    *b = 2;
}
```

If the caller passes:

```c
int x = 0;
write_two(&x, &x);
```

then `a` and `b` alias. This affects both reasoning and optimization. The
compiler cannot freely reorder loads and stores if two pointers might refer to
the same object.

## Undefined Behavior

Undefined behavior means the C standard gives no required behavior. It does
not mean "the program will crash." It means the compiler and runtime owe you no
predictable result.

Examples:

- dereferencing `NULL`;
- reading past the end of an array;
- writing past the end of an array;
- using memory after `free`;
- double-freeing;
- signed integer overflow;
- reading uninitialized automatic variables;
- returning a pointer to a stack local.

Exam-style rule: if a snippet contains undefined behavior, do not spend ten
minutes computing a deterministic output. The correct answer may be "undefined."

## Common Traps

- Confusing the pointer value with the pointed-to value.
- Forgetting to pass array lengths.
- Allocating `sizeof(pointer)` instead of `sizeof *pointer * count`.
- Assuming `malloc` initializes memory. It does not.
- Assuming `calloc` means the object is logically initialized. It only zeroes
  bytes.
- Returning addresses of locals.
- Freeing memory but continuing to use a saved pointer.

## Practice

1. Draw a table for this snippet:

   ```c
   int x = 1;
   int y = 2;
   int *p = &x;
   int *q = p;
   *q = 5;
   p = &y;
   *p = 7;
   ```

   What are `x` and `y`?

2. Why is this allocation suspicious?

   ```c
   int *values = malloc(sizeof(values) * count);
   ```

3. Why is this function wrong?

   ```c
   char *name(void) {
       char buf[32] = "alice";
       return buf;
   }
   ```

Answers:

1. `x` is `5`, `y` is `7`.
2. `sizeof(values)` is the size of the pointer, not the size of an `int`.
3. It returns a pointer to a dead stack array.

## Assignment Connection

HW01 uses fixed arrays and validation. HW03 and the C capstone use heap-managed
input buffers. HW05 introduces shared state. Every one of those projects becomes
dangerous if you lose track of pointer lifetime, ownership, and array bounds.
