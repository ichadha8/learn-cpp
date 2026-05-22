# Lesson 00: Course Map

## Why This Course Exists

The goal is not to memorize C and C++ syntax. The goal is to learn how real
systems behave when programs meet memory, files, processes, network sockets,
threads, compiler toolchains, and failure.

This course has three layers:

1. C systems fundamentals.
2. Modern C++ resource management and abstraction.
3. Python testing and automation around C/C++ programs.

Python is included because it is a practical systems engineer's helper language.
It is excellent for integration tests, load generators, corpus triage,
benchmark reports, and quick developer tooling.

## The Core Pattern

Every module follows the same loop:

1. Read the lesson.
2. Build the starter code.
3. Run the tests and observe failure.
4. Implement one small unit.
5. Re-run tests.
6. Debug with a real tool.
7. Write a short note explaining one bug or design choice.

The small written note is not busywork. It trains the skill of explaining
systems behavior, which is often what interviews and production debugging
really test.

## Expected Weekly Load

For people with jobs, plan for 5 to 8 hours per week:

- 60 to 90 minutes reading and examples;
- 3 to 5 hours implementation;
- 30 to 60 minutes debugging/reporting;
- 30 minutes peer review.

## Assignment Progression

`hw01-c-puzzle` teaches C data modeling, validation, arrays, and tests.

`hw02-unix-runner` teaches Unix process control, redirection, exit status, and
timeouts.

`hw03-fuzzer-lite` teaches mutation, queues, crash classification, and
sanitizer-friendly workflows.

`bonus-cpp-warmup` teaches C++ strings, vectors, optional results, enum classes,
headers, and small class design before the RAII assignment.

`hw04-cpp-raii` teaches modern C++ ownership and move-only resource wrappers.

`hw05-concurrent-server` teaches protocol parsing, shared state, threads, and
integration testing.

`c-fuzzlab` turns the runner and fuzzer ideas into a resume project.

`cpp-pulsekv` turns RAII and concurrency ideas into a resume project.

## Definition of Done

A homework is done when:

- it compiles with warnings enabled;
- unit tests pass;
- at least one sanitizer or Valgrind run is clean;
- edge cases are documented;
- the README has a short implementation note.

A capstone is done when:

- it has a working CLI;
- it has unit tests and Python integration tests;
- it has at least one benchmark or stress test;
- it has a final architecture report;
- it has two polished resume bullets.
