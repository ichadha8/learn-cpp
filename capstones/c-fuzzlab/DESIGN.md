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

## Four-File Source Split

The capstone source is split into four files:

```text
src/main.c
src/mutate.c
src/runner.c
src/campaign.c
```

Use this split to keep the project debuggable:

- `main.c`: CLI parsing, config validation, and top-level exit status.
- `mutate.c`: hashing, byte input helpers, and mutation strategies.
- `runner.c`: target argv replacement, fork/exec, redirection, timeout, status classification.
- `campaign.c`: corpus loading, artifact saving, summary writing, and `fuzzlab_run`.

Avoid moving campaign logic into `main.c`. `main.c` should translate command
line arguments into `fuzzlab_config_t`, call `fuzzlab_run`, and return an exit
status.

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

Save timeouts too. A fuzzer that only saves signal crashes hides an important
class of bugs: hangs.

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
- Oversized seeds are rejected.
- Corpus capacity is enforced.
- Summary counters add up.
- Python scripts can run from repo root.
