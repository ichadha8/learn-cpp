# Lesson 03: Unix Processes, I/O, and Signals

## Unix Philosophy

Unix programs are designed to compose. Files, pipes, sockets, terminals, and
devices are accessed through file descriptors. Processes communicate through
these abstractions.

That is why file descriptor discipline matters.

## File Descriptors

Every process starts with:

- `0`: stdin;
- `1`: stdout;
- `2`: stderr.

Common calls:

```c
int fd = open(path, O_RDONLY);
ssize_t n = read(fd, buffer, sizeof buffer);
ssize_t m = write(STDOUT_FILENO, buffer, (size_t)n);
close(fd);
```

`read` and `write` can return short counts. Always loop when exact byte counts
matter.

## Redirection

Shell redirection is built from `open` and `dup2`:

```c
int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);
```

After `dup2`, writes to stdout go to the file.

## Processes

`fork` creates a child process. The child starts as a near-copy of the parent.
Both continue from the same line, but `fork` returns different values:

- child receives `0`;
- parent receives child's PID;
- error returns `-1`.

`execvp` replaces the current process image with a new program.

`waitpid` lets the parent reap the child and collect status.

## Exit Status

Use the macros:

```c
if (WIFEXITED(status)) {
    int code = WEXITSTATUS(status);
}

if (WIFSIGNALED(status)) {
    int sig = WTERMSIG(status);
}
```

A process that crashes from `SIGSEGV` is different from a process that exits
with code `139`. Tests often distinguish these.

## Signals

Signals are asynchronous notifications. They are useful but sharp.

Use `sigaction`, not old `signal`, for serious programs.

Signal handlers should do as little as possible. Many library functions are not
async-signal-safe. A common pattern is to set a `volatile sig_atomic_t` flag in
the handler and let normal control flow do cleanup.

## Timeouts

For a child-process runner:

1. fork the child;
2. in the child, redirect output and exec;
3. in the parent, wait with polling or signal-driven logic;
4. if time expires, send `SIGKILL` or `SIGTERM`;
5. reap the child.

For homework, a simple loop using `waitpid(pid, &status, WNOHANG)` and `usleep`
is acceptable. For the capstone, use a cleaner event-driven design.

## Common Bugs

- child accidentally runs parent code after failed `exec`;
- parent forgets to close pipe ends;
- zombies accumulate because parent does not reap;
- blocking read waits forever;
- output redirection leaks into parent;
- timeout kills the wrong process.

## Practice

Write a function:

```c
int run_with_timeout(char *const argv[], int timeout_ms);
```

It should return:

- child exit code if exited normally;
- `128 + signal` if killed by signal;
- a special timeout code if killed by timeout.

## Interview Angle

You should be able to answer:

- What does `fork` copy?
- What does `exec` replace?
- Why do zombies exist?
- What is `dup2` used for?
- Why are signal handlers restricted?
