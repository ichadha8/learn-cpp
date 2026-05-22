# HW03: Fuzzer-Lite

## Theme

This assignment is inspired by the coverage-guided fuzzer homework, but it is
smaller. You will build a mutation-based fuzzer that reuses the process runner
idea from HW02.

## Learning Goals

- Represent inputs safely in C.
- Implement a queue.
- Write deterministic mutation functions.
- Run target programs repeatedly.
- Save crashing inputs.
- Use Python for integration checks.

## Provided

- `include/fuzzer_lite.h`
- `src/fuzzer_lite.c`
- `tests/test_fuzzer_lite.cpp`

## Your Tasks

1. Implement the input queue.
2. Implement at least three mutations:
   - flip byte;
   - duplicate input;
   - append integer string.
3. Implement seed-file loading.
4. Implement crash saving.
5. Wire the fuzzer loop to a target runner.

## Commands

```bash
cmake --preset hw03
cmake --build --preset hw03
ctest --preset hw03 --output-on-failure
```

## Stretch

Track "interesting" inputs by hashing their output length, exit code, or signal.
This is not real coverage, but it teaches the priority-queue shape before the
C capstone adds richer instrumentation.
