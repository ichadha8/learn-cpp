# Starter Code Scope

This document explains what the repository intentionally provides and what is
intentionally left for learners.

The rule for this course is:

```text
Provide enough structure to build and test.
Leave the core systems thinking and implementation to the learner.
```

## Global Policy

Provided:

- CMake build structure;
- Codespaces/devcontainer setup;
- public headers and method signatures;
- unit-test drivers;
- Python integration-tool skeletons;
- project READMEs and implementation guidance;
- small utility functions when they are not the learning goal.

Left to implement:

- core algorithms;
- OS interaction logic;
- parser details;
- synchronization logic;
- resource cleanup paths;
- capstone architecture internals.

All TODO functions should compile by using `(void)` on unused parameters and
returning a safe placeholder value.

## HW01: C Puzzle Engine

Provided:

- `puzzle_t` layout;
- `puzzle_init`;
- `puzzle_load`;
- tests describing visible counts, duplicates, and placement.

Left to implement:

- visible-count logic;
- duplicate checks;
- clue validation;
- placement validation;
- basic solver rules.

Rationale:

Parsing compact strings is not the main lesson. Validation and careful
row/column reasoning are the lesson.

## HW02: Unix Program Runner

Provided:

- result types;
- `runner_make_error`;
- tests describing process behavior.

Left to implement:

- wait-status classification;
- `fork`/`execvp`;
- output redirection;
- timeout loop;
- child cleanup.

Rationale:

The OS process model is the lesson. The starter should not hide it.

## HW03: Fuzzer-Lite

Provided:

- input and queue structs;
- `fl_queue_init`;
- `fl_input_from_cstr`;
- test descriptions.

Left to implement:

- queue destroy/push/pop;
- mutation functions;
- memory cleanup edge cases;
- optional fuzzer loop.

Rationale:

The learner should practice C ownership and deterministic mutation logic.

## Bonus: C++ Warmup Starship Codex

Provided:

- public C++ API for enum/struct/class practice;
- parser and collection function signatures;
- CMake preset and tests;
- an in-depth README with LearnCpp reading links and implementation hints.

Left to implement:

- enum/string conversion;
- strict record parsing;
- formatting;
- vector queries;
- duplicate-safe `Codex` class operations.

Rationale:

This bridge keeps HW04 focused on RAII by giving learners a smaller place to
practice C++ syntax, `std::string`, `std::vector`, `std::optional`, and classes.

## HW04: C++ RAII Systems Library

Provided:

- class declarations;
- partial `UniqueFd` skeleton;
- tests describing ownership behavior.

Left to implement:

- buffer append/string conversion;
- file read/write;
- command runner;
- additional move/cleanup edge cases.

Rationale:

Some `UniqueFd` structure is shown because RAII syntax is new, but learners
still need to complete and test the resource behavior.

## HW05: Concurrent Server Core

Provided:

- shared state structs;
- state init/destroy skeleton;
- parser and command signatures;
- Python smoke-client skeleton.

Left to implement:

- join/leave state transitions;
- exact command parser;
- command application;
- mutex-protected shared state updates;
- optional socket server.

Rationale:

The assignment should teach synchronization and parser correctness. A complete
starter would skip the point.

## C Capstone: FuzzLab

Provided:

- capstone architecture spec;
- public fuzzer types;
- hash/input helper examples;
- demo crashy target;
- Python triage/report script skeletons.

Left to implement:

- CLI parser;
- runner;
- timeout handling;
- corpus loading;
- mutation suite;
- crash artifact saving;
- campaign summary.

Rationale:

The capstone should demonstrate systems engineering, not only filling in a few
small functions.

## C++ Capstone: PulseKV

Provided:

- command/store types;
- method signatures;
- REPL placeholder;
- Python client/load-test skeletons.

Left to implement:

- store logic;
- parser;
- command execution;
- RAII sockets;
- thread pool;
- persistence;
- metrics;
- full TCP server.

Rationale:

This should become a resume-grade service project. The starter provides the
shape, not the implementation.
