# Lesson 04: Performance, Locality, Caches, Toolchains, and Fuzzing

## What This Lesson Teaches

This lesson connects C code to hardware and compiler behavior. You will learn
why two programs with the same Big-O can have very different runtime, why
compilers sometimes cannot optimize apparently obvious code, and how fuzzing
uses repeated execution to find bugs.

By the end, you should be able to:

- explain temporal and spatial locality;
- reason about row-major memory access;
- compute basic cache address fields;
- explain AMAT;
- recognize compiler optimization blockers;
- describe how a mutation fuzzer works.

## Good Code Has Multiple Dimensions

Good systems code is:

- correct;
- readable;
- testable;
- memory-safe;
- resource-safe;
- fast enough for its job.

Do not optimize random code. Measure first.

Useful tools:

```bash
time ./program
perf stat ./program
valgrind --tool=callgrind ./program
```

Some tools may be restricted inside containers, but simple timing and
instrumented counters still teach a lot.

## Big-O Is Only the First Layer

Big-O describes how work grows as input grows. It does not include:

- cache misses;
- branch mispredictions;
- memory allocation overhead;
- function-call overhead;
- synchronization overhead;
- system-call overhead.

Two O(n) programs can differ by a large constant factor.

## Locality

Locality is the reason caches work.

Temporal locality:

- recently used data is likely to be used again soon.

Spatial locality:

- nearby data is likely to be used soon.

Example with spatial locality:

```c
for (size_t i = 0; i < n; i++) {
    sum += values[i];
}
```

This walks contiguous memory.

## Row-Major C Arrays

C stores arrays row by row.

Good locality:

```c
for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
        use(matrix[row][col]);
    }
}
```

Poorer locality:

```c
for (int col = 0; col < cols; col++) {
    for (int row = 0; row < rows; row++) {
        use(matrix[row][col]);
    }
}
```

The second version jumps by an entire row on each inner-loop iteration.

## Cache Organization

A typical cache address split:

```text
tag | set index | block offset
```

Parameters:

- `B`: bytes per block;
- `S`: number of sets;
- `E`: lines per set.

If `B = 32`, then block offset uses `log2(32) = 5` bits.

Direct mapped:

- `E = 1`;
- each block has exactly one set it can occupy.

Set associative:

- `E > 1`;
- address chooses a set;
- tag can match any line in that set.

Hit requirements:

1. correct set;
2. valid bit set;
3. matching tag.

## AMAT

Average Memory Access Time:

```text
AMAT = hit time + miss rate * miss penalty
```

If hit time is 1 cycle, miss rate is 3%, and miss penalty is 100 cycles:

```text
1 + 0.03 * 100 = 4 cycles
```

Common trap: using hit rate instead of miss rate.

## Blocking

Blocking splits a large computation into chunks that fit better in cache.

For matrix multiplication, the mathematical work stays the same, but the memory
access pattern changes so data is reused while still nearby.

Blocking is a central systems lesson: performance can depend on how your data
flows through the memory hierarchy, not only on the algorithm's mathematical
complexity.

## Compiler Optimizations

Common optimization names:

- code motion: move repeated work out of a loop;
- reduction in strength: replace expensive operation with cheaper equivalent;
- common subexpression elimination: reuse repeated expression result;
- inlining: replace call with function body;
- loop unrolling: do multiple loop iterations per loop body.

## Optimization Blockers

Compilers must preserve observable behavior. They cannot optimize in ways that
change legal C program behavior.

Example:

```c
void lower(char *s) {
    for (size_t i = 0; i < strlen(s); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] += 'a' - 'A';
        }
    }
}
```

Why is this bad?

- `strlen` scans the string.
- It is called every iteration.
- The loop can become O(n^2).

Why might the compiler hesitate to move the call?

- Function calls can have side effects in general.
- Pointer writes could affect memory read by another call.

Aliasing is another blocker:

```c
void sum_rows(double *a, double *b, long n) {
    for (long i = 0; i < n; i++) {
        b[i] = 0;
        for (long j = 0; j < n; j++) {
            b[i] += a[i*n + j];
        }
    }
}
```

If `a` and `b` can alias, stores to `b[i]` might affect later loads from `a`.

## Sanitizers

Sanitizers add runtime checks:

```text
-fsanitize=address
-fsanitize=undefined
-fsanitize=thread
```

They are slower, but they turn silent bugs into loud failures. Fuzzers often run
targets with sanitizers so memory bugs become detectable crashes.

## Fuzzing

A fuzzer is a program that generates inputs for another program to find crashes,
hangs, or incorrect behavior.

Minimum mutation fuzzer:

```text
load seed inputs
repeat N times:
    choose input
    mutate input
    run target
    classify result
    save crash if target crashed
```

Coverage-guided fuzzer:

```text
keep inputs that reach new code paths
mutate those more often
```

Why coverage helps:

- random mutation alone may keep hitting the same shallow parser checks;
- coverage tells the fuzzer which inputs reached new logic.

## Mutation Examples

Useful mutations:

- flip one bit;
- increment/decrement a byte;
- duplicate input;
- truncate input;
- append boundary values;
- insert strings like `"0"`, `"-1"`, `"999999999"`, `"CRASH"`;
- splice two existing inputs.

The fuzzer itself must be robust. If the target crashes, the fuzzer should keep
running.

## Professor-Style Question Patterns

Likely questions:

- Which loop has better spatial locality?
- What is the address split for this cache?
- Is this access a hit or miss?
- Compute AMAT.
- Why is this optimization unsafe?
- What does a profiler measure?
- What is a coverage-guided fuzzer trying to preserve?

## Assignment Connection

HW03 is the first fuzzer. FuzzLab is the capstone version. Performance and
locality also show up in the puzzle solver and C++ server because data layout,
allocation count, and synchronization overhead all affect real runtime.
