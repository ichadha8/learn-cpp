# Lesson 05: Modern C++ Core

## C++ Is Not Just C With Classes

C++ can call C APIs and can operate close to the machine, but good modern C++
uses the type system to express ownership, lifetime, invariants, and error
handling.

LearnCpp's tutorial progression emphasizes modern C++ basics, debugging,
pointers/references, containers, classes, move semantics, and the standard
library. This course assumes you already know loops and functions, so it focuses
on the parts that change how you design systems code.

## References and Pointers

Use references when:

- the object must exist;
- there is no "not present" state;
- you do not need reseating.

Use raw pointers when:

- `nullptr` is meaningful;
- you are observing, not owning;
- you are interoperating with C APIs.

Use smart pointers when:

- the object is dynamically allocated;
- ownership must be explicit.

## Value Types

A good C++ value type can be copied, moved, compared, and destroyed naturally.

Examples:

- `std::string`
- `std::vector<int>`
- `std::chrono::milliseconds`

Prefer value types until you have a reason to use inheritance or shared
ownership.

## Constructors and Destructors

A constructor establishes an invariant. A destructor releases resources.

If an object owns a resource, the destructor should clean it up:

```cpp
class UniqueFd {
public:
    explicit UniqueFd(int fd) noexcept;
    ~UniqueFd();
};
```

This is RAII: resource acquisition is initialization. The resource lifetime is
bound to an object lifetime.

## Copy and Move

Copy means duplicate the value. Move means transfer resources from one object to
another.

If a class owns a unique resource, copying is often wrong:

```cpp
class UniqueFd {
public:
    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept;
    UniqueFd& operator=(UniqueFd&& other) noexcept;
};
```

Move operations should leave the moved-from object valid and destructible.

## Error Handling

Use the style that fits the boundary:

- exceptions for high-level C++ construction failures;
- `std::optional` for "maybe a value";
- `std::variant` or error objects for structured failure;
- integer error codes at C boundaries;
- `errno` only when wrapping POSIX calls.

For this course, library-like C++ code should avoid surprising process exits.
Return errors to the caller or throw documented exceptions.

## LearnCpp Anchors

Useful LearnCpp topics:

- pointers and references;
- containers and arrays;
- standard library algorithms;
- smart pointers and move semantics;
- `std::unique_ptr` and `std::shared_ptr`.

Use those readings when the syntax feels slippery. Use this course when you need
systems-level application.

## Practice

Write a move-only class that owns a heap buffer:

- destructor frees memory;
- copy is deleted;
- move transfers ownership;
- `data()` returns non-owning pointer;
- tests verify moved-from object is safe.

## Interview Angle

You should be able to answer:

- What is RAII?
- When would you use a reference instead of a pointer?
- What is move semantics?
- Why should a file descriptor wrapper be non-copyable?
