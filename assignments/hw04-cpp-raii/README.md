# Homework 4: C++ RAII Systems Library

Readme version 2.0

## Introduction

In the first three homeworks, you worked close to the C/POSIX boundary. You
manually checked return codes, closed file descriptors, and thought about who
owns each allocation. In this assignment, you will translate those ideas into
modern C++.

You will implement a small systems utility library centered on RAII: Resource
Acquisition Is Initialization. The key idea is that a C++ object should acquire
a resource during construction or reset, and release it automatically in its
destructor.

The goal is to learn:

- move-only resource ownership;
- destructors as cleanup;
- `std::vector` for owned buffers;
- `std::optional` for fallible results;
- safe wrappers around POSIX file descriptors;
- how C++ can make C-style resource management less error-prone.

## Required Reading

Read the C++ warmup first if you have not already:

- `lessons/05b-cpp-warmup.md`

Then use these LearnCpp pages while implementing this homework:

- destructors and RAII: https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/
- smart pointers and move semantics: https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
- move constructors and move assignment: https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/
- `std::move`: https://www.learncpp.com/cpp-tutorial/stdmove/
- `std::unique_ptr`: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/
- `std::vector`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdvector-and-list-constructors/
- `std::optional`: https://www.learncpp.com/cpp-tutorial/stdoptional/

## Base Code Policy

The public interface is in `include/raii.hpp`. Do not change class names,
method signatures, or struct fields unless you also update all tests.

The implementation belongs in `src/raii.cpp`. You may add private helper
functions in an unnamed namespace.

## Directory Layout

```text
hw04-cpp-raii/
|-- CMakeLists.txt
|-- README.md
|-- include/
|   `-- raii.hpp
|-- src/
|   `-- raii.cpp
`-- tests/
    `-- test_raii.cpp
```

## Build and Test

```bash
cmake --preset hw04
cmake --build --preset hw04
ctest --preset hw04 --output-on-failure
```

## Concept: RAII

In C, cleanup is manual:

```c
int fd = open(path, O_RDONLY);
if (fd < 0) {
    return -1;
}

/* use fd */

close(fd);
```

Every early return needs cleanup. Every error path is a chance to leak.

In C++, cleanup can be tied to object lifetime:

```cpp
{
    UniqueFd fd(open(path, O_RDONLY));
    /* use fd */
} // fd destructor closes automatically
```

That is the core idea of this homework.

## Required Class: `UniqueFd`

`UniqueFd` owns one POSIX file descriptor.

### Invalid State

The invalid descriptor value is `-1`.

When a `UniqueFd` holds `-1`, it owns nothing and its destructor should do
nothing.

### Copying

Copying must be disabled:

```cpp
UniqueFd(const UniqueFd&) = delete;
UniqueFd& operator=(const UniqueFd&) = delete;
```

Why? If two objects copied the same fd, both destructors would close it. Closing
the same descriptor twice is dangerous because the descriptor number may have
already been reused for something else.

### Moving

Moving transfers ownership.

After:

```cpp
UniqueFd a(open(...));
UniqueFd b(std::move(a));
```

`b` owns the descriptor and `a` should be invalid.

### Required Methods

#### `explicit UniqueFd(int fd) noexcept`

Stores `fd`.

#### `~UniqueFd()`

Closes the descriptor if valid.

#### `UniqueFd(UniqueFd&& other) noexcept`

Transfers ownership from `other`.

#### `UniqueFd& operator=(UniqueFd&& other) noexcept`

Closes the current descriptor if needed, then transfers ownership from `other`.
Self-move should not break the object.

#### `int get() const noexcept`

Returns the raw descriptor without transferring ownership.

#### `bool valid() const noexcept`

Returns whether the object currently owns a descriptor.

#### `int release() noexcept`

Returns the raw descriptor and makes the object invalid. The caller now owns the
descriptor and must close it.

#### `void reset(int fd = -1) noexcept`

Closes the current descriptor if valid, then stores `fd`.

## Required Class: `OwnedBuffer`

`OwnedBuffer` is a small wrapper around `std::vector<unsigned char>`.

Required behavior:

- construct empty;
- construct with a size;
- return `size`;
- return mutable and const `data`;
- append bytes;
- convert to `std::string`.

This class is intentionally simple. The point is to practice using a standard
container as owned storage instead of manually managing `malloc`/`free`.

## Required Functions

### `std::optional<std::string> read_file(const std::string& path)`

Reads an entire file into a string.

Return:

- file contents on success;
- `std::nullopt` on failure.

Do not call `exit`. This is library code; report failure to the caller.

### `bool write_file(const std::string& path, const std::string& contents)`

Writes all contents to a file, replacing it if it already exists.

Return:

- `true` on success;
- `false` on failure.

### `CommandResult run_command_capture_status(const std::vector<std::string>& argv)`

Runs a command and returns status information. This is the C++ version of the
HW02 runner idea.

Minimum requirement:

- fork/exec the command;
- wait for it;
- report normal exit code or signal death.

Stretch:

- capture stdout/stderr;
- support timeout;
- return richer error information.

## Suggested Implementation Order

1. Finish `UniqueFd`.
2. Write tests for move construction and move assignment.
3. Finish `OwnedBuffer`.
4. Implement `write_file`.
5. Implement `read_file`.
6. Implement command status.
7. Add failure-path tests.

## Common Bugs

- Forgetting to invalidate moved-from `UniqueFd`.
- Double-closing descriptors.
- Leaking descriptors on move assignment.
- Using `release` when `get` was intended.
- Returning raw pointers to temporary buffer storage.
- Treating file read failure as an empty file.

## Tests You Should Add

- move assignment closes old descriptor;
- `reset` closes previous descriptor;
- `release` prevents destructor from closing;
- reading nonexistent file returns `nullopt`;
- writing then reading a file round-trips contents;
- command runner detects `/bin/false`.

## Grading Breakdown

| Area | Weight |
|---|---:|
| `UniqueFd` ownership correctness | 35% |
| `OwnedBuffer` correctness | 15% |
| File read/write behavior | 20% |
| Command runner | 20% |
| Tests and style | 10% |

## Debugging Checklist

- Use `lsof` or `/proc` tools if available to inspect open descriptors.
- Run tests under Valgrind.
- Test move operations explicitly.
- Avoid calling `close` on invalid descriptors.
- Check that library functions do not terminate the whole process.

## Stretch

Implement a `Pipe` class:

```cpp
class Pipe {
public:
    UniqueFd read_end;
    UniqueFd write_end;
};
```

Then use it to capture command output.
