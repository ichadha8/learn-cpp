# Lesson 06: STL Containers, Algorithms, Templates, and RAII Design

## What This Lesson Teaches

Modern C++ code should use the standard library aggressively. If you are writing
your own dynamic array, string type, or hash table in ordinary application code,
you should have a reason.

By the end, you should be able to:

- choose basic STL containers;
- explain iterator and algorithm style;
- avoid unnecessary manual memory management;
- write small templates when they actually reduce duplication;
- wrap C resources in C++ RAII classes.

## Required LearnCpp Reading

Read these pages alongside this lesson:

- containers and arrays: https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/
- `std::vector`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdvector-and-list-constructors/
- vector resizing and capacity: https://www.learncpp.com/cpp-tutorial/stdvector-resizing-and-capacity/
- range-based for loops: https://www.learncpp.com/cpp-tutorial/range-based-for-loops-for-each/
- iterators: https://www.learncpp.com/cpp-tutorial/introduction-to-iterators/
- standard algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/
- function templates: https://www.learncpp.com/cpp-tutorial/function-templates/
- class templates: https://www.learncpp.com/cpp-tutorial/class-templates/
- destructors: https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/
- smart pointers and move semantics: https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
- `std::unique_ptr`: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/
- `std::shared_ptr`: https://www.learncpp.com/cpp-tutorial/stdshared_ptr/

## Standard Library First

Default choices:

- `std::vector<T>` for dynamic contiguous arrays;
- `std::string` for text;
- `std::array<T, N>` for fixed-size arrays;
- `std::unordered_map<K, V>` for hash maps;
- `std::map<K, V>` when sorted order matters;
- `std::deque<T>` for efficient push/pop at both ends;
- `std::optional<T>` for "maybe a value."

The STL is not just convenience. It gives you tested memory management,
iterators, value semantics, and exception safety.

## Contiguous Storage Matters

`std::vector<T>` is often faster than a linked list because its elements are
contiguous. Contiguous memory helps caches and prefetching.

Linked lists are useful for some splice-heavy workloads, but they are not a
default performance win.

## Iterators

An iterator represents a position in a range.

```cpp
for (auto it = values.begin(); it != values.end(); ++it) {
    use(*it);
}
```

`end()` points one past the last element. It is not an element.

Prefer range-for when you do not need iterator operations:

```cpp
for (const auto& value : values) {
    use(value);
}
```

Use `const auto&` to avoid copying large objects.

## Algorithms

Algorithms express intent:

```cpp
std::sort(values.begin(), values.end());
auto it = std::find(values.begin(), values.end(), target);
auto n = std::count_if(values.begin(), values.end(), predicate);
```

Benefits:

- tested;
- generic;
- clear once you learn the idiom;
- often optimized well;
- reduce indexing mistakes.

## `std::optional`

Use `std::optional<T>` when a function may not produce a value:

```cpp
std::optional<std::string> Store::get(const std::string& key);
```

This is clearer than returning an empty string and hoping the caller can tell
whether the key was missing.

## Templates

Templates generate code for types.

Good use:

```cpp
template <typename T>
T max_value(T a, T b) {
    return a < b ? b : a;
}
```

Bad use:

- templating code that only ever has one type;
- hiding complex type errors behind unnecessary generic code;
- writing generic containers before learning `std::vector`.

In this course, templates are useful for small helpers and for understanding
STL-style APIs. They are not the main goal.

## RAII Wrapper Checklist

When wrapping a C resource:

1. Choose the invalid value.
2. Decide whether the wrapper owns the resource.
3. Delete copy if ownership is unique.
4. Implement move if transfer is useful.
5. Release in destructor.
6. Provide `get()` for non-owning access.
7. Provide `release()` only if callers sometimes need to take ownership.
8. Make cleanup functions `noexcept` when possible.

Example shape:

```cpp
class UniqueFd {
public:
    UniqueFd() noexcept = default;
    explicit UniqueFd(int fd) noexcept;
    ~UniqueFd();

    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept;
    UniqueFd& operator=(UniqueFd&& other) noexcept;

    int get() const noexcept;
    int release() noexcept;
    void reset(int fd = -1) noexcept;

private:
    int fd_{-1};
};
```

## Exception Safety

Even if you do not use exceptions heavily, C++ library code may throw.

RAII helps because destructors run during stack unwinding. If a vector
allocation throws, already-constructed local RAII objects still clean up.

Basic goal for this course:

- no leaked file descriptors on early return;
- no leaked heap memory on exceptions;
- objects are destructible after moves;
- failed operations report errors cleanly.

## Common Traps

- Returning iterators after mutating a vector in ways that invalidate them.
- Holding references to vector elements across reallocation.
- Using `operator[]` on maps when you only wanted lookup.
- Using `shared_ptr` where `unique_ptr` or values would be simpler.
- Exposing raw owning pointers.
- Forgetting to join or stop threads in destructors.

## Practice

Implement on paper:

```cpp
class LineReader {
public:
    explicit LineReader(std::string path);
    std::optional<std::string> next();
};
```

Decide:

- what resource it owns;
- what invalid state means;
- whether it can be copied;
- whether it can be moved.

## Assignment Connection

HW04 asks for small RAII wrappers. PulseKV asks for the same idea in a service:
store data in STL containers, use RAII for sockets and files, and use
structured command objects instead of raw string soup.
