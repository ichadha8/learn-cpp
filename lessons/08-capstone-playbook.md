# Lesson 08: Capstone Playbook

## What Makes a Capstone Resume-Worthy

A resume-worthy systems project is not just a program that runs. It has:

- clear problem statement;
- real systems concepts;
- tests;
- measurable behavior;
- failure handling;
- developer tooling;
- documentation;
- a clean demo path.

The capstones in this repo are intentionally larger than the weekly homeworks.
They should feel like small production projects.

## Capstone Process

Use milestones:

1. Skeleton builds.
2. Core library unit tests pass.
3. CLI works for a simple case.
4. Python integration tests pass.
5. Stress or benchmark tool works.
6. Error handling is tightened.
7. README and final report are polished.

Do not start with every feature. Start with the smallest observable behavior and
grow outward.

## C Capstone: FuzzLab

The C capstone should show:

- process control;
- signals;
- file descriptor redirection;
- mutation;
- crash detection;
- timeout handling;
- corpus management;
- Python triage/reporting.

The best implementation path:

1. Run one target with one input.
2. Classify exit, crash, timeout.
3. Load seed corpus.
4. Add mutations.
5. Save crashes.
6. Add coverage or coverage-like priority.
7. Add Python triage reports.

## C++ Capstone: PulseKV

The C++ capstone should show:

- RAII wrappers;
- STL containers;
- thread pool;
- TCP server;
- parser;
- persistent append-only log;
- metrics;
- Python load testing.

The best implementation path:

1. Build the in-memory store.
2. Parse commands from strings.
3. Add a single-client server.
4. Add thread pool.
5. Add persistence.
6. Add metrics.
7. Add Python load tests and dashboard/reporting.

## Final Report Template

Each capstone should include:

- architecture overview;
- module list;
- protocol or CLI spec;
- failure modes;
- testing strategy;
- benchmark result;
- limitations;
- next steps;
- resume bullets.

## Demo Checklist

Before showing the project:

- fresh clone builds;
- tests pass;
- demo command is in README;
- sample output is included;
- no secret paths or local-only assumptions;
- screenshots or reports are committed if useful.
