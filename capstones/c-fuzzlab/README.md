# C Capstone: FuzzLab

## Resume Name

FuzzLab: Coverage-Guided Process Fuzzer in C with Python Crash Triage

## Pitch

FuzzLab is a mini AFL-style fuzzer written in C. It launches target programs as
child processes, feeds them mutated inputs, enforces timeouts, classifies
crashes, stores interesting inputs, and uses Python tooling to triage crashes
and produce reports.

This capstone is meant to be resume-worthy. It combines:

- C memory management;
- Unix process control;
- signals and timeouts;
- file descriptor redirection;
- mutation-based testing;
- optional shared-memory coverage;
- Python automation and reporting.

## What You Will Build

Required C features:

- CLI parser:
  - `-s seed_dir`
  - `-o output_dir`
  - `-n iterations`
  - `-t timeout_ms`
  - `-- target_program @@ args...`
- seed corpus loader;
- deterministic mutation engine;
- child runner using `fork`, `execvp`, `waitpid`;
- stdout/stderr redirection;
- crash and timeout classification;
- crash artifact saving;
- run summary JSON.

Required Python features:

- `python/triage.py`: groups crashes by signal and input hash;
- `python/report.py`: creates a text or HTML summary;
- `python/make_seeds.py`: creates starter corpora for demo targets.

Optional advanced features:

- shared-memory coverage map;
- input minimization;
- parallel runners;
- sanitizer target examples;
- corpus pruning.

## Starter Boundary

The starter code gives you the public types, function signatures, a demo target,
and Python tooling skeletons. It does not implement the actual campaign engine.

You are expected to implement:

- argument parsing;
- replacing `@@` in target arguments;
- writing mutated inputs to temporary files;
- launching and timing target programs;
- classifying target status;
- saving crash files;
- producing a run summary.

This is intentional. The capstone is resume-worthy only if you build the hard
systems pieces yourself.

## Milestones

### Milestone 1: Runner

Implement:

- `fuzzlab_classify_status`;
- `fuzzlab_run_target`;
- timeout handling;
- stdout/stderr redirection to `/dev/null`.

Done when:

- unit tests classify exit/crash/timeout;
- a target can be run once with one input file.

### Milestone 2: Corpus

Implement:

- seed loading;
- corpus queue;
- output directory creation;
- crash file naming.

Done when:

- seeds are discovered;
- the fuzzer can loop over inputs;
- crash inputs are saved.

### Milestone 3: Mutations

Implement:

- bit flip;
- byte increment/decrement;
- duplicate input;
- append boundary strings;
- splice two inputs if available.

Done when:

- mutation tests pass;
- `-n 100` generates 100 attempted inputs.

### Milestone 4: Python Triage

Implement:

- crash hashing;
- grouping by signal;
- summary table;
- optional target rerun to confirm crash.

Done when:

- `python3 python/triage.py out/crashes` prints grouped results.

### Milestone 5: Report and Demo

Add:

- `REPORT.md`;
- one benchmark run;
- one screenshot or copied terminal demo;
- final resume bullets.

## Architecture

```text
fuzzlab
  main.c
    parse args
    initialize config
    call fuzzlab_run

  corpus.c
    load seed files
    queue interesting inputs
    save crashes

  mutate.c
    deterministic mutations

  runner.c
    fork/exec target
    redirect output
    enforce timeout
    classify status

python/
  triage.py
  report.py
  make_seeds.py
```

## Public Function Contracts

### `unsigned int fuzzlab_hash_bytes(const unsigned char *bytes, size_t length)`

Computes a stable hash for input bytes. This is used for naming artifacts and
grouping crashes. A starter implementation may exist, but you should understand
that the hash must be deterministic across runs.

### `int fuzzlab_input_from_string(fuzzlab_input_t *out, const char *text)`

Creates an input object from a C string. This is mostly for tests and seed
helpers. The fuzzer itself should treat inputs as byte buffers, not strings.

### `int fuzzlab_mutate(const fuzzlab_input_t *input, fuzzlab_input_t *out, unsigned int seed)`

Produces one mutated input without modifying `input`.

Required first version:

- handle empty input;
- copy input to output;
- apply one deterministic byte/bit mutation based on seed.

Expanded version:

- cycle through multiple mutation strategies;
- truncate safely at `FUZZLAB_MAX_INPUT`;
- preserve deterministic behavior for tests.

### `fuzzlab_result_t fuzzlab_run_target(char *const argv[], const char *input_path, int timeout_ms)`

Runs the target once.

Required behavior:

1. Create a child process.
2. Replace `@@` argument with `input_path`.
3. Redirect stdout/stderr.
4. Execute target with `execvp`.
5. Enforce timeout.
6. Classify status.
7. Reap the child.

This is the heart of the capstone.

### `int fuzzlab_run(const fuzzlab_config_t *config)`

Runs a full fuzzing campaign:

```text
load seeds
for each iteration:
    choose input
    mutate
    run target
    save crash or interesting input
write summary
cleanup
```

This function should own the campaign-level resources and cleanup paths.

## Suggested Demo Target

Write a deliberately fragile parser:

```c
if (strstr(input, "CRASH")) {
    int *p = NULL;
    *p = 1;
}
```

Compile it with AddressSanitizer, then show FuzzLab discovering and saving the
input that triggers the crash.

## Resume Bullets

- Built a mutation-based C fuzzer using POSIX process control, signal-driven
  timeouts, file descriptor redirection, and crash corpus management.
- Added Python crash triage and reporting tools to group failing inputs,
  reproduce crashes, and summarize fuzzing campaigns.

## Commands

```bash
cmake --preset capstone-c
cmake --build --preset capstone-c
ctest --preset capstone-c --output-on-failure
```

Example final run:

```bash
./build/capstone-c/capstones/c-fuzzlab/fuzzlab \
  -s capstones/c-fuzzlab/seeds \
  -o out/fuzzlab \
  -n 1000 \
  -t 250 \
  -- ./build/capstone-c/capstones/c-fuzzlab/fuzzlab_crashy @@

python3 capstones/c-fuzzlab/python/triage.py out/fuzzlab/crashes
```
