# HW05: Concurrent Server Foundations

## Theme

This assignment is inspired by the multithreaded snake server, but it is scoped
as a foundation project. You will implement protocol parsing and thread-safe
shared state first. Networking can be added as a stretch or used as preparation
for the C++ capstone.

## Learning Goals

- Parse a simple protocol exactly.
- Protect shared state with a mutex.
- Avoid holding locks during blocking work.
- Write Python integration tests around a C/C++ binary.
- Prepare for a real concurrent server capstone.

## Protocol

The starter protocol is text-based:

```text
JOIN name
MOVE player_id UP
MOVE player_id DOWN
MOVE player_id LEFT
MOVE player_id RIGHT
LEAVE player_id
STATE
```

The required C unit functions do not need to open sockets. They parse commands
and update state. This keeps the core logic testable.

## Provided

- `include/server_core.h`
- `src/server_core.c`
- `tests/test_server_core.cpp`
- `python/smoke_client.py`

## Your Tasks

1. Implement `server_state_init`.
2. Implement `server_join`.
3. Implement `server_leave`.
4. Implement `server_parse_command`.
5. Implement `server_apply_command`.
6. Add locking around shared state.

## Commands

```bash
cmake --preset hw05
cmake --build --preset hw05
ctest --preset hw05 --output-on-failure
```

## Stretch

Add a TCP server with one thread per client, then replace it with a thread pool.
