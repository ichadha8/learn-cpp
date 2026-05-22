# Lesson 07: Concurrency and Networking

## Concurrency Is About Shared Time

Concurrent code has multiple flows of execution whose steps can interleave.
The hard part is not creating threads. The hard part is making shared state
correct for every legal interleaving.

Classic bug classes:

- data races;
- deadlocks;
- missed wakeups;
- lifetime races;
- blocking while holding a lock;
- partial network reads/writes.

## Threads

C uses pthreads:

```c
pthread_t thread;
pthread_create(&thread, NULL, worker, arg);
pthread_join(thread, NULL);
```

C++ uses `std::thread`:

```cpp
std::thread worker(fn);
worker.join();
```

Thread creation is not free. For servers, a thread pool often scales better
than creating a new thread for every tiny task.

## Mutexes

A mutex protects a critical section:

```c
pthread_mutex_lock(&mutex);
/* shared state */
pthread_mutex_unlock(&mutex);
```

In C++, prefer RAII locks:

```cpp
std::lock_guard<std::mutex> lock(mutex_);
```

The destructor unlocks automatically, including on exceptions.

## Condition Variables

Condition variables let threads sleep until state changes. They must be used
with a mutex and a predicate.

```cpp
cv.wait(lock, [&] { return !queue.empty() || stopping; });
```

Always wait in a loop or use the predicate overload. Spurious wakeups are real.

## Sockets

A TCP server usually does:

1. `socket`
2. `setsockopt`
3. `bind`
4. `listen`
5. `accept`
6. `recv` and `send`
7. `close`

TCP is a byte stream, not a message system. If your protocol says a message is
12 bytes, `recv` might return 3 bytes. You must keep reading until the message is
complete or the peer disconnects.

## Protocol Design

Simple protocols are better for learning:

Text protocol:

```text
SET key value
GET key
DEL key
STATS
```

Binary protocol:

```text
1 byte type
2 byte length
payload
```

Binary protocols force you to learn byte order and exact framing. Text protocols
are easier to inspect with `nc`.

## Testing Servers

Unit tests should cover:

- parser behavior;
- store behavior;
- serialization;
- queue behavior.

Python integration tests should cover:

- start server;
- connect client;
- send commands;
- verify responses;
- stress concurrency;
- terminate cleanly.

## Practice

Build a local echo server, then add:

- one command parser;
- shared counter;
- mutex protection;
- Python client test;
- graceful shutdown.

## Interview Angle

You should be able to answer:

- What is a data race?
- Why should you not block on network I/O while holding a mutex?
- Why can `recv` return fewer bytes than requested?
- How do you design a simple thread-safe queue?
