# Lesson 04: Performance, Memory, and Fuzzing

## Performance Mindset

Performance work starts with measurement. Guessing is allowed only long enough
to form a hypothesis.

Measure:

- wall-clock time;
- CPU time;
- allocation count;
- cache behavior;
- throughput;
- tail latency for servers.

Useful tools:

```bash
time ./program
perf stat ./program
valgrind --tool=callgrind ./program
```

In Codespaces, some low-level profiler features may be limited. Timing and
instrumented benchmarks are still useful.

## Big-O Is Not Enough

Asymptotic complexity tells you how an algorithm scales. It does not tell you
whether you are fighting the cache, allocating too much, branching badly, or
calling `strlen` inside a loop.

Common optimization blockers:

- repeated work in inner loops;
- unpredictable branches;
- pointer aliasing;
- poor locality;
- unnecessary allocation;
- excessive synchronization.

## Locality

Modern CPUs are much faster than main memory. Cache-friendly code uses nearby
data soon after related data.

Row-major traversal is usually better for C arrays:

```c
for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
        use(matrix[row][col]);
    }
}
```

For matrix operations, loop order can dominate runtime even when Big-O is the
same.

## Sanitizers

Sanitizers intentionally add runtime checks:

```bash
-fsanitize=address
-fsanitize=undefined
-fsanitize=thread
```

Use them early. They turn silent memory and concurrency bugs into loud failures.

## Fuzzing

A fuzzer repeatedly runs a target program with generated inputs, looking for
crashes, hangs, or interesting behavior.

A minimal fuzzer needs:

- a seed corpus;
- a mutation engine;
- a runner;
- crash and timeout detection;
- a queue of inputs worth mutating again.

Coverage-guided fuzzers go further: they keep inputs that reach new control-flow
paths. The capstone introduces this idea in stages.

## Mutation Examples

Simple mutations:

- duplicate input;
- truncate input;
- flip a byte;
- insert an integer string;
- replace one byte with boundary values like `0`, `255`, `'\n'`;
- append long repeated data.

Good fuzzing code is boring and robust. The fuzzer itself should not crash when
the target crashes.

## Python's Role

Python is excellent for:

- generating seed corpora;
- launching integration tests;
- grouping crash files by signal or stack trace;
- producing HTML or JSON reports;
- plotting benchmark results.

Keep the hot loop in C when the assignment asks for C. Use Python around it.

## Practice

Take a small parser and:

1. write five seed inputs;
2. mutate each input ten ways;
3. run the parser as a child process;
4. classify normal exit, crash, and timeout;
5. save crashing inputs to `crashes/`.

## Interview Angle

You should be able to answer:

- Why can two O(n) programs have very different runtimes?
- What is spatial locality?
- What does AddressSanitizer catch?
- How does a mutation fuzzer find crashes?
