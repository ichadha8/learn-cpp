# Lesson 05: Modern C++ Core Language for C Programmers

## What This Lesson Teaches

C++ can be used as "C with classes," but that is not the goal here. The goal is
to use C++ types to make ownership, lifetime, invariants, and errors easier to
reason about.

This lesson assumes you know basic loops, functions, and structs. It focuses on
the C++ ideas that change systems-programming style.

By the end, you should be able to:

- explain references vs pointers;
- use `const` intentionally;
- design a value type;
- explain constructors and destructors;
- explain RAII;
- explain copy vs move;
- choose between raw pointers, references, and smart pointers.

## C++ Is Still Close to the Machine

C++ does not remove memory, object lifetime, or performance concerns. It gives
you stronger tools for expressing them.

In C:

```c
int buffer_init(buffer_t *buffer);
void buffer_destroy(buffer_t *buffer);
```

In C++:

```cpp
class Buffer {
public:
    Buffer();
    ~Buffer();
};
```

The constructor establishes the object's invariant. The destructor releases its
resources automatically.

## References

A reference is an alias for an existing object:

```cpp
void increment(int& value) {
    value++;
}
```

Use references when:

- the argument must exist;
- `nullptr` is not a meaningful state;
- the function does not need to reseat the argument.

Use `const T&` for read-only access to large objects:

```cpp
void print_name(const std::string& name);
```

This avoids copying while promising not to modify the object.

## Pointers in C++

Raw pointers still exist:

```cpp
void maybe_use(int* value);
```

Use raw pointers when:

- `nullptr` is meaningful;
- you are observing but not owning;
- you are interoperating with C APIs.

Do not use raw owning pointers in ordinary modern C++ code. Ownership should be
expressed with values, containers, or smart pointers.

## `const` as a Design Tool

`const` is not decoration. It communicates and enforces read-only behavior.

```cpp
class Store {
public:
    std::size_t size() const;
};
```

The `const` after the method says the method does not modify the observable
state of the object.

Good habit:

- make input references `const` unless mutation is required;
- mark methods `const` when they only inspect state;
- prefer immutable local variables when possible.

## Value Types

A value type behaves like an ordinary value:

```cpp
std::string name = "alice";
std::vector<int> values = {1, 2, 3};
```

You can copy it, move it, pass it around, and let destructors clean it up.

Good C++ often starts by asking: "Can this be a value?"

Examples of good value-like types:

- `std::string`
- `std::vector<T>`
- `std::array<T, N>`
- `std::chrono::milliseconds`
- a small `Command` struct from a parser

## Constructors

A constructor should leave the object ready to use.

```cpp
class Counter {
public:
    explicit Counter(int start) : value_{start} {}

private:
    int value_;
};
```

The member initializer list initializes members directly. Prefer it over
assigning inside the constructor body.

## Destructors

A destructor runs automatically when the object lifetime ends:

```cpp
class UniqueFd {
public:
    ~UniqueFd();
};
```

This is the foundation of RAII. If an object owns a resource, its destructor
should release it.

Resources include:

- heap memory;
- file descriptors;
- sockets;
- mutex locks;
- threads;
- temporary files.

## RAII

RAII means Resource Acquisition Is Initialization.

The idea:

- acquire resource during object initialization;
- release resource in destructor;
- make invalid states hard to represent;
- let scope cleanup handle normal and exceptional paths.

Example:

```cpp
{
    UniqueFd fd(open_file("data.txt"));
    /* use fd */
} /* fd closes here */
```

In C, every early return needs manual cleanup. In C++, RAII makes cleanup
structural.

## Copy vs Move

Copy means duplicate the value.

Move means transfer resources from one object to another.

If a type owns a unique resource, copying is usually wrong:

```cpp
class UniqueFd {
public:
    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept;
    UniqueFd& operator=(UniqueFd&& other) noexcept;
};
```

After a move, the moved-from object must still be safe to destroy. For
`UniqueFd`, that usually means the moved-from fd becomes `-1`.

## Smart Pointers

Use `std::unique_ptr<T>` for unique ownership:

```cpp
auto node = std::make_unique<Node>();
```

Use `std::shared_ptr<T>` only when ownership is genuinely shared. Shared
ownership can hide design problems and has overhead.

Use raw pointers or references for non-owning access.

## LearnCpp Reading Anchors

Use these LearnCpp pages for syntax and examples before starting HW04:

- lvalue references: https://www.learncpp.com/cpp-tutorial/lvalue-references/
- pass by lvalue reference: https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/
- pass by `const` lvalue reference: https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/
- raw pointers in modern C++: https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/
- `std::optional`: https://www.learncpp.com/cpp-tutorial/stdoptional/
- classes: https://www.learncpp.com/cpp-tutorial/introduction-to-classes/
- member functions: https://www.learncpp.com/cpp-tutorial/member-functions/
- public/private members: https://www.learncpp.com/cpp-tutorial/public-and-private-members-and-access-specifiers/
- constructors: https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/
- destructors: https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/
- smart pointers and move semantics: https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
- move constructors and move assignment: https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/
- `std::move`: https://www.learncpp.com/cpp-tutorial/stdmove/

This course then applies those ideas to file descriptors, process runners, and
servers.

## Common Traps

- Writing Java-style `new` everywhere.
- Returning references to local variables.
- Copying a unique resource accidentally.
- Forgetting `virtual` destructors in polymorphic base classes.
- Overusing `shared_ptr`.
- Throwing from destructors.
- Designing classes where construction succeeds but the object is unusable.

## Practice

1. Design a `UniqueFd` class on paper.
2. Mark which operations should be deleted.
3. Decide what value represents "no fd."
4. Explain what happens during move construction.
5. Explain when the fd closes.

## Assignment Connection

HW04 is the direct application of this lesson. PulseKV uses these ideas at
larger scale: sockets, logs, thread pools, and store state should be managed by
objects whose lifetimes are clear.
