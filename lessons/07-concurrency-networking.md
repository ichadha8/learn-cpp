# Lesson 07: Networking, Threads, Synchronization, and Parallelism

## What This Lesson Teaches

Networking and threading are where many systems programs become nondeterministic.
The code may work once, fail under load, and then pass again. That is why this
lesson emphasizes design rules and test strategy, not just APIs.

By the end, you should be able to:

- explain TCP client/server API order;
- distinguish listening and connected sockets;
- handle message framing over a byte stream;
- identify shared state in threaded programs;
- use mutexes and condition variables;
- explain races, deadlocks, and speedup limits.

## C++ Reading Refresh

LearnCpp is not a socket or pthreads reference, so use `man` pages and Beej's
Guide for the C/POSIX side of this lesson. For the C++ portions, review:

- destructors and cleanup: https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/
- `std::unique_ptr`: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/
- `std::move`: https://www.learncpp.com/cpp-tutorial/stdmove/
- containers and arrays: https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/
- standard algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/

## TCP Server API Flow

Server side:

```text
socket -> bind -> listen -> accept -> recv/send -> close
```

Meaning:

- `socket`: create endpoint descriptor.
- `bind`: attach local address and port.
- `listen`: mark socket as accepting connections.
- `accept`: wait for a client and return a connected descriptor.

Important distinction:

- listening descriptor accepts new clients;
- connected descriptor communicates with one client.

Do not accidentally read client data from the listening descriptor.

## TCP Client API Flow

Client side:

```text
socket -> connect -> recv/send -> close
```

`connect` initiates the connection to the server address.

## TCP Is a Byte Stream

TCP does not preserve your application message boundaries.

If the client sends:

```text
SET a 1\n
GET a\n
```

the server might receive:

```text
SET a
```

then:

```text
 1\nGET a\n
```

or both commands at once. Your protocol parser must handle partial and combined
reads.

For text protocols, keep a per-client input buffer and extract complete lines.

For binary protocols, read exactly the fixed-size header, then exactly the
payload length.

## Threads vs Processes

Processes:

- separate address spaces;
- stronger isolation;
- heavier creation and communication;
- communicate through IPC or shared files/sockets.

Threads:

- share address space;
- cheaper communication through memory;
- easier to race;
- each has its own stack and registers.

Key rule: a memory location is shared if more than one thread can access it,
regardless of whether it is global, heap, or a stack variable passed by pointer.

## Pthreads and C++ Threads

C:

```c
pthread_t tid;
pthread_create(&tid, NULL, worker, arg);
pthread_join(tid, NULL);
```

C++:

```cpp
std::thread worker(fn);
worker.join();
```

Detached threads clean up automatically when they exit, but you cannot join
them later. Joinable threads must be joined or detached.

## Race Conditions

A race occurs when correctness depends on timing.

Classic example:

```c
cnt++;
```

This is usually:

```text
load cnt
add 1
store cnt
```

Two threads can interleave and lose an update.

## Mutexes

A mutex protects a critical section:

```c
pthread_mutex_lock(&mutex);
/* shared state */
pthread_mutex_unlock(&mutex);
```

C++ RAII style:

```cpp
std::lock_guard<std::mutex> lock(mutex_);
/* shared state */
```

The destructor unlocks automatically.

Design rule: hold locks for the shortest time that protects the invariant. Do
not hold a store mutex while blocking on socket I/O.

## Condition Variables

Use condition variables for "sleep until state changes."

C++ shape:

```cpp
std::unique_lock<std::mutex> lock(mutex);
cv.wait(lock, [&] { return !queue.empty() || stopping; });
```

Always wait with a predicate or in a loop. Spurious wakeups are allowed.

## Producer/Consumer

A thread pool is a producer/consumer system:

- producers submit tasks;
- workers consume tasks;
- a mutex protects the queue;
- a condition variable wakes workers.

Shutdown must be designed. Decide whether shutdown:

- finishes queued tasks;
- discards queued tasks;
- rejects new tasks;
- joins all workers.

## Deadlock

Deadlock occurs when threads wait forever for conditions that cannot become
true.

Common two-lock deadlock:

```text
Thread A: lock X, wait for Y
Thread B: lock Y, wait for X
```

Prevention:

- acquire locks in a consistent global order;
- avoid nested locks when possible;
- never wait on unbounded I/O while holding a shared-state lock;
- release locks on all control paths.

## Thread Safety and Reentrancy

Thread-safe means a function behaves correctly when called by multiple threads.

Reentrant is stronger: the function can be safely interrupted and called again
before the first call finishes, usually because it does not depend on shared
mutable state.

Functions returning pointers to static buffers are often not thread-safe.

## Parallelism and Amdahl's Law

Threads can be used for:

- concurrency: overlapping tasks or I/O;
- parallelism: using multiple cores for speed.

More threads do not automatically mean faster code. Speedup is limited by:

- serial portions;
- synchronization overhead;
- memory bandwidth;
- load imbalance;
- cache coherence traffic.

Amdahl's Law:

```text
speedup <= 1 / ((1 - p) + p/k)
```

where `p` is the parallelizable fraction and `k` is the speedup factor for that
part.

## Python Integration Tests

Python is excellent for testing servers:

- start the server as a subprocess;
- open many client sockets;
- send commands;
- assert responses;
- kill or terminate the server;
- collect logs.

This style catches bugs that unit tests cannot, especially protocol and
concurrency bugs.

## Common Traps

- Passing `&i` from a loop to every thread.
- Assuming one `recv` is one message.
- Forgetting to close sockets.
- Holding a mutex during blocking `recv`.
- Updating shared maps without a lock.
- Shutting down while workers still use destroyed state.
- Joining a detached thread.

## Assignment Connection

HW05 teaches thread-safe server core logic before sockets. PulseKV is the full
service version: parser, store, sockets, thread pool, persistence, metrics, and
Python load tests.
