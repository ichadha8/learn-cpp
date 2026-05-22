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
