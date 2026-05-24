# Homework 5: Concurrent Server Core

Readme version 2.0

## Introduction

In this assignment, you will implement the core logic for a small concurrent
server. This is inspired by a multithreaded multiplayer game server, but the
required portion is intentionally focused on the parts that should be unit
tested before sockets enter the picture:

- protocol parsing;
- shared server state;
- player registration;
- state updates;
- mutex-protected critical sections.

Networking is an optional stretch for this homework because the C++ capstone
will later turn the same ideas into a full TCP service.

The goal is to learn:

- how to identify shared state;
- how to protect state with `pthread_mutex_t`;
- how to parse commands exactly;
- how to avoid holding locks around slow/blocking work;
- how Python can be used as a client/testing tool.

## Base Code Policy

The public interface is in `include/server_core.h`. Treat it as the contract.
Do not change struct layouts, enum values, or function signatures.

The implementation belongs in `src/server_core.c`. You may add `static` helper
functions.

## Directory Layout

```text
hw05-concurrent-server/
├── CMakeLists.txt
├── README.md
├── include/
│   └── server_core.h
├── src/
│   └── server_core.c
├── tests/
│   └── test_server_core.cpp
└── python/
    └── smoke_client.py
```

## Build and Test

```bash
cmake --preset hw05
cmake --build --preset hw05
ctest --preset hw05 --output-on-failure
```

## Why This Is Split From Networking

A common beginner mistake is to write the socket server first and bury all game
or state logic inside client-handler threads. That makes bugs hard to isolate.

This assignment forces a cleaner architecture:

```text
socket/client layer
  parses lines from network
  calls server_core functions

server_core layer
  owns shared state
  validates commands
  updates players
```

Only the core layer is required here. If the core is correct, the networking
layer later becomes much easier.

## Protocol

The starter protocol is newline-delimited text:

```text
JOIN name
MOVE player_id UP
MOVE player_id DOWN
MOVE player_id LEFT
MOVE player_id RIGHT
LEAVE player_id
STATE
```

This protocol is intentionally simple. It lets you focus on exact parsing and
state transitions before binary framing and partial socket reads.

## Data Structures

### `player_t`

```c
typedef struct {
    int active;
    char name[SERVER_NAME_MAX + 1];
    move_dir_t last_move;
} player_t;
```

`active == 0` means the slot is free.

`active == 1` means the slot belongs to a connected player.

### `server_state_t`

```c
typedef struct {
    pthread_mutex_t mutex;
    player_t players[SERVER_MAX_PLAYERS];
    int active_count;
} server_state_t;
```

This struct is shared state. Every function that reads or writes `players` or
`active_count` must use the mutex unless the assignment explicitly says
otherwise.

## Required Functions

### `int server_state_init(server_state_t *state)`

Initializes the state object.

Required behavior:

- validate `state`;
- zero player slots;
- set `active_count` to `0`;
- initialize `mutex`.

Return `0` on success and `-1` on failure.

### `void server_state_destroy(server_state_t *state)`

Destroys synchronization resources. It should safely handle `NULL`.

Do not destroy a mutex while other threads may still use it. The required unit
tests are single-process, but your design should respect real threaded use.

### `int server_join(server_state_t *state, const char *name)`

Registers a player.

Required behavior:

1. Validate arguments.
2. Lock the mutex.
3. Find the first inactive slot.
4. Copy name into the slot with truncation safety.
5. Set default last move to `DIR_UP`.
6. Increment `active_count`.
7. Unlock the mutex.
8. Return assigned player id.

Return `-1` if no slots are available or input is invalid.

### `int server_leave(server_state_t *state, int player_id)`

Removes a player.

Required behavior:

- validate id;
- lock mutex;
- reject inactive slot;
- clear player state;
- decrement `active_count`;
- unlock mutex.

Return `0` on success and `-1` on failure.

### `int server_parse_command(const char *line, command_t *out)`

Parses one command line into a `command_t`.

Required behavior:

- reject `NULL`;
- reject unknown commands;
- reject malformed commands;
- reject invalid directions;
- fill all relevant fields in `out`;
- set `CMD_INVALID` only for invalid command objects before returning failure.

Exact forms:

```text
JOIN <name>
MOVE <id> <direction>
LEAVE <id>
STATE
```

Names may be limited to `SERVER_NAME_MAX`.

### `int server_apply_command(server_state_t *state, const command_t *command)`

Applies a parsed command to state.

Required behavior:

- `JOIN`: call `server_join`;
- `LEAVE`: call `server_leave`;
- `MOVE`: update active player's `last_move`;
- `STATE`: no mutation, return success;
- invalid command: return failure.

The `MOVE` case must lock the mutex while checking and updating the player.

## Synchronization Rules

Use the mutex for:

- reading or writing `players`;
- reading or writing `active_count`;
- checking whether a player id is active;
- updating `last_move`.

Do not use the mutex for:

- parsing a command string;
- formatting a response string;
- blocking socket I/O in the stretch server.

This distinction matters. Lock shared memory, not everything.

## Suggested Implementation Order

1. Implement `server_state_init` and `destroy`.
2. Implement `server_join`.
3. Implement `server_leave`.
4. Implement direction parsing as a helper.
5. Implement `server_parse_command`.
6. Implement `server_apply_command`.
7. Add tests for invalid commands.
8. Add basic multithreaded stress test.

## Common Bugs

- Forgetting to unlock on an error path.
- Updating `active_count` without a lock.
- Accepting malformed command strings.
- Using `strcpy` into fixed-size buffers.
- Treating inactive player ids as valid.
- Holding the mutex while doing slow I/O in stretch work.
- Destroying the mutex before all worker threads exit.

## Tests You Should Add

- join fills first available slot;
- join fails when full;
- leave rejects inactive id;
- move rejects inactive id;
- parser rejects extra/missing arguments;
- parser rejects bad direction;
- repeated join/leave keeps `active_count` correct;
- simple threaded test where many threads join/leave.

## Python Client

`python/smoke_client.py` is provided for the optional networking stretch. It is
not used by the required unit tests. It demonstrates the shape of an integration
client:

```bash
python3 python/smoke_client.py 127.0.0.1 9090
```

If you implement a TCP wrapper, use Python to test behavior from outside the C
process.

## Grading Breakdown

| Area | Weight |
|---|---:|
| State initialization/cleanup | 15% |
| Join/leave correctness | 25% |
| Parser correctness | 25% |
| Command application and locking | 25% |
| Tests and style | 10% |

## Debugging Checklist

- Print state to `stderr`, never stdout, while debugging.
- Use ThreadSanitizer if available.
- Add asserts for `active_count`.
- Review every function for lock/unlock symmetry.
- Test invalid input as much as valid input.

## Stretch: TCP Server

Build a simple server around the core:

1. Create listening socket.
2. Accept clients.
3. Spawn one thread per client.
4. Read newline-delimited commands.
5. Parse with `server_parse_command`.
6. Apply with `server_apply_command`.
7. Send text responses.

Do not hold the state mutex while blocked in `recv` or `send`.
