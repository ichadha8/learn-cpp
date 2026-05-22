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
