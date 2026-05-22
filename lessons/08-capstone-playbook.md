# Lesson 08: Capstone Playbook and Portfolio Standards

## What This Lesson Teaches

The capstones are not just bigger homeworks. They are portfolio projects. That
means the implementation matters, but so do the README, tests, demo path,
failure handling, and final report.

By the end, you should know how to plan and execute the two capstones without
turning them into unbounded projects.

## What Makes a Systems Capstone Resume-Worthy

A resume-worthy systems project has:

- a specific problem statement;
- real OS/compiler/networking/concurrency concepts;
- a build that works from a fresh clone;
- unit tests;
- integration tests;
- Python tooling around the C/C++ core;
- measurable output;
- a final architecture report;
- a short demo path.

It is not enough to say "implemented a fuzzer" or "implemented a server." You
need to show what engineering constraints you handled.

## Milestone Discipline

Each capstone must be implemented in stages:

1. Core data structures compile.
2. Unit tests pass.
3. Minimal CLI works.
4. Integration test works.
5. Error handling is tightened.
6. Stress/benchmark tool works.
7. Report and README are polished.

Do not start with the hardest feature. Start with the smallest behavior that can
be observed and tested.

## C Capstone: FuzzLab

FuzzLab demonstrates:

- process control;
- signals;
- timeouts;
- file descriptor redirection;
- mutation;
- crash classification;
- corpus management;
- Python triage and reporting.

### Implementation Order

1. Implement input representation and hashing.
2. Implement mutation functions.
3. Implement one target run with one input file.
4. Add timeout handling.
5. Add seed corpus loading.
6. Add crash saving.
7. Add Python triage.
8. Add optional coverage-like prioritization.

### What to Leave for the Learner

Starter code should provide:

- public headers;
- structs and enums;
- empty function bodies;
- demo target;
- Python script skeletons.

Starter code should not provide:

- working runner loop;
- complete CLI parsing;
- complete mutation suite;
- crash artifact naming;
- corpus priority logic.

The README should explain the architecture enough that a learner can implement
it without being handed the code.

## C++ Capstone: PulseKV

PulseKV demonstrates:

- modern C++ ownership;
- STL containers;
- RAII wrappers;
- parser design;
- thread-safe state;
- TCP server design;
- thread pool;
- Python load testing.

Preparation links:

- C++ warmup lesson: `05b-cpp-warmup.md`
- STL/RAII lesson: `06-cpp-raii-stl-templates.md`
- LearnCpp smart pointers and move semantics: https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
- LearnCpp `std::unique_ptr`: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/
- LearnCpp standard algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/
- LearnCpp containers and arrays: https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/

### Implementation Order

1. Implement `Store` without networking.
2. Implement command parsing.
3. Implement command execution.
4. Add append-only log.
5. Add single-client TCP server.
6. Add thread pool.
7. Add metrics.
8. Add Python load test and report.

### What to Leave for the Learner

Starter code should provide:

- class declarations;
- method signatures;
- command/response enums;
- tests that describe behavior;
- Python client/load-test skeletons.

Starter code should not provide:

- full store implementation;
- full parser implementation;
- full TCP server implementation;
- thread pool implementation.

## Report Template

Each capstone should finish with a `REPORT.md`:

```text
# Project Report

## Summary

## Architecture

## Build and Run

## Testing Strategy

## Interesting Bugs

## Performance or Stress Result

## Limitations

## Future Work

## Resume Bullets
```

## Interview Prep

For each capstone, be ready to explain:

- why you chose the architecture;
- what could go wrong;
- how you tested it;
- how you handled memory/resource lifetime;
- one bug you found and fixed;
- what you would improve with more time.

## Final Quality Bar

Before calling a capstone done:

- fresh clone builds;
- tests pass;
- Python demo works;
- README has exact commands;
- report explains architecture;
- no local-only paths;
- no generated files committed;
- no secrets or machine-specific assumptions.
