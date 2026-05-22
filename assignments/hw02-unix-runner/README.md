# Homework 2: Unix Program Runner

Readme version 2.0

## Introduction

In this assignment, you will implement a small Unix program runner in C. The
runner starts another program as a child process, redirects its output, waits
for it to finish, enforces a timeout, and reports whether the child exited
normally, crashed from a signal, timed out, or failed to launch.

This is the foundation for the fuzzer assignments. A fuzzer is only useful if
it can run target programs many times without leaking children, confusing exit
states, or hanging forever.

The goal is to become comfortable with:

- `fork`;
- `execvp`;
- `waitpid`;
- `dup2`;
- `open` and `close`;
- child exit status macros;
- timeout logic;
- exact error handling.

## Base Code Policy

The public interface is in `include/runner.h`. Do not change the enum names,
struct fields, or function signatures. Tests and later assignments depend on
that interface.

You may add helper functions to `src/runner.c`. Keep helper functions `static`
unless they must be shared.

## Directory Layout

```text
hw02-unix-runner/
├── CMakeLists.txt
├── README.md
├── include/
│   └── runner.h
├── src/
│   └── runner.c
└── tests/
    └── test_runner.cpp
```

## Build and Test

```bash
cmake --preset hw02
cmake --build --preset hw02
ctest --preset hw02 --output-on-failure
```

## Process Model

The runner has two roles after `fork`:

```text
parent process
  waits
  enforces timeout
  classifies result

child process
  redirects output
  execs target program
```

After `fork`, both processes continue from the next line. You must separate
parent and child logic using the return value of `fork`.

## Required Types

### `runner_state_t`

```c
typedef enum {
    RUNNER_EXITED,
    RUNNER_SIGNALED,
    RUNNER_TIMEOUT,
    RUNNER_ERROR
} runner_state_t;
```

Meaning:

- `RUNNER_EXITED`: child exited normally.
- `RUNNER_SIGNALED`: child was killed by a signal.
- `RUNNER_TIMEOUT`: runner killed the child because time expired.
- `RUNNER_ERROR`: the runner itself failed.

### `runner_result_t`

```c
typedef struct {
    runner_state_t state;
    int code;
    int elapsed_ms;
} runner_result_t;
```

For `RUNNER_EXITED`, `code` is the exit code.

For `RUNNER_SIGNALED`, `code` is the signal number.

For `RUNNER_TIMEOUT`, `code` should be a timeout-specific value such as `-1`.

For `RUNNER_ERROR`, `code` should be `-1`.

## Required Functions

### `runner_result_t runner_make_error(void)`

Returns a standard error result. This function is already implemented. Use it
to keep error paths consistent.

### `runner_result_t runner_classify_wait_status(int status, int elapsed_ms)`

Converts a `waitpid` status into a `runner_result_t`.

Use:

```c
WIFEXITED(status)
WEXITSTATUS(status)
WIFSIGNALED(status)
WTERMSIG(status)
```

Return `RUNNER_ERROR` if the status is not recognized.

### `runner_result_t runner_run(char *const argv[], int timeout_ms, const char *stdout_path, const char *stderr_path)`

Runs a target program.

Parameters:

- `argv`: null-terminated argument vector for `execvp`;
- `timeout_ms`: maximum runtime in milliseconds;
- `stdout_path`: path where child stdout should be redirected;
- `stderr_path`: path where child stderr should be redirected.

Required behavior:

1. Validate arguments.
2. Fork a child.
3. In the child:
   - open stdout path;
   - open stderr path;
   - redirect descriptors with `dup2`;
   - close unneeded descriptors;
   - call `execvp(argv[0], argv)`;
   - call `_exit(127)` if `execvp` fails.
4. In the parent:
   - wait for child using `waitpid`;
   - enforce timeout;
   - kill child if timeout expires;
   - reap child after killing;
   - classify result.

## Timeout Design

A simple correct timeout is better than a clever flaky one.

Recommended beginner approach:

```text
elapsed = 0
loop:
    waitpid(pid, &status, WNOHANG)
    if child done: classify
    if elapsed >= timeout: kill child, reap child, return timeout
    sleep 1-5 ms
    update elapsed
```

Do not busy-spin at 100% CPU. Sleep briefly between polls.

## Output Redirection

The child should redirect stdout and stderr before `execvp`.

For stdout:

```text
open stdout_path
dup2(opened_fd, STDOUT_FILENO)
close opened_fd
```

Do the same for stderr.

If redirection fails in the child, call `_exit(127)` or another clear failure
code. Do not return into parent-style code.

## Common Bugs

- Calling `exit` instead of `_exit` in the child after failed `exec`.
- Forgetting the final `NULL` in `argv`.
- Parent accidentally redirects its own output.
- Killing a child on timeout but not reaping it.
- Treating signal death as a normal exit code.
- Failing to handle `fork` errors.
- Waiting forever because timeout logic only uses blocking `waitpid`.

## Tests You Should Add

Provided tests are a starting point. Add tests for:

- `/bin/true`;
- `/bin/false`;
- nonexistent program;
- timeout with `/bin/sleep`;
- stdout redirection;
- stderr redirection;
- signal death if you create a small crashing target.

## Grading Breakdown

| Area | Weight |
|---|---:|
| Status classification | 20% |
| Correct fork/exec behavior | 25% |
| Output redirection | 20% |
| Timeout and cleanup | 25% |
| Tests and style | 10% |

## Debugging Checklist

- Use `ps` to check that no child processes remain.
- Print diagnostics to `stderr`.
- Use `strace` if available to inspect system calls.
- In GDB, follow child processes only after you understand the parent path.
- Verify files are closed on both success and failure.

## Stretch

Capture stdout/stderr into temporary files and return their paths in an extended
result type. This is useful preparation for FuzzLab crash triage.
