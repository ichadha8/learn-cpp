# C++ Capstone: PulseKV

## Resume Name

PulseKV: Multithreaded C++ Key-Value Server with Python Load Testing

## Pitch

PulseKV is a production-style TCP key-value server written in modern C++. It
uses RAII resource wrappers, STL containers, a thread pool, an append-only log,
metrics, and Python tooling for load testing and reporting.

This capstone is designed to show that you can build more than isolated
algorithms. You can build a service.

## What You Will Build

Required C++ features:

- thread-safe in-memory key-value store;
- command parser;
- RAII socket/file wrappers;
- TCP server;
- thread pool;
- append-only persistence log;
- metrics endpoint/command;
- graceful shutdown.

Preparation reading:

- C++ warmup lesson: `../../lessons/05b-cpp-warmup.md`
- STL/RAII lesson: `../../lessons/06-cpp-raii-stl-templates.md`
- LearnCpp smart pointers and move semantics: https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
- LearnCpp `std::unique_ptr`: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/
- LearnCpp standard algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/
- LearnCpp containers and arrays: https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/

Required Python features:

- `python/client.py`: simple client library;
- `python/load_test.py`: concurrent load generator;
- `python/report.py`: benchmark summary.

Optional advanced features:

- snapshot compaction;
- TTL expiration;
- binary protocol;
- Prometheus-style metrics;
- chaos testing with random disconnects.

## Starter Boundary

The starter code provides command/store type declarations, method signatures,
a placeholder REPL, tests that describe expected behavior, and Python tooling
skeletons. It intentionally does not implement the store, parser, TCP server,
thread pool, or persistence layer.

The project should be built in milestones. Do not try to write the full server
first. Get the store and parser correct before any socket code exists.

## Protocol

Start with a newline-delimited text protocol:

```text
SET key value
GET key
DEL key
STATS
QUIT
```

Responses:

```text
OK
VALUE value
NOT_FOUND
DELETED
ERR message
```

Text protocol is easy to debug with `nc`. After the text version works, a
binary protocol is a good stretch.

## Milestones

### Milestone 1: Store

Implement:

- `Store::set`;
- `Store::get`;
- `Store::erase`;
- `Store::stats`.

Done when unit tests pass without networking.

### Milestone 2: Parser

Implement:

- parse valid commands;
- reject malformed commands;
- preserve spaces in values where specified.

Done when parser tests pass.

### Milestone 3: Single-Client Server

Implement:

- listen socket;
- accept one client;
- read lines;
- execute commands;
- write responses.

Done when Python `client.py` can set and get keys.

### Milestone 4: Thread Pool

Implement:

- blocking task queue;
- worker threads;
- safe shutdown;
- client handling tasks.

Done when multiple Python clients can run concurrently.

### Milestone 5: Persistence

Implement:

- append log entries for `SET` and `DEL`;
- replay log on startup;
- flush policy;
- error handling.

Done when data survives restart.

### Milestone 6: Benchmark and Report

Implement:

- Python load test;
- throughput and latency summary;
- final `REPORT.md`.

## Architecture

```text
pulsekv
  Store
    unordered_map
    mutex

  Parser
    text command to Command variant

  Server
    UniqueFd listen socket
    accepts clients
    dispatches to ThreadPool

  ThreadPool
    queue<function<void()>>
    mutex
    condition_variable

  AppendLog
    writes mutations
    replays on startup

python/
  client.py
  load_test.py
  report.py
```

## Public Component Contracts

### `Store`

`Store` owns the in-memory key-value map and its metrics.

Required behavior:

- `set(key, value)` stores or replaces a value;
- `get(key)` returns `std::optional<std::string>`;
- `erase(key)` returns whether a key was removed;
- `stats()` returns a snapshot of counters.

Thread-safety:

- every public method must lock internally;
- never expose references into the internal map;
- do not hold the lock while doing socket I/O.

### `parse_command`

Turns a protocol line into a structured `Command`.

Required valid forms:

```text
SET key value
GET key
DEL key
STATS
QUIT
```

Malformed lines must return `CommandType::Invalid` with a useful error string.

### `execute_command`

Applies one parsed command to a store and returns the exact protocol response.

Examples:

```text
SET a 1  -> OK
GET a    -> VALUE 1
GET nope -> NOT_FOUND
```

### TCP Server

The final server should:

1. create a listening socket;
2. accept clients;
3. read newline-delimited commands;
4. parse commands;
5. execute against shared `Store`;
6. write responses;
7. close clients cleanly;
8. shut down gracefully.

The TCP layer should not know the internals of `Store`; it should call public
methods only.

## Resume Bullets

- Built a multithreaded C++ TCP key-value server with RAII socket management,
  thread-pool concurrency, append-only persistence, and structured metrics.
- Developed Python client, load-testing, and reporting tools to validate
  correctness and measure throughput under concurrent workloads.

## Commands

```bash
cmake --preset capstone-cpp
cmake --build --preset capstone-cpp
ctest --preset capstone-cpp --output-on-failure
```

Example final run:

```bash
./build/capstone-cpp/capstones/cpp-pulsekv/pulsekv --port 9090 --log data/pulsekv.log
python3 capstones/cpp-pulsekv/python/load_test.py --host 127.0.0.1 --port 9090 --clients 16 --ops 1000
```
