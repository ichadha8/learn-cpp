# Lesson 03: Unix Files, Processes, Redirection, and Signals

## What This Lesson Teaches

Unix systems programming is built around two huge ideas:

1. A process is a running program with its own execution context.
2. Many resources are represented as file descriptors.

By the end, you should be able to:

- explain file descriptors and short reads/writes;
- use `open`, `read`, `write`, `close`, and `dup2`;
- explain `fork`, `exec`, `waitpid`, zombies, and exit status;
- implement a child-process runner with timeout behavior;
- explain why signal handling is tricky.

## File Descriptors

A file descriptor is a small integer in a process's descriptor table.

By convention:

```text
0 = stdin
1 = stdout
2 = stderr
```

The descriptor does not contain the file. It is a handle that the kernel uses to
find an open file description.

Important calls:

```c
int fd = open(path, O_RDONLY);
ssize_t n = read(fd, buf, sizeof buf);
ssize_t m = write(STDOUT_FILENO, buf, (size_t)n);
close(fd);
```

Always check return values. System calls fail.

## Short Counts

`read(fd, buf, n)` means "read up to `n` bytes." It does not promise `n`
bytes.

`write(fd, buf, n)` means "write up to `n` bytes." It does not promise all
bytes were written.

Short counts are common with:

- pipes;
- terminals;
- sockets;
- interrupted system calls;
- nonblocking descriptors.

Robust helper shape:

```c
int read_exact(int fd, unsigned char *buf, size_t len);
int write_all(int fd, const unsigned char *buf, size_t len);
```

These helpers loop until they finish, hit EOF, or see an error.

## `dup2` and Redirection

`dup2(oldfd, newfd)` makes `newfd` refer to the same open file description as
`oldfd`.

Classic stdout redirection:

```c
int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);
```

After this, calls that write to descriptor `1` write to `out.txt`.

This is how shells implement:

```bash
command > out.txt
```

Common trap: `dup2` changes descriptors. It does not copy file contents.

## Processes

A program is code on disk. A process is a running instance.

`fork` creates a child process:

```c
pid_t pid = fork();
if (pid == 0) {
    /* child */
} else if (pid > 0) {
    /* parent */
} else {
    /* error */
}
```

The child starts as a copy of the parent, but the two processes then execute
independently.

## `exec`

`execvp` replaces the current process image with another program:

```c
char *argv[] = {"ls", "-l", NULL};
execvp(argv[0], argv);
_exit(127);
```

If `execvp` succeeds, it does not return. If it returns, it failed. In a child
process, use `_exit`, not `exit`, after failed `exec`, because `exit` can flush
buffers copied from the parent.

## `waitpid` and Zombies

When a child terminates, the kernel keeps a small record containing its exit
status. Until the parent collects that status, the child is a zombie.

```c
int status = 0;
pid_t done = waitpid(pid, &status, 0);
```

Exit-status macros:

```c
WIFEXITED(status)
WEXITSTATUS(status)
WIFSIGNALED(status)
WTERMSIG(status)
```

A process that exits with code `1` is different from a process killed by
`SIGSEGV`.

## Building a Program Runner

The runner pattern appears in HW02, HW03, and FuzzLab.

Parent:

1. create temp input/output paths if needed;
2. `fork`;
3. wait for child;
4. enforce timeout;
5. classify result;
6. clean up files and descriptors.

Child:

1. redirect stdout/stderr if required;
2. replace `@@` placeholder if required;
3. `execvp`;
4. `_exit(127)` if `execvp` fails.

## Timeouts

A simple timeout loop:

```text
start clock
while child not done:
    waitpid(pid, &status, WNOHANG)
    if done: classify
    if elapsed > timeout: kill child and reap it
    sleep a little
```

This is not the most advanced design, but it is good for learning. The crucial
rule is: if you kill a child, you still must reap it.

## Signals

Signals are asynchronous notifications. Examples:

- `SIGINT`: Ctrl-C;
- `SIGCHLD`: child changed state;
- `SIGSEGV`: invalid memory access;
- `SIGALRM`: timer;
- `SIGKILL`: cannot be caught or ignored.

Signal handlers are dangerous because they interrupt normal flow. Many functions
are not async-signal-safe, including `printf` and `malloc`.

Safe beginner pattern:

```c
static volatile sig_atomic_t stop_requested = 0;

static void handle_sigint(int sig) {
    (void)sig;
    stop_requested = 1;
}
```

Then normal code checks `stop_requested`.

## Common Traps

- Parent forgets to reap the child.
- Child continues running parent code after failed `exec`.
- Parent accidentally redirects its own stdout instead of only the child's.
- Code assumes one `read` receives a whole message.
- Signal handler calls unsafe functions.
- Timeout logic kills but does not reap.
- Parent and child both keep pipe ends open, preventing EOF.

## Professor-Style Question Patterns

Likely questions:

- What does `fork` return in parent vs child?
- What happens if `execvp` succeeds?
- What is a zombie?
- What does `dup2(fd, 1)` do?
- Why can `read` return fewer bytes than requested?
- Which status macro tells you a child died from a signal?

## Practice

1. Write pseudocode for `run_with_timeout`.
2. Explain why the child should call `_exit(127)` after failed `execvp`.
3. Explain why closing an already-closed descriptor can be dangerous in threaded
   programs.

## Assignment Connection

HW02 is the pure process-control assignment. HW03 and FuzzLab reuse the same
runner design repeatedly. If HW02 is shaky, the fuzzer projects will be flaky.
