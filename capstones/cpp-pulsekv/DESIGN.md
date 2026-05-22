# PulseKV Implementation Notes

## First Rule

Do not start with sockets. Start with the store and parser. Networking makes
bugs harder to reproduce, so keep the core logic testable without a server.

## Store

Use:

```cpp
std::unordered_map<std::string, std::string>
std::mutex
```

Each public method should lock internally. Return values by value for
simplicity. For a learning project, clarity beats micro-optimization.

## Parser

Text parsing should be strict:

- empty line is invalid;
- unknown command is invalid;
- `GET` requires exactly one key;
- `SET` requires key and value;
- value may contain spaces if you choose that spec.

Parser output should be a structured command object, not a raw string.

## Server

Use RAII for sockets:

- constructor/factory opens;
- destructor closes;
- copy deleted;
- move allowed.

Never hold the store mutex while doing blocking socket I/O. Parse outside the
store lock. Lock only while touching the map.

## Thread Pool

The thread pool needs:

- queue;
- mutex;
- condition variable;
- stop flag;
- worker loop;
- graceful destructor.

The hardest part is shutdown. Define exactly what happens to queued tasks when
shutdown starts.

## Persistence

Append-only log format can be simple:

```text
SET key base64_value
DEL key
```

Base64 avoids whitespace ambiguity. A simpler first version can reject values
with newlines and write raw text.

## Metrics

Track:

- total commands;
- set count;
- get count;
- delete count;
- not found count;
- connected clients;
- uptime.

Expose via `STATS`.

## Safety Checklist

- No raw owning pointers.
- All file descriptors are RAII-wrapped.
- Worker threads join on shutdown.
- Parser rejects malformed commands.
- Store methods are thread-safe.
- Python load test can run repeatedly.
