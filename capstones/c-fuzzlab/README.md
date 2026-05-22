# C Capstone: FuzzLab

## Resume Name

FuzzLab: Coverage-Guided Process Fuzzer in C with Python Crash Triage

## Pitch

FuzzLab is a mini AFL-style fuzzer written in C. It launches target programs as
child processes, feeds them mutated inputs, enforces timeouts, classifies
crashes, stores artifacts, and uses Python tooling to triage crashes and produce
reports.

This capstone should be harder than the homeworks. It combines several systems
skills at once:

- C memory and byte-buffer management;
- Unix process control;
- file descriptor redirection;
- timeout handling and child cleanup;
- deterministic mutation strategies;
- corpus management;
- crash artifact naming;
- summary/report generation;
- Python automation around a C core.

## What You Will Build

Required C features:

- CLI parser:
  - `-s seed_dir`
  - `-o output_dir`
  - `-n iterations`
  - `-t timeout_ms`
  - `-- target_program @@ args...`
- seed corpus loader;
- deterministic mutation engine with multiple mutation strategies;
- child runner using `fork`, `execvp`, and `waitpid`;
- stdout/stderr redirection;
- timeout enforcement;
- crash, timeout, and runner-error classification;
- crash and timeout artifact saving;
- run summary JSON;
- safe cleanup on all error paths.

Required Python features:

- `python/make_seeds.py`: creates demo seed corpora;
- `python/triage.py`: groups crashes by signal and input hash;
- `python/report.py`: creates a readable summary from campaign output.

Stretch features:

- shared-memory coverage map;
- input minimization;
- parallel runners;
- sanitizer target examples;
- corpus pruning;
- artifact deduplication by signal and input hash.

## Source File Split

FuzzLab is split into four C source files:

```text
src/main.c
src/mutate.c
src/runner.c
src/campaign.c
```

Keep that split. Do not collapse the capstone back into a single `fuzzlab.c`.

Ownership:

| File | Responsibility |
|:--|:--|
| `src/main.c` | CLI parsing, config validation, top-level exit status |
| `src/mutate.c` | byte hashing, input helpers, deterministic mutations |
| `src/runner.c` | status classification, `fork`/`execvp`, timeouts, redirection |
| `src/campaign.c` | seed corpus, temporary inputs, artifact saving, summary, `fuzzlab_run` |

The public interface lives in `include/fuzzlab.h` so the tests and Python demo
tools have one stable contract.

## Directory Layout

```text
capstones/c-fuzzlab
|-- CMakeLists.txt
|-- DESIGN.md
|-- README.md
|-- include
|   `-- fuzzlab.h
|-- python
|   |-- make_seeds.py
|   |-- report.py
|   `-- triage.py
|-- seeds
|   `-- seed_000.txt
|-- src
|   |-- campaign.c
|   |-- main.c
|   |-- mutate.c
|   `-- runner.c
|-- targets
|   `-- crashy.c
`-- tests
    `-- test_fuzzlab.cpp
```

## Starter Boundary

The starter code gives you:

- public types;
- function signatures;
- simple hash/input helpers;
- a simple first mutation;
- TODO stubs for runner and campaign logic;
- a demo crash target;
- Python tooling skeletons.

The starter intentionally does not implement:

- CLI parsing;
- seed directory traversal;
- corpus queue logic;
- target argument replacement;
- process launching;
- timeout enforcement;
- output redirection;
- artifact writing;
- campaign summary writing.

The goal is to build the hard systems pieces yourself.

## Build and Test

From the repository root:

```bash
cmake --preset capstone-c
cmake --build --preset capstone-c
ctest --preset capstone-c --output-on-failure
```

The scaffold should compile. Capstone tests may fail until the TODO functions
are implemented.

## Program Operation

Final command shape:

```bash
fuzzlab -s seed_dir -o output_dir -n iterations -t timeout_ms -- target_program @@ args...
```

Example:

```bash
./build/capstone-c/capstones/c-fuzzlab/fuzzlab \
  -s capstones/c-fuzzlab/seeds \
  -o out/fuzzlab \
  -n 1000 \
  -t 250 \
  -- ./build/capstone-c/capstones/c-fuzzlab/fuzzlab_crashy @@
```

Rules:

- `-s` is required.
- `-o` is required.
- `-n` must be positive.
- `-t` must be positive.
- the target command must appear after `--`.
- exactly one target argument should be `@@`;
- `@@` is replaced by the current generated input path before `execvp`.

On invalid arguments, print a one-line error to `stderr` and exit nonzero.

## Output Layout

The output directory should be created if it does not exist.

Required final shape:

```text
out/fuzzlab
|-- crashes
|   |-- id_000042_sig11_hash1a2b3c4d.input
|   `-- id_000042_sig11_hash1a2b3c4d.json
|-- timeouts
|   |-- id_000077_timeout_hash9abc1234.input
|   `-- id_000077_timeout_hash9abc1234.json
|-- queue
|   `-- id_000005_hash11112222.input
`-- summary.json
```

The exact artifact names may vary, but they must be deterministic and include:

- iteration number;
- result kind;
- signal or timeout marker;
- input hash.

## Milestones

### Milestone 1: Mutation and Input Helpers

Implement or expand:

- `fuzzlab_hash_bytes`;
- `fuzzlab_input_from_string`;
- `fuzzlab_mutate`.

Done when:

- hashing is deterministic;
- input helpers reject invalid pointers and oversized strings;
- mutation preserves valid lengths;
- non-empty inputs change for common seeds;
- the expanded mutation suite includes at least bit flip, byte add/subtract,
  boundary append, and truncation.

### Milestone 2: Runner

Implement:

- `fuzzlab_classify_status`;
- `fuzzlab_run_target`;
- `@@` replacement;
- stdout/stderr redirection to `/dev/null`;
- timeout handling;
- child reaping.

Done when:

- normal exit is classified as `FUZZLAB_VALID`;
- signal death is classified as `FUZZLAB_CRASH`;
- timeout is classified as `FUZZLAB_TIMEOUT`;
- failed fork/exec/setup is classified as `FUZZLAB_RUNNER_ERROR`;
- no zombie children are left behind.

### Milestone 3: Corpus and Artifacts

Implement:

- `fuzzlab_corpus_init`;
- `fuzzlab_corpus_push`;
- `fuzzlab_load_seeds`;
- `fuzzlab_write_input_file`;
- `fuzzlab_save_artifact`.

Done when:

- seed files are loaded from the seed directory;
- empty and oversized seed files are handled deliberately;
- corpus capacity is enforced;
- crashes and timeouts are saved with deterministic names;
- metadata sidecar files are written.

### Milestone 4: Campaign Engine

Implement:

- `fuzzlab_run`;
- mutation loop;
- target execution loop;
- crash/timeout saving;
- queue saving for interesting inputs;
- `fuzzlab_write_summary`.

Done when:

- `-n 100` performs 100 attempted target runs;
- summary counters add up;
- all temporary files are cleaned up or stored intentionally;
- campaign failure paths release resources.

### Milestone 5: Python Triage and Report

Implement:

- crash grouping by signal and hash;
- timeout grouping;
- report table;
- optional crash reproduction.

Done when:

```bash
python3 capstones/c-fuzzlab/python/triage.py out/fuzzlab/crashes
python3 capstones/c-fuzzlab/python/report.py out/fuzzlab/summary.json
```

produce readable output.

### Milestone 6: Final Demo

Add:

- `REPORT.md`;
- a demo target run;
- final resume bullets;
- one paragraph explaining a bug you found in process cleanup or timeout logic.

## Public Type Contracts

### `fuzzlab_state_t`

Represents the result category from one target execution:

```c
FUZZLAB_VALID
FUZZLAB_CRASH
FUZZLAB_TIMEOUT
FUZZLAB_RUNNER_ERROR
```

`FUZZLAB_VALID` does not necessarily mean the target accepted the input; it
means the target exited normally.

### `fuzzlab_artifact_kind_t`

Represents what kind of saved artifact is being written:

```c
FUZZLAB_ARTIFACT_CRASH
FUZZLAB_ARTIFACT_TIMEOUT
FUZZLAB_ARTIFACT_INTERESTING
```

Use this to choose output subdirectories and metadata fields.

### `fuzzlab_result_t`

```c
typedef struct {
    fuzzlab_state_t state;
    int aux;
    int elapsed_ms;
} fuzzlab_result_t;
```

Fields:

- `state`: result category;
- `aux`: exit code, signal number, or `-1` for runner errors;
- `elapsed_ms`: approximate wall-clock runtime.

### `fuzzlab_input_t`

```c
typedef struct {
    unsigned char bytes[FUZZLAB_MAX_INPUT];
    size_t length;
} fuzzlab_input_t;
```

This is a byte buffer, not a C string. Inputs may contain `\0` bytes.

### `fuzzlab_corpus_t`

```c
typedef struct {
    fuzzlab_input_t inputs[FUZZLAB_MAX_CORPUS];
    size_t count;
} fuzzlab_corpus_t;
```

This fixed-size corpus is enough for the capstone starter. A stretch version
may replace it with a dynamic queue.

### `fuzzlab_config_t`

```c
typedef struct {
    const char *seed_dir;
    const char *output_dir;
    int iterations;
    int timeout_ms;
    char **target_argv;
} fuzzlab_config_t;
```

This is the validated campaign configuration passed from `main.c` to
`fuzzlab_run`.

### `fuzzlab_summary_t`

```c
typedef struct {
    int total_runs;
    int valid_runs;
    int crashes;
    int timeouts;
    int runner_errors;
    int saved_artifacts;
} fuzzlab_summary_t;
```

This should be written to `summary.json` at the end of a campaign.

## Public Function Contracts

### `unsigned int fuzzlab_hash_bytes(const unsigned char *bytes, size_t length)`

Computes a deterministic hash for a byte buffer.

Parameters:

- `bytes`: input byte pointer, may be `NULL` only when `length == 0`;
- `length`: number of bytes to hash.

Returns:

- stable unsigned hash value.

Use this for artifact names and triage grouping. Do not use a random seed.

### `int fuzzlab_input_from_string(fuzzlab_input_t *out, const char *text)`

Creates a `fuzzlab_input_t` from a null-terminated string.

Parameters:

- `out`: output input object;
- `text`: source string.

Returns:

- `0` on success;
- `-1` on invalid pointer or oversized text.

This helper is mostly for tests and seed generation. The campaign should treat
inputs as byte buffers.

### `int fuzzlab_mutate(const fuzzlab_input_t *input, fuzzlab_input_t *out, unsigned int seed)`

Produces one mutated input without modifying `input`.

Parameters:

- `input`: source input;
- `out`: destination input;
- `seed`: deterministic mutation selector.

Returns:

- `0` on success;
- `-1` on invalid pointers or invalid input length.

Required mutation strategies:

- bit flip;
- byte increment;
- byte decrement;
- append one boundary byte if capacity allows;
- truncate input if length is greater than one.

Use `seed` to choose strategy and offset. The same input and seed must produce
the same output.

### `fuzzlab_result_t fuzzlab_classify_status(int status, int elapsed_ms)`

Classifies a `waitpid` status.

Parameters:

- `status`: raw wait status;
- `elapsed_ms`: measured runtime.

Returns:

- `FUZZLAB_VALID` with `aux = exit_code` for normal exit;
- `FUZZLAB_CRASH` with `aux = signal_number` for signal death;
- `FUZZLAB_RUNNER_ERROR` with `aux = -1` for anything else.

Timeouts are usually classified in `fuzzlab_run_target`, because timeout logic
kills the child before status classification.

### `fuzzlab_result_t fuzzlab_run_target(char *const argv[], const char *input_path, int timeout_ms)`

Runs one target process against one input file.

Parameters:

- `argv`: target argv vector containing exactly one `@@` placeholder;
- `input_path`: path to the input file for this run;
- `timeout_ms`: maximum runtime.

Required behavior:

1. Validate arguments.
2. Build a child argv with `@@` replaced by `input_path`.
3. Fork.
4. In the child, redirect stdout/stderr to `/dev/null`.
5. In the child, call `execvp`.
6. In the parent, poll `waitpid` with `WNOHANG`.
7. Kill and reap the child on timeout.
8. Return a classified result.

Returns:

- `FUZZLAB_VALID`, `FUZZLAB_CRASH`, `FUZZLAB_TIMEOUT`, or
  `FUZZLAB_RUNNER_ERROR`.

Never leave a child process unreaped.

### `void fuzzlab_corpus_init(fuzzlab_corpus_t *corpus)`

Initializes a corpus object.

Parameters:

- `corpus`: corpus to initialize.

Returns: nothing.

This function should safely ignore `NULL`.

### `int fuzzlab_corpus_push(fuzzlab_corpus_t *corpus, const fuzzlab_input_t *input)`

Adds one input to the corpus.

Parameters:

- `corpus`: destination corpus;
- `input`: input to copy.

Returns:

- `0` on success;
- `-1` on invalid pointers, invalid input length, or full corpus.

### `int fuzzlab_load_seeds(const char *seed_dir, fuzzlab_corpus_t *corpus)`

Loads seed files from a directory into a corpus.

Parameters:

- `seed_dir`: directory containing seed files;
- `corpus`: destination corpus.

Returns:

- number of seeds loaded on success;
- `-1` on directory or read error.

Rules:

- skip subdirectories;
- reject files larger than `FUZZLAB_MAX_INPUT`;
- load files in deterministic sorted order if possible.

### `int fuzzlab_write_input_file(const char *path, const fuzzlab_input_t *input)`

Writes one fuzz input to disk.

Parameters:

- `path`: output path;
- `input`: input bytes to write.

Returns:

- `0` on success;
- `-1` on invalid arguments or write failure.

Handle partial writes.

### `int fuzzlab_save_artifact(const char *output_dir, fuzzlab_artifact_kind_t kind, int iteration, const fuzzlab_input_t *input, const fuzzlab_result_t *result)`

Saves an input and metadata sidecar for a crash, timeout, or interesting input.

Parameters:

- `output_dir`: campaign output directory;
- `kind`: artifact type;
- `iteration`: campaign iteration;
- `input`: bytes that produced the result;
- `result`: classified run result.

Returns:

- `0` on success;
- `-1` on invalid arguments or file failure.

This function should create deterministic artifact names using
`fuzzlab_hash_bytes`.

### `int fuzzlab_write_summary(const char *output_dir, const fuzzlab_summary_t *summary)`

Writes `summary.json`.

Parameters:

- `output_dir`: campaign output directory;
- `summary`: final counters.

Returns:

- `0` on success;
- `-1` on invalid arguments or write failure.

### `int fuzzlab_run(const fuzzlab_config_t *config)`

Runs a full fuzzing campaign.

Required algorithm:

```text
validate config
create output directories
load seeds
for iteration in 0..iterations-1:
    choose corpus input
    mutate input
    write temp input file
    run target
    update summary
    save crash/timeout artifacts
write summary.json
cleanup
```

Returns:

- `0` on completed campaign;
- `-1` on setup or campaign failure.

## Implementation Notes

### Target Argument Replacement

Do not modify the original `target_argv` in place. Build a temporary argv array
for the child so repeated runs do not corrupt the config.

### Output Redirection

The child should redirect stdout and stderr to `/dev/null` for the required
version. A stretch version may capture output to files.

### Timeouts

A reliable timeout loop:

1. record start time;
2. call `waitpid(pid, &status, WNOHANG)`;
3. sleep briefly;
4. kill child if elapsed time exceeds `timeout_ms`;
5. call `waitpid` again to reap.

Use `SIGKILL` for the required version.

### Cleanup

Every path that opens a file descriptor should close it. Every path that forks a
child should reap it. Every path that allocates memory should free it.

## Python Tooling

### `python/make_seeds.py`

Should create a small seed directory containing normal and boundary inputs.

### `python/triage.py`

Should read crash artifacts and group them by:

- signal;
- input hash;
- optional reproduction result.

### `python/report.py`

Should read `summary.json` and print:

- total runs;
- valid runs;
- crash count;
- timeout count;
- runner errors;
- saved artifacts;
- most common crash signatures.

## Suggested Tests

Add tests for:

- hash stability;
- oversized input rejection;
- each mutation strategy;
- status classification for exit and signal;
- target argv replacement;
- timeout classification;
- corpus full behavior;
- seed directory loading;
- partial write handling;
- artifact naming;
- summary JSON content.

## Suggested Demo Target

Use the provided `targets/crashy.c`, then add your own fragile parser:

```c
if (strstr(input, "CRASH")) {
    int *p = NULL;
    *p = 1;
}
```

Compile with AddressSanitizer for a stronger demo.

## Demo Commands

Build:

```bash
cmake --preset capstone-c
cmake --build --preset capstone-c
ctest --preset capstone-c --output-on-failure
```

Run:

```bash
./build/capstone-c/capstones/c-fuzzlab/fuzzlab \
  -s capstones/c-fuzzlab/seeds \
  -o out/fuzzlab \
  -n 1000 \
  -t 250 \
  -- ./build/capstone-c/capstones/c-fuzzlab/fuzzlab_crashy @@
```

Triage:

```bash
python3 capstones/c-fuzzlab/python/triage.py out/fuzzlab/crashes
python3 capstones/c-fuzzlab/python/report.py out/fuzzlab/summary.json
```

## Grading Breakdown

| Area | Weight | Notes |
|:--|:-:|:--|
| Mutation engine | 15% | deterministic, varied, safe bounds |
| Runner and timeout handling | 25% | fork/exec, redirection, classification, no zombies |
| Corpus and artifact management | 20% | seed loading, deterministic names, metadata |
| Campaign engine | 20% | loop, summary, cleanup, error handling |
| Python triage/reporting | 10% | useful crash grouping and readable output |
| Tests and demo quality | 10% | edge tests, final report, resume story |

## Resume Bullets

- Built a mutation-based C fuzzer using POSIX process control, timeout
  enforcement, file descriptor redirection, corpus management, and crash
  artifact generation.
- Added Python crash triage and reporting tools to group failing inputs,
  reproduce crashes, and summarize fuzzing campaigns.

## Final Deliverables

Before calling this capstone done, produce:

- passing C/C++ unit tests;
- working CLI demo;
- crash or timeout artifacts;
- `summary.json`;
- Python triage output;
- `REPORT.md`;
- final resume bullets;
- short explanation of one process-control or cleanup bug you fixed.
