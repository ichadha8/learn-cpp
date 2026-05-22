# C and C++ Systems Training

This repository is a project-based training regimen for learning C, modern C++,
Linux systems programming, debugging, performance work, and resume-grade project
delivery.

It is designed for two learners who already know basic programming and want a
compressed, job-focused path. The normal track is six weeks. The capstone track
adds two larger projects: one in C and one in C++.

## Course Shape

- `lessons/` contains notes and implementation guidance.
- `assignments/` contains five scaffolded homeworks with starter code and tests.
- `capstones/` contains two portfolio projects with C/C++ cores and Python tooling.
- `.devcontainer/` configures GitHub Codespaces.
- `.github/workflows/` contains a starter CI workflow.

## Fast Start

Open the repository in GitHub Codespaces, then run:

```bash
cmake --preset dev
cmake --build --preset dev
```

To work on one assignment with its unit tests:

```bash
cmake --preset hw01
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

Repeat with `hw02`, `hw03`, `hw04`, `hw05`, `capstone-c`, or `capstone-cpp`.

The scaffold build is expected to pass before any homework is solved. Individual
homework tests are intentionally written against the TODOs, so they may fail
until you implement that assignment. The GitHub Actions workflow runs the
scaffold build automatically and exposes the full assignment-test matrix as a
manual workflow.

## Recommended Timeline

Week 1: C tooling, pointers, arrays, testing, and debugging.

Week 2: C memory, structs, ownership, parsing, and validation.

Week 3: Unix files, processes, redirection, signals, and timeouts.

Week 4: profiling, caches, mutation, fuzzing, and sanitizer workflows.

Week 5: modern C++, RAII, STL, move semantics, and CMake.

Week 6: threads, sockets, synchronization, and service design.

Capstone C: FuzzLab, a coverage-guided process fuzzer with Python triage.

Capstone C++: PulseKV, a multithreaded key-value server with Python load tools.

## Design Philosophy

Each homework is intentionally smaller than the hardest CSE320-style projects,
but keeps the systems-course flavor: precise specs, strict output, unit tests,
integration tests, and debugging requirements.

The capstones are where the gloves come off. They are milestone-based,
resume-worthy projects meant to demonstrate engineering judgment, testing,
tooling, and documentation.
