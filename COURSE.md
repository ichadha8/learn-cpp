# Course Plan

This course is a compressed systems-programming track. It borrows the shape of a
C systems course, then uses modern C++ to turn the same concepts into safer,
more maintainable production-style code.

## Outcomes

By the end, learners should be able to:

- build and debug C/C++ programs in Linux;
- reason about stack, heap, object lifetime, and undefined behavior;
- use GDB, Valgrind, sanitizers, CMake, and GitHub Actions;
- write C programs using files, processes, signals, sockets, and pthreads;
- write C++ using RAII, STL containers, move semantics, and tests;
- create Python integration tooling around C/C++ systems projects;
- describe two capstones clearly in interviews.

## Weekly Structure

Each week has:

- one lesson note in `lessons/`;
- one lab-sized exercise or homework milestone;
- required debugging practice;
- a short written reflection or report.

The written reports matter. Systems work is not just code; it is the ability to
explain a failure mode, a profiling result, a race, or a design tradeoff.

## Six-Week Core

### Week 1: C Tooling and Correctness

Read:

- `lessons/01-c-linux-tooling.md`
- `lessons/02-c-memory.md`

Do:

- build with CMake;
- run unit tests;
- use GDB on a failing test;
- use Valgrind or AddressSanitizer on a memory bug.

Assignment:

- `assignments/hw01-c-puzzle`

### Week 2: C Data Modeling and Validation

Read:

- `lessons/02-c-memory.md`

Do:

- finish the puzzle engine;
- write two new tests of your own;
- document one invalid input class.

Assignment:

- finish `assignments/hw01-c-puzzle`

### Week 3: Unix Processes and I/O

Read:

- `lessons/03-unix-processes-io-signals.md`

Do:

- run child processes;
- redirect file descriptors;
- implement timeouts;
- handle process exit states correctly.

Assignment:

- `assignments/hw02-unix-runner`

### Week 4: Performance, Fuzzing, and Sanitizers

Read:

- `lessons/04-performance-memory-and-fuzzing.md`

Do:

- run a profiler or timing benchmark;
- build with sanitizers;
- mutate inputs;
- classify crash, timeout, and valid executions.

Assignment:

- `assignments/hw03-fuzzer-lite`

### Week 5: Modern C++

Read:

- `lessons/05-modern-cpp-core.md`
- `lessons/05b-cpp-warmup.md`
- `lessons/06-cpp-raii-stl-templates.md`

Do:

- complete the bonus warmup if C++ syntax feels rusty;
- write move-only resource wrappers;
- use STL containers and algorithms;
- test exception and ownership behavior.

Assignment:

- optional bridge: `assignments/bonus-cpp-warmup`
- `assignments/hw04-cpp-raii`

### Week 6: Concurrency and Networking

Read:

- `lessons/07-concurrency-networking.md`

Do:

- write a thread-safe shared state object;
- implement a protocol parser;
- run Python integration clients;
- use ThreadSanitizer where available.

Assignment:

- `assignments/hw05-concurrent-server`

## Capstone Track

After the core course, complete both capstones:

- `capstones/c-fuzzlab`
- `capstones/cpp-pulsekv`

The capstones should be done on feature branches, with a clean README, milestone
checklist, tests, and final report.

## Grading Rubric

Use this rubric for self-review or peer review:

- Correctness: 35%
- Tests and debugging evidence: 20%
- Memory/resource safety: 15%
- Design clarity: 15%
- Documentation and report: 10%
- Stretch features: 5%

## Pair Workflow

For two learners:

1. Driver writes code while navigator reads the spec and tests.
2. Switch every 30 to 45 minutes.
3. Commit at every working milestone.
4. Review each other's code before moving to the next part.
5. Keep a shared `NOTES.md` with bugs found, commands used, and lessons learned.
