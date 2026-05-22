# C++ Capstone: PulseKV

## Resume Name

PulseKV: Multithreaded C++ Key-Value Server with RAII, Persistence, and Python
Load Testing

## Pitch

PulseKV is a production-style TCP key-value server written in modern C++. It
uses RAII resource wrappers, STL containers, a thread pool, an append-only log,
structured metrics, and Python tooling for load testing and reporting.

This capstone should feel larger than the homeworks. The goal is not only to
pass a few parser tests. The goal is to build a small service that you can
explain in an interview:

- how clients connect;
- how commands are parsed;
- how shared state is protected;
- how file descriptors and worker threads are cleaned up;
- how persistence works;
- how Python tooling proves the server behaves under load.

## What You Will Build

Required C++ features:

- thread-safe in-memory key-value store;
- strict text protocol parser;
- command execution layer;
- RAII wrapper for sockets and files;
- TCP server using POSIX sockets;
- thread pool for concurrent clients;
- append-only persistence log;
- startup replay from the persistence log;
- structured `STATS` response;
- graceful shutdown path;
- Python client and load-test tooling.

Required Python features:

- `python/client.py`: reusable client library;
- `python/load_test.py`: concurrent load generator;
- `python/report.py`: benchmark summary.

Stretch features:

- snapshot compaction;
- TTL expiration;
- binary protocol mode;
- Prometheus-style metrics;
- chaos testing with random disconnects;
- graceful `SIGINT` handling with all worker threads joined.

## Preparation Reading

Read these before implementation:

- C++ warmup lesson: `../../lessons/05b-cpp-warmup.md`
- Modern C++ core lesson: `../../lessons/05-modern-cpp-core.md`
- STL/RAII lesson: `../../lessons/06-cpp-raii-stl-templates.md`
- LearnCpp smart pointers and move semantics: https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
- LearnCpp `std::unique_ptr`: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/
- LearnCpp standard algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/
- LearnCpp containers and arrays: https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/

For sockets and POSIX behavior, use:

- `man 2 socket`
- `man 2 bind`
- `man 2 listen`
- `man 2 accept`
- `man 2 recv`
- `man 2 send`
- `man 2 close`
- Beej's Guide to Network Programming: https://beej.us/guide/bgnet/html/

## Source File Split

The C++ source is intentionally split into four implementation files:

```text
src/main.cpp
src/store.cpp
src/protocol.cpp
src/server.cpp
```

Keep that split. Do not collapse the project back into a single
`pulsekv.cpp`.

Ownership:

| File | Responsibility |
|:--|:--|
| `src/main.cpp` | CLI parsing, choosing REPL or TCP server mode, top-level exit status |
| `src/store.cpp` | `Store` and `StoreStats` behavior |
| `src/protocol.cpp` | command parsing, key validation, command execution, response formatting |
| `src/server.cpp` | `UniqueFd`, `AppendLog`, `ThreadPool`, `Server`, TCP loop |

The public interface is still declared in `include/pulsekv.hpp` so tests can
compile against one stable header. You may add private helper functions inside
the `.cpp` files, preferably in unnamed namespaces.

## Directory Layout

```text
capstones/cpp-pulsekv
|-- CMakeLists.txt
|-- DESIGN.md
|-- README.md
|-- include
|   `-- pulsekv.hpp
|-- python
|   |-- client.py
|   |-- load_test.py
|   `-- report.py
|-- src
|   |-- main.cpp
|   |-- protocol.cpp
|   |-- server.cpp
|   `-- store.cpp
`-- tests
    `-- test_pulsekv.cpp
```

## Starter Boundary

The starter code gives you:

- public type declarations;
- function and method signatures;
- TODO stubs that compile;
- a placeholder REPL;
- starter unit tests;
- Python tooling skeletons.

The starter intentionally does not implement:

- store mutation;
- parser details;
- command execution;
- RAII socket/file cleanup;
- append-log replay;
- thread pool worker loop;
- TCP accept/read/write loop;
- graceful shutdown.

The hard pieces are left to you because this capstone is meant to be
resume-worthy.

## Build and Test

From the repository root:

```bash
cmake --preset capstone-cpp
cmake --build --preset capstone-cpp
ctest --preset capstone-cpp --output-on-failure
```

The tests are expected to fail before implementation. The scaffold should
compile.

## Protocol

PulseKV uses a newline-delimited text protocol. Every client command ends with
`\n`.

Required commands:

```text
SET key value
GET key
DEL key
STATS
PING
QUIT
```

Responses:

```text
OK
VALUE value
NOT_FOUND
DELETED
STATS keys=<n> commands=<n> sets=<n> gets=<n> dels=<n> misses=<n> clients=<n>
PONG
BYE
ERR message
```

Rules:

- `SET` requires a key and a value.
- `SET` values may contain spaces.
- `GET` requires exactly one key.
- `DEL` requires exactly one key.
- `STATS`, `PING`, and `QUIT` take no arguments.
- Unknown commands are invalid.
- Empty lines are invalid.
- Keys must pass `is_valid_key`.
- Responses must end with exactly one newline.

Do not print debugging text into protocol responses. Protocol output must be
exact because Python integration tests will compare it directly.

## Key Rules

A valid key:

- is non-empty;
- is at most 64 characters;
- contains only letters, digits, `_`, `-`, `.`, and `:`;
- does not begin with `.`.

This keeps parsing simple and prevents path-like persistence surprises.

## Milestones

### Milestone 1: Store

Implement:

- `Store::set`;
- `Store::get`;
- `Store::erase`;
- `Store::stats`.

Done when:

- unit tests can set, get, replace, delete, and miss keys;
- stats counters are accurate;
- public store methods lock internally.

### Milestone 2: Protocol

Implement:

- `is_valid_key`;
- `parse_command`;
- `format_stats`;
- `execute_command`.

Done when:

- malformed input returns `CommandType::Invalid`;
- `SET` preserves spaces in values;
- command responses match the spec exactly;
- no socket code is needed to test parser behavior.

### Milestone 3: RAII and Persistence

Implement:

- `UniqueFd`;
- `AppendLog::open`;
- `AppendLog::append_set`;
- `AppendLog::append_del`;
- `AppendLog::replay`;
- `AppendLog::close`.

Done when:

- file descriptors are not leaked;
- setting and deleting keys writes log entries;
- replay reconstructs store state after restart;
- malformed log lines are skipped or reported consistently.

### Milestone 4: Single-Client TCP Server

Implement:

- listening socket setup;
- `accept`;
- line-based reads;
- full writes using `send`;
- command execution against the shared store;
- clean client disconnect.

Done when:

- `nc 127.0.0.1 9090` can run `SET`, `GET`, `DEL`, `PING`, and `QUIT`;
- Python `client.py` can talk to the server.

### Milestone 5: Thread Pool and Multi-Client Support

Implement:

- `ThreadPool::start`;
- `ThreadPool::enqueue`;
- `ThreadPool::stop`;
- worker loop;
- one client task per accepted connection.

Done when:

- multiple Python clients can run concurrently;
- ThreadSanitizer reports no store data races;
- shutdown joins all worker threads.

### Milestone 6: Load Test and Report

Implement:

- Python load test;
- benchmark output;
- final `REPORT.md`;
- resume bullets based on measured behavior.

Done when:

- `python3 python/load_test.py --clients 16 --ops 1000` completes;
- report includes throughput, latency, error count, and final stats;
- README has final run instructions.

## Public Function Contracts

### `void Store::set(std::string key, std::string value)`

Stores or replaces a key/value pair.

Required behavior:

- lock the store mutex;
- insert the key if missing;
- replace the value if present;
- increment `set_count`;
- update `keys` in stats.

Return type: `void`.

### `std::optional<std::string> Store::get(const std::string& key)`

Looks up a key.

Required behavior:

- lock the store mutex;
- increment `get_count`;
- return the value if found;
- increment `not_found_count` and return `std::nullopt` if missing.

Return type:

- `std::optional<std::string>` containing the value on hit;
- `std::nullopt` on miss.

Do not return references into the map. Returning by value keeps callers from
holding store internals after the lock is released.

### `bool Store::erase(const std::string& key)`

Deletes a key.

Required behavior:

- lock the store mutex;
- increment `del_count`;
- remove the key if present;
- update `keys`;
- increment `not_found_count` if the key was absent.

Return type:

- `true` if a key was removed;
- `false` if no key existed.

### `StoreStats Store::stats() const`

Returns a snapshot of metrics.

Required behavior:

- lock the store mutex;
- copy current stats;
- set `keys` from the current map size;
- return the copy.

Return type: `StoreStats`.

### `bool is_valid_key(const std::string& key)`

Validates protocol keys.

Return type:

- `true` for valid keys;
- `false` for invalid keys.

This function should not print and should not throw for ordinary invalid input.

### `Command parse_command(const std::string& line)`

Parses one line without the trailing newline.

Required valid forms:

```text
SET key value
GET key
DEL key
STATS
PING
QUIT
```

Return type: `Command`.

For valid commands:

- set `type`;
- fill `key` and `value` when applicable;
- leave `error` empty.

For invalid commands:

- set `type = CommandType::Invalid`;
- leave `key` and `value` empty unless useful for debugging;
- set `error` to a short reason such as `empty command`, `invalid key`, or
  `missing value`.

### `std::string execute_command(Store& store, const Command& command)`

Applies one parsed command and returns the exact protocol response.

Required responses:

```text
SET    -> OK\n
GET    -> VALUE <value>\n or NOT_FOUND\n
DEL    -> DELETED\n or NOT_FOUND\n
STATS  -> STATS keys=<n> commands=<n> sets=<n> gets=<n> dels=<n> misses=<n> clients=<n>\n
PING   -> PONG\n
QUIT   -> BYE\n
Invalid -> ERR <message>\n
```

Return type: `std::string`.

This function should not know about sockets. That keeps command behavior unit
testable.

### `std::string format_stats(const StoreStats& stats)`

Formats a `StoreStats` snapshot for the `STATS` response.

Return type: `std::string`.

Required format:

```text
STATS keys=<n> commands=<n> sets=<n> gets=<n> dels=<n> misses=<n> clients=<n>\n
```

### `explicit UniqueFd::UniqueFd(int fd) noexcept`

Creates an owning wrapper around a raw file descriptor.

The invalid descriptor value is `-1`.

### `UniqueFd::~UniqueFd()`

Closes the descriptor if valid.

The destructor must not throw.

### `UniqueFd::UniqueFd(UniqueFd&& other) noexcept`

Transfers descriptor ownership from `other` into `*this`.

After the move, `other.get()` should be `-1`.

### `UniqueFd& UniqueFd::operator=(UniqueFd&& other) noexcept`

Closes the current descriptor if valid, then takes ownership from `other`.

Return type: `UniqueFd&`.

Self-move should not double-close.

### `int UniqueFd::get() const noexcept`

Returns the raw descriptor without transferring ownership.

Return type: `int`.

### `bool UniqueFd::valid() const noexcept`

Returns whether the wrapper owns a descriptor.

Return type: `bool`.

### `int UniqueFd::release() noexcept`

Returns the raw descriptor and makes the wrapper invalid.

Return type: `int`.

The caller becomes responsible for closing the descriptor.

### `void UniqueFd::reset(int fd = -1) noexcept`

Closes the current descriptor if valid, then stores `fd`.

Return type: `void`.

### `explicit AppendLog::AppendLog(std::string path)`

Stores the path to the append-only log.

Construction should not open the file. Opening belongs in `open()` so startup
errors can be reported.

### `bool AppendLog::open()`

Opens or creates the log file.

Return type:

- `true` on success;
- `false` on failure.

Use `UniqueFd` to own the descriptor.

### `bool AppendLog::append_set(const std::string& key, const std::string& value)`

Appends a `SET` record to the log.

Return type:

- `true` when the full record was written and flushed as required;
- `false` on write failure.

### `bool AppendLog::append_del(const std::string& key)`

Appends a `DEL` record to the log.

Return type:

- `true` on success;
- `false` on write failure.

### `bool AppendLog::replay(Store& store)`

Reads the log from the beginning and applies all valid records to `store`.

Return type:

- `true` if replay completed;
- `false` if the log could not be opened or read.

Malformed records should not crash the server.

### `void AppendLog::close() noexcept`

Closes the log descriptor if open.

Return type: `void`.

### `explicit ThreadPool::ThreadPool(std::size_t worker_count)`

Creates a worker pool. You may start workers in the constructor or call
`start(worker_count)` from the constructor.

### `bool ThreadPool::start(std::size_t worker_count)`

Starts worker threads.

Return type:

- `true` when workers started;
- `false` if workers were already running or `worker_count` is invalid.

### `bool ThreadPool::enqueue(std::function<void()> task)`

Pushes a task into the work queue and wakes one worker.

Return type:

- `true` if the task was accepted;
- `false` if the pool is stopping.

### `void ThreadPool::stop()`

Stops accepting work, wakes all workers, and joins all threads.

Return type: `void`.

The destructor must call `stop()`.

### `explicit Server::Server(ServerConfig config)`

Stores server configuration and constructs owned components.

The constructor should not block on `accept`.

### `int Server::run()`

Runs the TCP server.

Required behavior:

1. Open/replay persistence log if enabled.
2. Create listening socket.
3. Bind to `config.port`.
4. Start worker pool.
5. Accept clients in a loop.
6. Enqueue a client handler task for each connection.
7. Stop cleanly when shutdown is requested.

Return type:

- `0` on clean shutdown;
- nonzero on startup/runtime error.

### `void Server::request_stop() noexcept`

Requests server shutdown.

Return type: `void`.

This should be safe to call from signal-handling-adjacent code, though a full
signal-safe design may require an atomic flag or self-pipe.

### `Store& Server::store() noexcept`

Returns the server's store for tests and integration helpers.

Return type: `Store&`.

Do not expose internals of `Store`.

### `int run_server(const ServerConfig& config)`

Convenience wrapper:

```cpp
Server server{config};
return server.run();
```

Return type: `int`.

## Implementation Notes

### Partial Reads

TCP is a byte stream. A single `recv` may return:

- half a command;
- exactly one command;
- multiple commands;
- zero bytes on disconnect.

Use a per-client input buffer. Extract complete lines when `\n` appears.

### Full Writes

`send` may write fewer bytes than requested. Implement a helper that loops until
the whole response is sent or an error occurs.

### Locking

Never hold the store mutex while doing socket I/O.

Good shape:

```text
read line from socket
parse command
execute command against Store
send response
```

Only `Store` methods should lock `Store` internals.

### Persistence Ordering

For required behavior, update the append log before mutating the in-memory store
for `SET` and `DEL`.

If logging fails, return an error response rather than silently losing
persistence.

### Shutdown

A clean shutdown should:

1. stop accepting new clients;
2. close the listening socket;
3. stop the thread pool;
4. close connected clients;
5. close the append log;
6. leave no joinable threads.

## Python Tooling

### `python/client.py`

Should provide a small client wrapper:

```python
client = PulseClient("127.0.0.1", 9090)
client.set("name", "systems")
assert client.get("name") == "systems"
client.close()
```

### `python/load_test.py`

Should:

- start N client threads or processes;
- issue mixed `SET`, `GET`, `DEL`, and `PING` commands;
- record latency and error count;
- print a summary.

### `python/report.py`

Should turn load-test output into a readable report:

- total operations;
- operations per second;
- median latency;
- p95 latency;
- error count;
- final server stats.

## Suggested Tests

Add tests for:

- replacing an existing key;
- deleting a missing key;
- stats counters after mixed commands;
- invalid keys;
- `SET` with value spaces;
- malformed command lines;
- `PING`;
- `STATS` exact formatting;
- `UniqueFd` move and reset behavior;
- append-log replay;
- thread pool accepts and runs tasks;
- server handles two clients concurrently.

## Demo Commands

Build:

```bash
cmake --preset capstone-cpp
cmake --build --preset capstone-cpp
ctest --preset capstone-cpp --output-on-failure
```

Example final server run:

```bash
./build/capstone-cpp/capstones/cpp-pulsekv/pulsekv --port 9090 --log out/pulsekv.log
```

Manual protocol test:

```bash
nc 127.0.0.1 9090
SET name systems
GET name
STATS
QUIT
```

Load test:

```bash
python3 capstones/cpp-pulsekv/python/load_test.py --host 127.0.0.1 --port 9090 --clients 16 --ops 1000
python3 capstones/cpp-pulsekv/python/report.py out/pulsekv-load.json
```

## Grading Breakdown

| Area | Weight | Notes |
|:--|:-:|:--|
| Store correctness and thread safety | 20% | map behavior, stats, locking |
| Parser and response formatting | 15% | exact protocol behavior |
| RAII and persistence | 20% | descriptor cleanup, append log, replay |
| TCP server | 20% | client handling, partial reads/writes |
| Thread pool and shutdown | 15% | concurrency, clean stop, no leaked threads |
| Python tooling and report | 10% | load test, benchmark, final report |

## Resume Bullets

- Built a multithreaded C++ TCP key-value server with RAII socket management,
  thread-pool concurrency, append-only persistence, replay-on-startup, and
  structured metrics.
- Developed Python client, load-testing, and reporting tools to validate
  correctness and measure throughput under concurrent workloads.

## Final Deliverables

Before calling this capstone done, produce:

- passing unit tests;
- working TCP demo;
- Python load-test output;
- `REPORT.md`;
- final resume bullets;
- short explanation of one concurrency bug or shutdown bug you fixed.
