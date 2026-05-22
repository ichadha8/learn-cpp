# FuzzLab Implementation Notes

## Start Small

Do not start with coverage. Start with a single target execution:

1. write input to a temporary file;
2. fork;
3. child replaces `@@` with temp path;
4. child redirects stdout/stderr;
5. child calls `execvp`;
6. parent waits;
7. parent classifies status.

Once that works, the fuzzer loop is just repetition with better inputs.

## Target Arguments

The target command uses `@@` as the input placeholder:

```bash
fuzzlab -- ./target @@ --mode parse
```

Before `execvp`, create a child argv where `@@` is replaced with the current
input file path.

## Timeout Strategy

Simple version:

- record start time;
- call `waitpid(pid, &status, WNOHANG)`;
- sleep 1 to 5 ms;
- if elapsed time exceeds timeout, kill child;
- reap child.

Do not leave children behind. A fuzzer that creates zombies is broken even if it
finds crashes.

## Crash Saving

Use deterministic names:

```text
crashes/id_000042_sig11_hash1a2b3c4d.input
```

Include metadata in a sidecar file if useful:

```json
{
  "iteration": 42,
  "state": "crash",
  "signal": 11,
  "input_hash": "1a2b3c4d"
}
```

## Coverage Guidance

For the first finished version, fake "interestingness" with:

- new exit code;
- new signal;
- new output size bucket;
- new hash of stderr.

For the advanced version, add real edge coverage with compiler
instrumentation and shared memory.

## Safety Checklist

- Every allocation has a cleanup path.
- Every child is reaped.
- Every file descriptor opened in parent is closed.
- Child calls `_exit` after failed `execvp`.
- Timeout cannot kill the parent.
- Output paths are validated.
- Python scripts can run from repo root.
