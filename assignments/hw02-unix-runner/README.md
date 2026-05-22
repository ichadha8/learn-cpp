# HW02: Unix Program Runner

## Theme

This assignment is a smaller stepping stone toward a fuzzer. You will write a
Unix process runner that launches another program, redirects output, waits for
completion, and enforces a timeout.

## Learning Goals

- Use `fork`, `execvp`, and `waitpid`.
- Redirect stdout/stderr with `dup2`.
- Classify normal exit, signal death, and timeout.
- Avoid zombies.
- Handle errors without crashing.

## Provided

- `include/runner.h`
- `src/runner.c`
- `tests/test_runner.cpp`

## Your Tasks

1. Implement `runner_classify_wait_status`.
2. Implement `runner_run`.
3. Redirect child stdout/stderr to files or `/dev/null`.
4. Enforce timeout using `waitpid(..., WNOHANG)` and `kill`.
5. Add tests for invalid programs and timeout behavior.

## Commands

```bash
cmake --preset hw02
cmake --build --preset hw02
ctest --preset hw02 --output-on-failure
```

## Implementation Hints

In the child:

1. open redirection target;
2. call `dup2`;
3. close unused descriptors;
4. call `execvp`;
5. if `execvp` fails, call `_exit(127)`.

In the parent:

1. poll child with `waitpid(pid, &status, WNOHANG)`;
2. sleep briefly between polls;
3. if timeout expires, kill child;
4. call `waitpid` again to reap;
5. return structured result.

## Stretch

Capture stdout and stderr into files and expose the paths in the result struct.
